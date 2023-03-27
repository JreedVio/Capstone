#ifndef CODEACTOR_H
#define CODEACTOR_H

#include "Actor.h"
#include "CodeUI.h"

class UserInterface;


class CodeActor : public Actor{

private:
	bool panelOpened;
	std::vector<int> codeEntered;
	CodeUI* codeUI;

public:

	CodeActor(Component* parent_);
	~CodeActor();
	void Update(float deltaTime_) override;
	void CollisionResponse() override;
	void NotCollided() override;
	std::vector<int> GetCodeEntered() { return codeUI->GetEntered(); }
};


#endif

