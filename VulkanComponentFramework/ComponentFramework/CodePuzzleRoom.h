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
	void ParseCode(int tempCode);

public:
	CodePuzzleRoom(int width_, int length_, int height_, float passCode_ = 0000);
	~CodePuzzleRoom();

	virtual bool OnCreate() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy() override;
	virtual void CheckPuzzle() override;
	virtual void Reset() override;

	void SetCode(const char* code_);
	void SetCode(float code_);

};


#endif
