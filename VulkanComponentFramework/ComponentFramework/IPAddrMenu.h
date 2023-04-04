#pragma once

#include "UserInterface.h"
#include <string>

#define IP_LENGTH 15

class IPAddrMenu : public UserInterface
{

	void showEnterIPAddr();
	std::string entered;

public:
	IPAddrMenu(UserInterface* parent_) : UserInterface(parent_) {}
	~IPAddrMenu();
	void Display();
	void Update(Scene* scene_);

	std::string GetEntered() const { return entered; }
	

	void toParent();

};

