#include "MaterialComponent.h"
#include "VulkanRenderer.h"
MaterialComponent::MaterialComponent(Component* parent_, const char* filename_):
    textureImage(0), textureImageMemory(0), textureImageView(0), textureSampler(0), 
    renderer(nullptr), filename(filename_), Component(parent_) {
}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
    renderer = VulkanRenderer::GetInstance();
    createTextureImage();

	return true;
}

void MaterialComponent::OnDestroy() {	
}
void MaterialComponent::Update(const float deltaTime) {}
void MaterialComponent::Render()const {}

void MaterialComponent::createTextureImage() {
    //SDL_Surface* image = IMG_Load(TEXTURE_PATH.c_str());
    SDL_Surface* image = IMG_Load(filename);
    ///image->format
    // Assumed that it is png
    VkDeviceSize imageSize = image->w * image->h * 4;

    VkDeviceMemory stagingBufferMemoryID;
    VkBuffer stagingBufferID;

    VkDevice device = renderer->GetDevice();

    VulkanRenderer::GetInstance()-> createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferID, stagingBufferMemoryID);

    void* data;
    vkMapMemory(device, stagingBufferMemoryID, 0, imageSize, 0, &data);
    // It's a fast way of transfering information
    memcpy(data, image->pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemoryID);


    renderer->createImage(image->w, image->h, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        textureImage, textureImageMemory);
    renderer->transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    // Loading memory from the staging memory to the actual image memory
    renderer->copyBufferToImage(stagingBufferID, textureImage,
        static_cast<uint32_t>(image->w), static_cast<uint32_t>(image->h));
    renderer->transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBufferID, nullptr);
    vkFreeMemory(device, stagingBufferMemoryID, nullptr);

    SDL_FreeSurface(image);

    createTextureImageView();
    createTextureSampler();
}

void MaterialComponent::createTextureImageView() {
    textureImageView = renderer->createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void MaterialComponent::createTextureSampler() {

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(renderer->GetPhysicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(renderer->GetDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}