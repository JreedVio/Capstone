#include "MeshComponent.h"
#include "VulkanRenderer.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace MATH;

MeshComponent::MeshComponent(Component* parent_, const char* filename_) : 
    indexBufferID(0), indexBufferMemoryID(0), vertexBufferID(0), vertexBufferMemoryID(0),
    vertices(0), indices(0), renderer(nullptr), filename(filename_), Component(parent_) {
}

MeshComponent::~MeshComponent() {
    OnDestroy();
}

bool MeshComponent::OnCreate() {
    renderer = VulkanRenderer::GetInstance();
    loadModel();
    return true;
}

void MeshComponent::Render() const {
}

void MeshComponent::OnDestroy() {
}

void MeshComponent::Update(const float deltaTime) {}

void MeshComponent::loadModel() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };


            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            // Deduplication

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    createVertexBuffer();
    createIndexBuffer();
}

void MeshComponent::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

    VkDeviceMemory stagingBufferMemoryID;
    VkBuffer stagingBufferID;

    // these are being pulled in by reference, be constatant
    renderer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBufferID, stagingBufferMemoryID);

    void* data;
    vkMapMemory(renderer->GetDevice(), stagingBufferMemoryID, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(renderer->GetDevice(), stagingBufferMemoryID);

    renderer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferID, vertexBufferMemoryID);

    renderer->copyBuffer(stagingBufferID, vertexBufferID, bufferSize);

    vkDestroyBuffer(renderer->GetDevice(), stagingBufferID, nullptr);
    vkFreeMemory(renderer->GetDevice(), stagingBufferMemoryID, nullptr);
}

void MeshComponent::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

    VkDeviceMemory stagingBufferMemoryID;
    VkBuffer stagingBufferID;

    renderer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBufferID, stagingBufferMemoryID);

    void* data;
    vkMapMemory(renderer->GetDevice(), stagingBufferMemoryID, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(renderer->GetDevice(), stagingBufferMemoryID);

    renderer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferID, indexBufferMemoryID);

    renderer->copyBuffer(stagingBufferID, indexBufferID, bufferSize);

    vkDestroyBuffer(renderer->GetDevice(), stagingBufferID, nullptr);
    vkFreeMemory(renderer->GetDevice(), stagingBufferMemoryID, nullptr);
}