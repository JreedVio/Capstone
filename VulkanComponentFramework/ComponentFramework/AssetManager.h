#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Debug.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include "Actor.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class VulkanRenderer;
class MeshComponent;
class MaterialComponent;
class ShaderComponent;
class Room;
class Scene;

enum LoadType {
	ASSETS,
	SCENE
};

class AssetManager{
private:
	static AssetManager* instance_;
	VulkanRenderer* renderer;
	XMLDocument assetDoc;
	XMLDocument sceneDoc;
	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::unordered_map<const char*, Ref<Component>> componentCatalog;
	std::unordered_map<const char*, Ref<ShaderComponent>> shaderList;

	XMLElement* ReadManiFest(LoadType loadType, const char* fileName_);
	AssetManager();
	void AddComponentData(XMLElement* componentData);
	bool CreateComponents();
	void AddActorData(XMLElement* actorData);
	bool CreateActors();
	Scene* CreateRoom(XMLElement* roomData);

public:
	static AssetManager* GetInstance();

	~AssetManager();
	bool OnCreate();
	void LoadAssets(const char* fileName_);
	Scene* LoadRoom(const char* sceneName_);
	void OnDestroy();

	std::unordered_map<const char*, Ref<Component>> GetComponentList() { return componentCatalog; }
	std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }
	std::unordered_map<const char*, Ref<ShaderComponent>> GetShaderList() { return shaderList; }

	Ref<Actor> GetActor(const char* name_);
	Ref<MaterialComponent> GetMaterial(const char* name_);
	Ref<MeshComponent> GetMesh(const char* name_);
	Ref<ShaderComponent> GetShader(const char* name_);
	
	void RemoveAllComponents();

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_) {
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			//Need to check memory for the string
			//std::string* temp_name = new std::string();
			//*temp_name = name_;
			actorList[name_] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type to actor list - ignored\n";
#endif
		}
	}

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		t->OnCreate();
		if (std::dynamic_pointer_cast<ShaderComponent>(t)) {
			shaderList[name_] = std::dynamic_pointer_cast<ShaderComponent>(t);
		}
		else {
			componentCatalog[name_] = t;
		}

	}

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Ref<ComponentTemplate> component_) {
		component_->OnCreate();
		if (std::dynamic_pointer_cast<ShaderComponent>(component_)) {
			shaderList[name_] = component_;
		}
		else {
			componentCatalog[name_] = component_;
		}
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name_) const {
		Ref<Component> id;
		//Find only compares the address, therefore need to use the actual value to compare
		for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
			std::string key = c.first;
			std::string givenkey = name_;
			if (key == givenkey) {
				id = c.second;
			}
		}
#ifdef _DEBUG
		if (!id) {
			Debug::Error("Cannot find component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id);
	}

};

#endif