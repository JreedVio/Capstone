#include "RoomScene.h"
#include "Room.h"

RoomScene::RoomScene(Renderer* renderer_, Ref<Room> room_):Scene(renderer), room(room_){
}

RoomScene::~RoomScene(){
    OnDestroy();
}

bool RoomScene::OnCreate(){
    return false;
}

void RoomScene::OnDestroy(){
    for (auto actor_ : actorList) {
        actor_.second->OnDestroy();
    }
}

void RoomScene::Update(const float deltaTime){

}

void RoomScene::Render() const{

}

void RoomScene::HandleEvents(const SDL_Event& sdlEvent)
{
}
