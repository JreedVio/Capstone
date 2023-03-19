#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <unordered_map>
#include "UserInterface.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"

class VulkanRenderer;
class Scene;

class UIManager{

protected:
	static UIManager* instance_;
	UIManager();
	int width;
	int height;
	VulkanRenderer* renderer;
	std::unordered_map<const char*, UserInterface*> ui_list;
	const char* currentUI;
	ImGuiStyle* style;
	bool menuHovered;


public:

	UserInterface* GetUI(const char*);
	VkDescriptorPool descriptorPool;

	~UIManager();
	static UIManager* getInstance();
	void Display();
	bool OnCreate();
	void OnDestroy();
	void Update(Scene* scene_);
	void setRenderer(VulkanRenderer* renderer_) { renderer = renderer_; }
	void setSize(int width_, int height_);
	void AddUserInterface(const char*, UserInterface*);
	void CleanUserInterface();
	void Resized() { ui_list[currentUI]->Resized(); }
	bool isValid();
	//UserInterface* getCurrentUI();
	void defaultTheme();
	void whiteTheme();
	//void greenTheme();
	bool isOpened(const char* menuName_);
	void openMenu(const char* menuName_);
	bool isHovered();
};

#endif

