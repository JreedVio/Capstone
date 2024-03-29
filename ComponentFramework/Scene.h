#ifndef SCENE_H
#define SCENE_H

#include "VulkanRenderer.h"
#include "Actor.h"
#include "GlobalLighting.h"
#include <unordered_map>
#include <memory>

#define LIGHT_NUM 2

union SDL_Event;
class CameraActor;

enum SceneStatus {
	MENUTRANSIT = 0,
	ROOMTRANSIT,
	REGULAR
};

class Scene {
public:
	explicit Scene(VulkanRenderer* renderer_):renderer(renderer_), sceneStatus(REGULAR){};
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


	virtual Ref<Actor> GetActor(const char* name_) = 0;
	virtual SceneStatus GetStatus() { return sceneStatus; }
	virtual void SetStatus(SceneStatus status_) { sceneStatus = status_; }
	virtual const char* GetSceneName() { return sceneName; }
	virtual void SetSceneName(const char* sceneName_) { sceneName = sceneName_; }

protected:
	VulkanRenderer* renderer;
	Ref<CameraActor> camera;
	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::vector<Ref<LightActor>> globalLights;
	SceneStatus sceneStatus;
	const char* sceneName;
};
#endif