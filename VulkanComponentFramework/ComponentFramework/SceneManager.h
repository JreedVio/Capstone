#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "VulkanRenderer.h"
#include "NetworkManager.h"

class Actor;
class AssetManager;
class Scene;

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
	
	
private:
	
	enum SCENETYPE {
		ROOMSCENE = 0,
		MENUSCENE
	};

	SceneManager();
	static SceneManager* Instance;

	NetworkManager* networkManager;

	enum class RendererType rendererType;
	Scene* currentScene;
	class Timer* timer;

	Ref<Actor> localPlayer;
	Ref<Actor> remotePlayer;

	VulkanRenderer* renderer;
	AssetManager* assetManager;
	unsigned int fps;
	bool isRunning;
	void BuildScene(SCENETYPE scenetype_, const char* fileName);
};


#endif // SCENEMANAGER_H