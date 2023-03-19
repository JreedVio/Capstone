#ifndef CODEPUZZLEROOM_H
#define CODEPUZZLEROOM_H

#include <memory>
#include "Room.h"

class UserInterface;

class CodePuzzleRoom : public Room{

private:

	UserInterface* codeUI;
	const char* passCode;

public:
	CodePuzzleRoom(int width_, int length_, int height_): Room(width_, length_, height_), passCode("1234") {}
	~CodePuzzleRoom();

	virtual bool OnCreate() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy() override;

	void SetCode(const char* code_) { passCode = code_; }


};


#endif
