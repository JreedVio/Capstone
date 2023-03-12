#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "VulkanRenderer.h"
#include "NetworkManager.h"

class PlayerController;
class AssetManager;
class UIManager;
class Scene;

enum SCENETYPE {
	ROOMSCENE = 0,
	MENUSCENE
};

enum USERTYPE {
	SERVER = 0,
	CLIENT
};


class SceneManager  {
public:
	
	static SceneManager* GetInstance();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void GetEvents();
	static void RunNetworkUpdate(NetworkManager*);
	bool GetIsRunning() { return isRunning; }
	Scene* GetCurrentScene() { return currentScene; }
	Ref<PlayerController> GetRemotePlayer() { return remotePlayer; }
	Ref<PlayerController> GetLocalPlayer() { return localPlayer; }
	bool StartGame(USERTYPE userType_);
	void QuitGame();
	void RoomChange(const char* roomName_);
	void MainMenu();
	bool GameOver();
	bool GameWin();
	
private:


	SceneManager();
	static SceneManager* Instance;
	NetworkManager* networkManager;
	enum class RendererType rendererType;
	Scene* currentScene;
	class Timer* timer;

	Ref<PlayerController> localPlayer;
	Ref<PlayerController> remotePlayer;

	VulkanRenderer* renderer;
	AssetManager* assetManager;
	UIManager* uiManager;
	unsigned int fps;
	bool isRunning;
	void BuildScene(SCENETYPE scenetype_, const char* fileName);

	int winCondition = 10;
};


#endif // SCENEMANAGER_H