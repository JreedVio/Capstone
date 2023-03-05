#include <SDL.h>
#include "SceneManager.h"
#include "VulkanRenderer.h"
#include "OpenGLRenderer.h"
#include "AssetManager.h"
#include "Timer.h"
#include "RoomScene.h"
#include "Debug.h"
#include "ChronoTimer.h"
#include <future>
#include <thread>
#include "PlayerController.h"


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

	if (networkManager) {
		//networkManager->OnDestroy();
		delete networkManager;
		networkManager = nullptr;
	}


	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);

}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	renderer = VulkanRenderer::GetInstance();
	renderer->setRendererType(RendererType::VULKAN);
	renderer->CreateSDLWindow(name_, width_, height_);
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

	//Create Players
	localPlayer = std::make_shared<PlayerController>(nullptr);
	Ref<Actor> localActor = assetManager->GetActor("LocalPlayer");
	localActor->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion());
	localActor->OnCreate();
	localPlayer->SetPawn(localActor);
	localPlayer->GetPawn()->SetVisible(false);

	remotePlayer = std::make_shared<PlayerController>(nullptr);
	Ref<Actor> remoteActor = assetManager->GetActor("RemotePlayer");
	remoteActor->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion());
	remoteActor->OnCreate();
	remotePlayer->SetPawn(remoteActor);
	remotePlayer->GetPawn()->SetVisible(false);

	BuildScene(ROOMSCENE, "TestScene");

	networkManager = new NetworkManager();
	if (!networkManager->OnCreate()) {
		Debug::FatalError("Failed to initialize Network Manager", __FILE__, __LINE__);
		return false;
	}

	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	
	std::thread networking(&NetworkManager::Update, this->networkManager);
	networking.detach();


	while (isRunning) {
		{
			//ChronoTimer chronoTimer;

			timer->UpdateFrameTicks();
			currentScene->Update(timer->GetDeltaTime());
			currentScene->Render();

			//networkManager->Update();
			//std::async(std::launch::async, RunNetworkUpdate, networkManager);

			GetEvents();
		}
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::RunNetworkUpdate(NetworkManager* networkManager_) {
	
	networkManager_->Update();
}


void SceneManager::RoomChange(const char* roomName_){
	//TODO: TEST
	//When room change occurs, check the winning condition
	if (GameWin()) return;
	//Change room
	BuildScene(ROOMSCENE, roomName_);
}

bool SceneManager::GameOver(){

	//TODO: Change to the lose screen
	return true;
}

bool SceneManager::GameWin(){

	if (localPlayer->GetSurvivedRoom() >= winCondition) {
		//TODO: Change to the winning screen
	}
	return true;
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
				//BuildScene(ROOMSCENE, "TestScene");
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
		
		currentScene = assetManager->LoadRoom(fileName);
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




