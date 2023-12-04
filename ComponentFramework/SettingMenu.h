#ifndef SETTINGMENU_H
#define SETTINGMENU_H

#include "UserInterface.h"
#include <string>
#include "Vector.h"
#include "VMath.h"
#include "Quaternion.h"
#include "QMath.h"
#include "Actor.h"

using namespace MATH;


class SettingMenu : public UserInterface{

private:

	void showAudioControl();
	void toParent();

public:
	SettingMenu(UserInterface* parent_) : UserInterface(parent_) {}
	~SettingMenu();
	void Display();
	void Update(Scene* scene_);

};

#endif
