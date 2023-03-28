#include "CodeActor.h"
#include "UIManager.h"

CodeActor::CodeActor(Component* parent_) : Actor(parent_), panelOpened(false) {
	codeUI = dynamic_cast<CodeUI*>(UIManager::getInstance()->GetUI("CodeUI"));
}


CodeActor::~CodeActor(){
	
}

void CodeActor::Update(float deltaTime_){

}

void CodeActor::CollisionResponse(){
	//Open CodeUI when colliding
	codeUI->ShowWindow(true);

}

void CodeActor::NotCollided(){
	codeUI->ShowWindow(false);
}
