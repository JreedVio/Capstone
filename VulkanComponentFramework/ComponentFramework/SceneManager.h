#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "VulkanRenderer.h"

class AssetManager;
class Scene;

class SceneManager  {
public:
	
	static SceneManager* GetInstance();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void GetEvents();
	Scene* GetCurrentScene() { return currentScene; }
	
	
private:
	
	enum SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6
	};

	SceneManager();
	static SceneManager* Instance;

	enum class RendererType rendererType;
	Scene* currentScene;
	class Timer* timer;

	VulkanRenderer* renderer;
	AssetManager* assetManager;
	unsigned int fps;
	bool isRunning;
	void BuildScene(SCENE_NUMBER scene_);
};


#endif // SCENEMANAGER_H