#ifndef SCENE_H
#define SCENE_H

#include "VulkanRenderer.h"
#include "Actor.h"
#include "GlobalLighting.h"
#include <unordered_map>

#define LIGHT_NUM 2

union SDL_Event;
class Camera;

class Scene {
public:
	explicit Scene(VulkanRenderer* renderer_):renderer(renderer_){};
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	virtual std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }

	void AddLight(Vec4 pos_, Vec4 colour_) {
		//Don't add light if exceeds max light number
		if (globalLights.size() >= LIGHT_NUM) return;

		Ref<LightActor> light_ = std::make_shared<LightActor>(nullptr);
		light_->SetPosition(pos_);
		light_->SetDiffuse(colour_);
		globalLights.push_back(light_);
	}

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_) {
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			actorList[name_] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a non-actor type to actor list - ignored\n";
#endif
		}
	}
	virtual Ref<Actor> GetActor(const char* name_) = 0;


protected:
	VulkanRenderer* renderer;
	Ref<Camera> camera;
	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::vector<Ref<LightActor>> globalLights;
};
#endif