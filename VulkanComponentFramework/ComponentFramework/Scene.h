#ifndef SCENE_H
#define SCENE_H

#include "Renderer.h"
#include "Actor.h"
#include <unordered_map>

union SDL_Event;
class Camera;

class Scene {
public:
	explicit Scene(Renderer* renderer_):renderer(nullptr){};
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }

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
	Renderer *renderer;
	Ref<Camera> camera;
	std::unordered_map<const char*, Ref<Actor>> actorList;
};
#endif