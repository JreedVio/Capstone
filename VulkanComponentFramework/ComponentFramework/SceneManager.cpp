#include <SDL.h>
#include "SceneManager.h"
#include "VulkanRenderer.h"
#include "OpenGLRenderer.h"
#include "AssetManager.h"
#include "Timer.h"
#include "RoomScene.h"
#include "Debug.h"

SceneManager* SceneManager::Instance(nullptr);

SceneManager::SceneManager(): 
	currentScene(nullptr), timer(nullptr),
	fps(60), isRunning(false), rendererType(RendererType::VULKAN), 
	renderer(nullptr), assetManager(nullptr) {}

SceneManager* SceneManager::GetInstance(){
	if (!Instance) {
		Instance = new SceneManager();
	}

	return Instance;
}

SceneManager::~SceneManager() {
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (renderer) {
		renderer->OnDestroy();
		delete renderer;
		renderer = nullptr;
	}

	if (assetManager) {
		assetManager->OnDestroy();
		delete assetManager;
		assetManager = nullptr;
	}

	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);

}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	renderer = VulkanRenderer::GetInstance();
	renderer->setRendererType(RendererType::VULKAN);
	renderer->CreateWindow(name_, width_, height_);
	renderer->OnCreate();

	//Create asset manager
	assetManager = AssetManager::GetInstance();
	if (!assetManager->OnCreate()) {
		Debug::Info("AssetManager Failed", __FILE__, __LINE__);
		//return false;
	}
	Debug::Info("AssetManager Created", __FILE__, __LINE__);
	assetManager->LoadAssets("RendererAssets");

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}
	
	BuildScene(ROOMSCENE, "TestScene");
	
	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		GetEvents();
		SDL_Delay(timer->GetSleepTime(fps));	
	}
}

void SceneManager::GetEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q:
				isRunning = false;
				return;

			case SDL_SCANCODE_F1:
				BuildScene(ROOMSCENE, "TestScene");
				break;

			case SDL_SCANCODE_F2:
				BuildScene(ROOMSCENE, "TestScene2");
				break;

			case SDL_SCANCODE_F3:
				///BuildScene(SCENE3);
				break;

			case SDL_SCANCODE_F4:
				///BuildScene(SCENE4);
				break;

			case SDL_SCANCODE_F5:
				///BuildScene(SCENE5);
				break;

			case SDL_SCANCODE_F6:
				///BuildScene(SCENE6);
				break;

			default:
				BuildScene(ROOMSCENE, "TestScene");
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::BuildScene(SCENETYPE scenetype_, const char* fileName) {
	bool status; 

	//Cleanup current scene
	if (currentScene != nullptr) {
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scenetype_) {
	case ROOMSCENE:
		//currentScene = new RoomScene(renderer);
		
		currentScene = new RoomScene(renderer, assetManager->LoadRoom(fileName));
		status = currentScene->OnCreate();
		break;

	case MENUSCENE:

		//status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}	
}


