#ifndef WINMENU_H
#define WINMENU_H

#include "UserInterface.h"
#include <string>


using namespace MATH;

class WinMenu : public UserInterface{

private:

	void showWinMenu();

public:
	WinMenu(UserInterface* parent_);
	~WinMenu();
	void Display();
	void Update(Scene* scene_);

};

#endif
