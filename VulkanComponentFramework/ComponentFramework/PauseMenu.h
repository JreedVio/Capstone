#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "UserInterface.h"
#include <string>


using namespace MATH;

class PauseMenu : public UserInterface {

private:

	void showAudioControl();
	void showPauseMenu();

public:
	PauseMenu(UserInterface* parent_);
	~PauseMenu();
	void Display();
	void Update(Scene* scene_);

};



#endif