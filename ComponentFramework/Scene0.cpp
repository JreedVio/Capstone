#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "Camera.h"

Scene0::Scene0(VulkanRenderer* renderer_):
	Scene(nullptr),renderer(renderer_){
	camera = std::make_shared<Camera>();

	lights.push_back(light);
	lights.push_back(light);
	lights[0] = std::make_shared<LightActor>();
	lights[1] = std::make_shared<LightActor>();	
	

	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	OnDestroy();
}

bool Scene0::OnCreate() {
	float aspectRatio;
	int width, height;
	switch (renderer->getRendererType()){
	case RendererType::VULKAN:
		SDL_GetWindowSize(dynamic_cast<VulkanRenderer*>(renderer)->GetWindow(), &width, &height);
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		//camera->Perspective(45.0f, aspectRatio, 0.5f, 20.0f);
		//camera->LookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, -3.0f), Vec3(0.0f, 1.0f, 0.0f));
		lights[0]->SetPosition(Vec4(5.0f, 0.0f, -5.0f, 0.0f));
		lights[1]->SetPosition(Vec4(-5.0f, 0.0f, -5.0f, 0.0f));
		lights[0]->SetDiffuse(Vec4(0.0f, 0.0f, 1.0f, 1.0f));		
		lights[1]->SetDiffuse(Vec4(1.0f, 0.0f, 0.0f, 1.0f));		
		
		break;

	case RendererType::OPENGL:
		break;
	}

	return true;
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	if (sdlEvent.type == SDL_WINDOWEVENT) {
		switch (sdlEvent.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		std::cout << "Change size Event\n";
		float aspectRatio = static_cast<float>(sdlEvent.window.data1) / static_cast<float>(sdlEvent.window.data2);
		camera->Perspective(45.0f, aspectRatio, 0.5, 20.0f);
		break;

		}
	}
	
}

void Scene0::Update(const float deltaTime) {
	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime;
	mariosModelMatrix = MMath::rotate(elapsedTime * 90.0f, Vec3(0.0f, 1.0f, 0.0f));
}

void Scene0::Render() const {
	switch (renderer->getRendererType()) {
	case RendererType::VULKAN:
		VulkanRenderer* vRenderer;
		vRenderer = dynamic_cast<VulkanRenderer*>(renderer);
		vRenderer->SetUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
		vRenderer->SetGLightsUbo(lights);
		
		//vRenderer->SetGLightsUbo(lights[1]);
		vRenderer->SetPushConst(mariosModelMatrix);
		vRenderer->Render();
		break;

	case RendererType::OPENGL:
		/// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		/// Draw your scene here
		
		glUseProgram(0);
		
		break;
	}
}


void Scene0::OnDestroy() {
	for (auto actor_ : actorList) {
		actor_.second->OnDestroy();
	}
}
