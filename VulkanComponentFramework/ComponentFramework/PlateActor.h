#pragma once
#include "Actor.h"
class PlateActor : public Actor
{
private:
	bool isEnabled;

public:

	PlateActor(Component* parent_);
	~PlateActor();
	bool GetIsEnabled() { return isEnabled; }
	void SetIsEnabled(bool isEnabled_) { isEnabled = isEnabled_; }
};

