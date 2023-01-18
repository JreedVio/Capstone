#include "Actor.h"
#include "Debug.h"
#include "VulkanRenderer.h"

Actor::Actor(Component* parent_) :
    pool(0), sets(0), renderer(nullptr),
    Component(parent_) {}

bool Actor::OnCreate() {
    renderer = VulkanRenderer::GetInstance();

    renderer->createDescriptorPool(pool);
    createDescriptorSets();

	if (isCreated) return isCreated;
	Debug::Info("Loading assets for Actor: ", __FILE__, __LINE__);
	for (auto component : components) {
		if (component->OnCreate() == false) {
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
			return isCreated;
		}
	}
	isCreated = true;
	return isCreated;
}

Actor::~Actor() {
	OnDestroy();
}
void Actor::OnDestroy() {
	RemoveAllComponents();
	//RemoveComponent();
	Debug::Info("Deleting assets for Actor: ", __FILE__, __LINE__);
	isCreated = false;
}



void Actor::Update(const float deltaTime) {

}
void Actor::Render()const {}

void Actor::RemoveAllComponents() {
	//for (auto component : components) {
	//	component->OnDestroy();
	//	delete component.get();
	//}
	components.clear();
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
	std::cout << '\n';
}

PushConst Actor::GetModelMatrix() {
	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	if (transform) {
		pushConst.model = transform->GetTransformMatrix();
	}
	else {
        pushConst.model.loadIdentity();
	}
	if (parent) {
        pushConst.model = dynamic_cast<Actor*>(parent)->pushConst.model * pushConst.model;
	}
    pushConst.normal = MMath::transpose(MMath::inverse(pushConst.model));

	return pushConst;
}

void Actor::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(renderer->GetSwapChainImages().size(), GetComponent<ShaderComponent>()->descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(renderer->GetSwapChainImages().size());
    allocInfo.pSetLayouts = layouts.data();

    sets.resize(renderer->GetSwapChainImages().size());
    if (vkAllocateDescriptorSets(renderer->GetDevice(), &allocInfo, sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < renderer->GetSwapChainImages().size(); i++) {
        VkDescriptorBufferInfo cameraBufferInfo{};
        cameraBufferInfo.buffer = renderer->GetCameraBuffers()[i];
        cameraBufferInfo.offset = 0;
        cameraBufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorBufferInfo glightsBufferInfo{};
        glightsBufferInfo.buffer = renderer->GetGLightingBuffers()[i];
        glightsBufferInfo.offset = 0;
        glightsBufferInfo.range = sizeof(GlobalLighting);

        // The 2D sampler
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = GetComponent<MaterialComponent>()->textureImageView;
        imageInfo.sampler = GetComponent<MaterialComponent>()->textureSampler;

        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = sets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &cameraBufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = sets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &glightsBufferInfo;

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = sets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(renderer->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Actor::SetPushConst(const Matrix4& model) {
    pushConst.model = model;
    pushConst.normal = MMath::transpose(MMath::inverse(model));
}
