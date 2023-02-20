#ifndef DOORACTOR_H
#define DOORACTOR_H

#include "Actor.h"


class DoorActor: public Actor{

	DoorActor(const DoorActor&) = delete; 
	DoorActor(DoorActor&&) = delete;
	Actor& operator=(const DoorActor&) = delete;
	DoorActor& operator=(DoorActor&&) = delete;

private:
	const char* connectedRoom;
	bool isOpened;

public:
	DoorActor(Component* parent_): Actor(parent_), isOpened(false) {}
	~DoorActor(){}
	void SetConnection(const char* connectedRoom_) { connectedRoom = connectedRoom_; }
	const char* GetConnection() { return connectedRoom; }
	//Move the local player to the connected room
	void RoomTransition();
	void SetIsOpened(bool isOpened_) { isOpened = isOpened_; }
	bool GetIsOpened() { return isOpened; }
};

#endif 

