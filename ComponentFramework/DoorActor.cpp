#include "DoorActor.h"
#include "SceneManager.h"
#include "PlayerController.h"
#include "Scene.h"


void DoorActor::Update(const float deltaTime_){
	Actor::Update(deltaTime_);
	if (isOpened) {
		SetAlpha(1.0f);
	}
	else {
		SetAlpha(0.5f);
	}
}

bool DoorActor::RoomTransition(){

	//Ask SceneManager to do the room transtition
	//sceneManger->RoomChange(connectedRoom);
	return true;
}

void DoorActor::CollisionResponse(){
	//TODO: TEST
	SceneManager* sceneManger = SceneManager::GetInstance();

	//If the door is not opened, cannot transit rooms
	if (!isOpened) return;

	//When this function is triggered, it means that the player survived a room
	sceneManger->GetLocalPlayer()->AddSurvivedRoom(1);
	sceneManger->RoomChange(connectedRoom);

}
