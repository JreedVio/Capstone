#include "Actor.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "CameraActor.h"
#include "Physics.h"


Actor::Actor(Component* parent_) :
    pool(0), sets(0), renderer(nullptr),
    Component(parent_), visible(true), alpha(1.0f), alphaChange(0.1f), 
    flash(0), updateInterval(0.5f), elapsedTime(0.0f), currentFlash(0) {}

Actor::Actor(const Actor& actor_){
    //std::cout << "Copying actor\n";
    std::vector<Ref<Component>> new_List = std::vector<Ref<Component>>();
    new_List = actor_.GetComponentList();
    components = new_List;
    visible = actor_.GetVisible();
    parent = actor_.GetParent();
    alpha = actor_.GetAlpha();
    alphaChange = actor_.GetAlphaChange();
    flash = actor_.IsFlashing();
    updateInterval = actor_.GetUpdateInterval();
    elapsedTime = 0.0f;
    currentFlash = flash;
}

bool Actor::OnCreate() {
    renderer = VulkanRenderer::GetInstance();
	if (isCreated) return isCreated;

    renderer->createDescriptorPool(pool);
    createDescriptorSets();
	//Debug::Info("Loading assets for Actor: ", __FILE__, __LINE__);
	//for (auto component : components) {
	//	if (component->OnCreate() == false) {
	//		//Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
	//		isCreated = false;
	//		return isCreated;
	//	}
	//}
	isCreated = true;
	return isCreated;
}

Actor::~Actor() {

}
void Actor::OnDestroy() {
    //If the actor is not created, don't destroy it
    if (!isCreated) return;

    VkDevice device = renderer->GetDevice();
    vkDestroyDescriptorPool(device, pool, nullptr);
	//RemoveAllComponents();
	Debug::Info("Deleting assets for Actor: ", __FILE__, __LINE__);
	isCreated = false;
}

void Actor::Update(const float deltaTime) {

    elapsedTime += deltaTime;

    //Update all components
    //for (auto component : components) {
    //    component->Update(deltaTime);
    //}

    //Update Physics
    auto dlm = GetComponent<PHYSICS::DynamicLinearMovement>();
    auto ab = GetComponent<PHYSICS::AABB>();
    if (ab)
    {
        ab->Update(deltaTime);
    }
    if (dlm)
    {		
        dlm->Update(deltaTime);
    }

    //Update alpha if needed
    if (currentFlash != 0) {

        if (elapsedTime >= updateInterval) {

            if (alpha >= 0.01f) {
                alpha -= alphaChange;
                currentFlash--;
            }
            else {
                alpha = 1.0f;
            }
            elapsedTime = 0.0f;
        }

    }



}
void Actor::Render()const {}

void Actor::RemoveAllComponents() {
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
        pushConst.model = dynamic_cast<Actor*>(parent)->GetModelMatrix().model * pushConst.model;
	}

    pushConst.normal = MMath::transpose(MMath::inverse(pushConst.model));
    pushConst.alpha = GetAlpha();
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
