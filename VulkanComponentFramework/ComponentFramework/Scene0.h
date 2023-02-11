#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Camera.h"
#include "GlobalLighting.h"


using namespace MATH;

/// Forward declarations 
union SDL_Event;

template<typename T>
using Ref = std::shared_ptr<T>;

class Scene0 : public Scene {
private:
	Renderer *renderer;
	//Ref<Camera> camera;	
	Matrix4 mariosModelMatrix;

	Ref<LightActor> light;
	std::vector<Ref<LightActor>> lights;
	
public:
	explicit Scene0(VulkanRenderer* renderer_);
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	Renderer* getRenderer() { return renderer; }
};


#endif // SCENE0_H