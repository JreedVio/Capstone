#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
#include <vulkan/vulkan.h>
#include <array>


using namespace MATH;

class VulkanRenderer;

class Actor : public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

private:
	std::vector< Ref<Component> > components;
	Matrix4 modelMatrix;

	VulkanRenderer* renderer;

public:

	VkDescriptorPool pool;
	std::vector<VkDescriptorSet> sets;

	Actor(Component* parent_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

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

	Matrix4 GetModelMatrix();
	void createDescriptorPool();
	void createDescriptorSets();
};
