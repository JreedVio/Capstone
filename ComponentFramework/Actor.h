#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
#include "QMath.h"
#include <vulkan/vulkan.h>
#include <array>


using namespace MATH;

class VulkanRenderer;

enum ActorType {
	DEFAULT = 0,
	WALL,
	FLOOR
};

struct PushConst {
	Matrix4 model;
	Matrix4 normal;
	alignas(4)float alpha;
};

class Actor : public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	//Actor(const Actor&) = delete; //test with copy contructor
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

protected:
	std::vector<Ref<Component>> components;
	ActorType actorType;
	bool visible;
	bool rotate;
	bool move;
	float updateInterval;
	float alpha;
	float alphaChange;
	float elapsedTime;
	int flash; // controls the flashing times, -1 = loop, 0 = don't falsh
	int currentFlash;
	VulkanRenderer* renderer;

public:
	VkDescriptorPool pool;
	std::vector<VkDescriptorSet> sets;
	
	PushConst pushConst;
	
	Actor(Component* parent_);
	Actor(const Actor& actor_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	ActorType GetActorType() const { return actorType; }
	void SetActorType(ActorType actorType_) { actorType = actorType_; }
	float GetUpdateInterval() const { return updateInterval; }
	void SetUpdateInterval(float updateInterval_) { updateInterval = updateInterval_; }
	float GetAlpha() const { return alpha; }
	void SetAlpha(float alpha_) { alpha = alpha_; }
	float GetAlphaChange() const { return alphaChange; }
	void SetAlphaChange(float alphaChange_) { alphaChange = alphaChange_; }
	int IsFlashing() const { return currentFlash; }
	void SetFlashing(int flash_) { 
		flash = flash_;	
		currentFlash = flash;
	}
	void ResetFlash() {
		currentFlash = flash;
	}
	bool GetVisible() const { return visible; }
	void SetVisible(bool visible_) { visible = visible_; }

	std::vector<Ref<Component>> GetComponentList() const { return components; }

	template<typename ComponentTemplate>
	void AddComponent(Ref<ComponentTemplate> component_) {
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		components.push_back(component_);
	}


	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		/// before you add the component, ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		/// Using std::make_shared to do the work. This is the new idea!,
		components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	}


	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent() const {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component.get())) {
				return std::dynamic_pointer_cast<ComponentTemplate>(component);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (unsigned int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i].get()) != nullptr) {
				components.erase(components.begin() + i);
				break;
			}
		}
	}
	void RemoveAllComponents();

	void ListComponents() const;

	virtual PushConst GetModelMatrix();
	void createDescriptorSets();
	void SetPushConst(const Matrix4& model);

	virtual void CollisionResponse(){}
	virtual void NotCollided(){}
};
