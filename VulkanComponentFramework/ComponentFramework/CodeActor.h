#ifndef CODEACTOR_H
#define CODEACTOR_H

#include "Actor.h"

class UserInterface;

class CodeActor : public Actor{

private:

public:

	CodeActor(Component* parent_) : Actor(parent_) {}
	~CodeActor();
	void CollisionResponse() override;
};


#endif

