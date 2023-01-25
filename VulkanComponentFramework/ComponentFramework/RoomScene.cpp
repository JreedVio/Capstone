#include "RoomScene.h"
#include <iostream>
#include "Debug.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "Camera.h"

RoomScene::RoomScene(VulkanRenderer* renderer_):Scene(renderer_) {
    camera = std::make_shared<Camera>();
}

RoomScene::RoomScene(VulkanRenderer* renderer_, Ref<Room> room_):Scene(renderer_), room(room_){
    camera = std::make_shared<Camera>();

}

RoomScene::~RoomScene(){
    OnDestroy();
}

bool RoomScene::OnCreate(){

    float aspectRatio = static_cast<float>(renderer->GetWidth()) / static_cast<float>(renderer->GetHeight());
    camera->Perspective(45.0f, aspectRatio, 0.5f, 20.0f);
    camera->LookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, -3.0f), Vec3(0.0f, 1.0f, 0.0f));
    globalLights.position[0] = Vec4(5.0f, 0.0f, -3.0f, 0.0f);
    globalLights.position[1] = Vec4(-5.0f, 0.0f, -3.0f, 0.0f);
    globalLights.diffuse = Vec4(0.0f, 0.1f, 0.0f, 1.0f);

    return false;
}

void RoomScene::OnDestroy(){
    //for (auto actor_ : actorList) {
    //    actor_.second->OnDestroy();
    //}
    room->OnDestroy();
}

void RoomScene::Update(const float deltaTime){

}

void RoomScene::Render() const{
    renderer->SetUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
    renderer->SetGLightsUbo(globalLights);
    //renderer->SetPushConst(mariosModelMatrix);
    renderer->Render();
}

void RoomScene::HandleEvents(const SDL_Event& sdlEvent)
{
}
