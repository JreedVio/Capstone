#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Debug.h"
#include <unordered_map>
#include "Vector.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"
#include "VulkanRenderer.h"

template<typename T>
using Ref = std::shared_ptr<T>;

class Scene;

using namespace MATH;

class UserInterface {

protected:

	UserInterface* parent;
	std::unordered_map<const char*, Ref<UserInterface>> UIChildList;
	int width;
	int height;
	VulkanRenderer* renderer;
	bool resized = false;
	bool showWindow;
	float windowAlpha;
	const char* shortcut;
	const char* soundFile;
	bool windowHovered;

	void checkHovered() {
		windowHovered = ImGui::IsWindowHovered();
	}

	void ChildDisplay() {
		for (auto ui_ : UIChildList) {
			if (ui_.second->isOpened()) {
				ui_.second->Display();
			}
		}
	}

public:
	UserInterface(UserInterface* parent_) : parent(parent_), renderer(VulkanRenderer::GetInstance()), showWindow(false), windowAlpha(1.0f),
						height(0), width(0), shortcut(""), soundFile(""), windowHovered(false) {}
	virtual ~UserInterface() = default;
	virtual void Display() = 0;
	virtual void Update(Scene* scene_) = 0;
	void setSize(int width_, int height_) {
		width = width_;
		height = height_;
	}
	void SetParent(UserInterface* parent_) { parent = parent_; }
	void ShowWindow(bool showWindow_) { 
		showWindow = showWindow_; 
		//printf("Showing\n");
	}
	bool isOpened() { 
		if (showWindow) return showWindow;
		for (auto ui_ : UIChildList) {
			if (ui_.second->isOpened()) {
				return true;
			}
		}
		return false;
	}
	void setWindowAlpha(float alpha_) { windowAlpha = alpha_; }
	void setShortcut(const char* shortcut_) { shortcut = shortcut_; }
	const char* getShortcut() { return shortcut; }
	void setSoundFile(const char* soundFile_) { soundFile = soundFile_; }
	const char* getSoundFile() { return soundFile; }
	void Resized() { resized = true; }
	bool isHovered() { return windowHovered; }

	void AddChildUI(const char* uiName_, Ref<UserInterface> ui_) {

		if (GetChildUI(uiName_)) return;

		UIChildList[uiName_] = ui_;
	}

	Ref<UserInterface> GetChildUI(const char* uiName_) {
		for (auto ui_ : UIChildList) {
			if (strcmp(ui_.first, uiName_) == 0) {
				return ui_.second;
			}
		}
		return nullptr;
	}

};

#endif

