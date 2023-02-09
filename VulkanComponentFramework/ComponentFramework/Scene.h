#ifndef SCENE_H
#define SCENE_H

#include "VulkanRenderer.h"
#include "Actor.h"
#include <unordered_map>

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

protected:
	VulkanRenderer* renderer;
	Ref<Camera> camera;
	std::unordered_map<const char*, Ref<Actor>> actorList;
};
#endif