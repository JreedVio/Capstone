#include <SDL.h>
#include <future>
#include <thread>
#include "SceneManager.h"
#include "VulkanRenderer.h"
#include "OpenGLRenderer.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "UIManager.h"
#include "Timer.h"
#include "RoomScene.h"
#include "MenuScene.h"
#include "Debug.h"
#include "ChronoTimer.h"

#include "PlayerController.h"


SceneManager* SceneManager::Instance(nullptr);

SceneManager::SceneManager(): 
	currentScene(nullptr), timer(nullptr),
	fps(60), isRunning(false), rendererType(RendererType::VULKAN), 
	renderer(nullptr), assetManager(nullptr), audioManager(nullptr), uiManager(nullptr),
	openMainMenu(false){}

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


	if (uiManager) {
		delete uiManager;
		uiManager = nullptr;
	}

	if (renderer) {
		renderer->OnDestroy();
		delete renderer;
		renderer = nullptr;
	}

	if (audioManager) {
		delete audioManager;
		audioManager = nullptr;
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

	if (nextScene) {
		delete[] nextScene;
		nextScene = nullptr;
	}

	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);

}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	//Setup renderer
	renderer = VulkanRenderer::GetInstance();
	renderer->setRendererType(RendererType::VULKAN);
	renderer->CreateSDLWindow(name_, width_, height_);
	renderer->OnCreate();

	//Setup UI Manager
	uiManager = UIManager::getInstance();
	uiManager->setRenderer(renderer);
	uiManager->setSize(width_, height_);
	if (!uiManager->OnCreate()) {
		return false;
		Debug::FatalError("UIManager Failed", __FILE__, __LINE__);
	}

	//Create asset manager
	assetManager = AssetManager::GetInstance();
	if (!assetManager->OnCreate()) {
		Debug::Info("AssetManager Failed", __FILE__, __LINE__);
		//return false;
	}
	Debug::Info("AssetManager Created", __FILE__, __LINE__);
	assetManager->LoadAssets("RendererAssets");

	//Setup Audio Manager
	audioManager = AudioManager::getInstance();
	if (!audioManager->OnCreate()) {
		return false;
		Debug::FatalError("AudioManager Failed", __FILE__, __LINE__);
	}
	Debug::Info("AudioManager Created", __FILE__, __LINE__);

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	networkManager = NetworkManager::GetInstance();
	if (!networkManager->Initialize()) {
		Debug::FatalError("Failed to initialize Network Manager", __FILE__, __LINE__);
		return false;
	}

	//StartGame(CLIENT);

	//Open Main Menu
	MainMenu();
	//Enter the start room
	//BuildScene(ROOMSCENE, "TestScene");
	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	
	while (isRunning) {
	{
			//ChronoTimer chronoTimer;
			timer->UpdateFrameTicks();
			//if(nextScene != nullptr) std::cout << "Current nextScene is: " << nextScene << std::endl;
			if (currentScene->GetStatus() == ROOMTRANSIT) {
				//If wins, enter win screen instead
				if (strcmp(nextScene, "Win") == 0) {
					BuildScene(MENUSCENE, "WinMenu");
				}
				else {
					BuildScene(ROOMSCENE, nextScene);
				}
			}
			if (openMainMenu && !dynamic_cast<MenuScene*>(currentScene)) {
				MainMenu();
				openMainMenu = false;
			}
			if(openMainMenu) openMainMenu = false;

			currentScene->Update(timer->GetDeltaTime());
			uiManager->Update(currentScene);
			uiManager->Display();
			audioManager->Update();
			currentScene->Render();

			GetEvents();
		}

		SDL_Delay(timer->GetSleepTime(fps));
	}
}


bool SceneManager::StartServer(){
	//Set usertype
	if (!networkManager->StartServer()) {
		Debug::FatalError("Failed to start Network Manager as a Server", __FILE__, __LINE__);
		networkManager->ResetNetwork();
		return false;
	}
	SetNextScene("Level1");
			

	uiManager->openMenu("MainMenu");
	//Enter the start room
	BuildScene(ROOMSCENE, nextScene);

	std::thread networking(&NetworkManager::Update, this->networkManager);
	networking.detach();

	//Restart the timer
	//timer->Start();

	return true;
}

bool SceneManager::StartClient(const char* address) {
	if (!networkManager->StartClient(address)) {
		Debug::FatalError("Failed to start Network Manager as a Client", __FILE__, __LINE__);
			networkManager->ResetNetwork();
			return false;
	}

	uiManager->openMenu("MainMenu");
		//Enter the start room
		BuildScene(ROOMSCENE, nextScene);

		std::thread networking(&NetworkManager::Update, this->networkManager);
	networking.detach();

	//Restart the timer
	//timer->Start();

	return true;
}


void SceneManager::RoomChange(const char* roomName_) {

	//Change room
	networkManager->GetUnit()->SendRoomName(roomName_);
	currentScene->SetStatus(ROOMTRANSIT);
	SetNextScene(roomName_);
}

void SceneManager::MainMenu() {

	if(networkManager != nullptr)
		networkManager->ResetNetwork();

	audioManager->PauseBGM(true);

	uiManager->GetUI("PauseMenu")->GetChildUI("SettingMenu")->ShowWindow(false);
	uiManager->GetUI("PauseMenu")->ShowWindow(false);

	uiManager->GetUI("WinMenu")->ShowWindow(false);

	BuildScene(MENUSCENE, "MainMenu");
}

void SceneManager::QuitGame(){
	isRunning = false;
}


bool SceneManager::GameOver(){

	//TODO: Change to the lose screen
	return true;
}

bool SceneManager::GameWin(){

	if (localPlayer->GetSurvivedRoom() >= winCondition) {
		//TODO: Change to the winning screen
		return true;
	}
	return false;
}

void SceneManager::CreatePlayers()
{
	//Create Players
	localPlayer = std::make_shared<PlayerController>(nullptr, "LocalPlayer");
	localPlayer->OnCreate();

	remotePlayer = std::make_shared<PlayerController>(nullptr, "RemotePlayer");
	remotePlayer->OnCreate();
}

void SceneManager::GetEvents() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_Q:
				//isRunning = false;
				return;

			case SDL_SCANCODE_F1:
				BuildScene(ROOMSCENE, "Level1");
				break;

			case SDL_SCANCODE_F2:
				BuildScene(ROOMSCENE, "Level2");
				break;

			case SDL_SCANCODE_F3:
				BuildScene(ROOMSCENE, "Level3");
				break;

			case SDL_SCANCODE_F4:
				BuildScene(ROOMSCENE, "Level4");
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

void SceneManager::SetNextScene(const char* nextScene_)
{
	if (nextScene != nullptr) delete[] nextScene;

	size_t size = strlen(nextScene_);
	nextScene = new char[size + 1];
	std::memcpy(nextScene, nextScene_, size);
	nextScene[strlen(nextScene_)] = '\0';
}

void SceneManager::BuildScene(SCENETYPE scenetype_, const char* fileName) {
	bool status; 
	std::string sceneName_ = std::string(fileName);

	//Cleanup current scene
	if (currentScene != nullptr) {
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scenetype_) {
	case ROOMSCENE:

		currentScene = assetManager->LoadRoom(fileName);
		status = currentScene->OnCreate();
		break;

	case MENUSCENE:

		uiManager->openMenu(fileName);
		currentScene = new MenuScene(renderer);
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}	
}




