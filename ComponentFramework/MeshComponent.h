// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder

#pragma once
#include "Component.h"
#include <vector>
#include "Vector.h"
#include "Hash.h"
#include "VMath.h"
#include "MMath.h"
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <array>


using namespace MATH;

class VulkanRenderer;

struct Vertex {
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
    bool operator == (const Vertex& other) const {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }

}; /// End of struct Vertex

class MeshComponent : public Component {
	// delete all the implicit stuff. Scott doesn't trust them
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

private:
	VulkanRenderer* renderer;

public:
	const char* filename;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer indexBufferID;
	VkDeviceMemory indexBufferMemoryID;

	VkBuffer vertexBufferID;
	VkDeviceMemory vertexBufferMemoryID;

	MeshComponent(Component* parent_, const char* filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void loadModel();
	void createVertexBuffer();
	void createIndexBuffer();
	void Render() const;
};
