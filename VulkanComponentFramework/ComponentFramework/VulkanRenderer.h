#ifndef VULKANRENDERER_H 
#define VULKANRENDERER_H

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <unordered_map>
#include <array>
#include <chrono>
#include <memory>

#include "Vector.h"
#include "VMath.h"
#include "MMath.h"
#include "GlobalLighting.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Actor.h"

using namespace MATH;


#include "Renderer.h"

template<typename T>
using Ref = std::shared_ptr<T>;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG /// only use validation layers if in debug mode
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = false;
#endif

struct QueueFamilyIndices {
    /// optional means that it contains no value until it is assigned
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    };

    


    namespace std {
        template<> struct hash<Vertex> {
            size_t operator()(Vertex const& vertex) const noexcept {
                size_t hash1 = hash<Vec3>()(vertex.pos);
                size_t hash2 = hash<Vec3>()(vertex.normal);
                size_t hash3 = hash<Vec2>()(vertex.texCoord);
                size_t result = ((hash1 ^ (hash2 << 1)) >> 1) ^ (hash3 << 1);
                return result;
            }
        };
    }


    struct UniformBufferObject {
        Matrix4 view;
        Matrix4 proj;
    };
    

    class VulkanRenderer : public Renderer {
    public:
        /// C11 precautions 
        VulkanRenderer(const VulkanRenderer&) = delete;  /// Copy constructor
        VulkanRenderer(VulkanRenderer&&) = delete;       /// Move constructor
        VulkanRenderer& operator=(const VulkanRenderer&) = delete; /// Copy operator
        VulkanRenderer& operator=(VulkanRenderer&&) = delete;      /// Move operator


        ~VulkanRenderer();
        SDL_Window* CreateWindow(std::string name_, int width, int height);
        bool OnCreate();
        void OnDestroy();
        void Render();
        void SetUBO(const Matrix4& projection, const Matrix4& view);
        void SetGLightsUbo(const GlobalLighting& glights);
        void SetPushConst(const Matrix4& model);
        SDL_Window* GetWindow() {
            return window;
        }
        VkDevice GetDevice(){
            return device;
        }
        VkPhysicalDevice GetPhysicalDevice() {
            return physicalDevice;
        }
        VkRenderPass GetRenderPass() {
            return renderPass;
        }
        VkExtent2D GetSwapChainExtent() {
            return swapChainExtent;
        }
        std::vector<VkImage> GetSwapChainImages() {
            return swapChainImages;
        }
        std::vector<VkBuffer>  GetCameraBuffers() {
            return cameraBuffers;
        }
        std::vector<VkBuffer> GetGLightingBuffers() {
            return glightingBuffers;
        }

        static VulkanRenderer* GetInstance() {
            if (Instance == nullptr) {
                Instance = new VulkanRenderer();
            }
            return Instance;
        }

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createDescriptorPool(VkDescriptorPool& pool_);

private:
    static VulkanRenderer* Instance;

    VulkanRenderer();

    UniformBufferObject ubo;
    GlobalLighting glightsUBO;


    const size_t MAX_FRAMES_IN_FLIGHT = 2;
    SDL_Event sdlEvent;
    uint32_t windowWidth;
    uint32_t windowHeight;
    SDL_Window* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkRenderPass renderPass;

    Ref<Actor> actor[2];

    Ref<ShaderComponent> shaderComponent;
    Ref<MaterialComponent> materialComponent[2];
    Ref<MeshComponent> meshComponent[2];

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkCommandPool commandPool;

    std::vector<VkBuffer> cameraBuffers;
    std::vector<VkDeviceMemory> cameraBuffersMemory;
    std::vector<VkBuffer> glightingBuffers;
    std::vector<VkDeviceMemory> glightingBuffersMemory;


    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    bool framebufferResized = false;

    bool hasStencilComponent(VkFormat format);

    void initVulkan();
    void createInstance();
    void createSurface();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void recreateSwapChain();
    void updateUniformBuffer(uint32_t currentImage);
    void updateGLightsUniformBuffer(uint32_t currentImage);
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createDepthResources();
        /// A helper function for createVertexBuffer()
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createUniformBuffers(VkDeviceSize bufferSize,
        std::vector<VkBuffer>& uniformBuffer, std::vector<VkDeviceMemory>& uniformBufferMemory);
    void destroyUniformBuffer(std::vector<VkBuffer>& uniformBuffer, std::vector<VkDeviceMemory>& uniformBufferMemory);
    void createCommandBuffers();
    void recordCommandBuffer();
    void createSyncObjects();
    void cleanup();
    void cleanupSwapChain();

    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void  populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);   
};
#endif 

