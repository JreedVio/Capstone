#ifndef CODEPUZZLEROOM_H
#define CODEPUZZLEROOM_H

#include <memory>
#include "Room.h"

#define CODE_SIZE 4

class UserInterface;
class CodeActor;

class CodePuzzleRoom : public Room{

private:

	Ref<CodeActor> codePanel;
	std::vector<int> passCode;

public:
	CodePuzzleRoom(int width_, int length_, int height_);
	~CodePuzzleRoom();

	virtual bool OnCreate() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy() override;

	virtual void CheckPuzzle() override;
	void SetCode(const char* code_);
	

};


#endif
