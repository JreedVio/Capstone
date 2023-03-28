#include "CodePuzzleRoom.h"
#include "CodeActor.h"
#include "UIManager.h"


CodePuzzleRoom::CodePuzzleRoom(int width_, int length_, int height_): Room(width_, length_, height_) {
	SetCode("1234");

}

CodePuzzleRoom::~CodePuzzleRoom(){

}

bool CodePuzzleRoom::OnCreate(){

	codePanel = std::dynamic_pointer_cast<CodeActor>(GetActor("CodePanel"));

	return true;
}

void CodePuzzleRoom::Update(float deltaTime){
	
}

void CodePuzzleRoom::OnDestroy(){
}

void CodePuzzleRoom::CheckPuzzle(){

	std::vector<int> codeEntered = codePanel->GetCodeEntered();
	if (codeEntered.size() != CODE_SIZE) return;
	std::cout << "Check Puzzled\n";
	for (int i = 0; i < CODE_SIZE; i++) {
		int code_ = codeEntered.at(i);
		int passCode_ = passCode.at(i);
		printf("%d, %d\n", code_, passCode_);
		if (code_ != passCode_) {
			return;
		}
	}
	puzzleSolved = true;
}

void CodePuzzleRoom::SetCode(const char* code_){
	//TODO::Set code
	int tempCode = atoi(code_);
	passCode.push_back(tempCode / 1000.0f);
	tempCode = tempCode % 1000;
	passCode.push_back(tempCode / 100.0f);
	tempCode = tempCode % 100;
	passCode.push_back(tempCode / 10.0f);
	tempCode = tempCode % 10;
	passCode.push_back(tempCode);
}

