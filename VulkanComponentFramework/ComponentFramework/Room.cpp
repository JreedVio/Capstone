#include "Room.h"


Room::Room(int width_, int length_, int height_): width(width_), length(length_), height(height_),
                                                  roomTime(10.0f) {
    currentTime = roomTime;
}

Room::~Room()
{
}

void Room::Update(float deltaTime){

    //When reaches 0, call the time over function.
    if (currentTime <= 0.0f) {
        currentTime = 0.0f;
        TimeOver();
        return;
    }

    //Update timer for the room
    currentTime -= deltaTime;
    //printf("Current Time: %f\n", currentTime);

}

void Room::OnDestroy(){
    for (auto actor_ : actorList) {
        actor_.second->OnDestroy();
    }
}

void Room::SetRoomTime(float roomTime_){
    roomTime = roomTime_;
    currentTime = roomTime;
}


void Room::TimeOver(){

    //TODO: Do something when time over

    //printf("TimeOver\n");
}