#ifndef CODEUI_H
#define CODEUI_H

#include "UserInterface.h"


class CodeUI : public UserInterface{

private:

	const char* entered;

public:
	CodeUI(UserInterface* parent_);
	~CodeUI();
	void Display();
	void Update(Scene* scene_);

};

#endif

