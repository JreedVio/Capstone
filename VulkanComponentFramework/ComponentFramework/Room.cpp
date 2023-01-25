#include "Room.h"

Room::Room(int width_, int length_, int height_): width(width_), length(length_), height(height_){
}

Room::~Room()
{
}

void Room::OnDestroy(){
    for (auto actor_ : actorList) {
        actor_.second->OnDestroy();
    }
}
