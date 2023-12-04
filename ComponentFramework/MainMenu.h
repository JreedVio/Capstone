#ifndef MAINMENU_H
#define MAINMENU_H

#include "UserInterface.h"
#include <string>


using namespace MATH;

class MainMenu : public UserInterface{

private:

	void showAudioControl();
	void showMainMenu();

public:
	MainMenu(UserInterface* parent_);
	~MainMenu();
	void Display();
	void Update(Scene* scene_);

};

#endif
