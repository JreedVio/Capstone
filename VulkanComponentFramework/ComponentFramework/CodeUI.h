#ifndef CODEUI_H
#define CODEUI_H

#include "UserInterface.h"
#include <vector>

#define CODE_LENGTH 4

class CodeUI : public UserInterface{

private:

	std::vector<int> entered;

public:
	CodeUI(UserInterface* parent_);
	~CodeUI();
	void Display();
	void Update(Scene* scene_);
	std::vector<int> GetEntered() { return entered; }

};

#endif

