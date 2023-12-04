#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"
#include <vector>

class LightActor;

class MenuScene : public Scene{

public:
	MenuScene(VulkanRenderer* renderer_);
	virtual ~MenuScene();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	virtual Ref<Actor> GetActor(const char* name_) override { return nullptr; }

private:

};

#endif
