#include "RoomScene.h"
#include <iostream>
#include "Debug.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "Camera.h"
#include "GlobalLighting.h"

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
    
    //globalLights.push_back(std::make_shared<LightActor>(nullptr));
    //globalLights.push_back(std::make_shared<LightActor>(nullptr));

    //globalLights[0]->SetPosition(Vec4(5.0f, 0.0f, -3.0f, 0.0f));
    //globalLights[0]->SetDiffuse(Vec4(0.0f, 0.1f, 0.0f, 1.0f));
    //globalLights[1]->SetPosition(Vec4(-5.0f, 0.0f, -3.0f, 0.0f));
    //globalLights[0]->SetDiffuse(Vec4(0.6f, 0.0f, 0.0f, 1.0f));
    //globalLights.diffuse = Vec4(0.0f, 0.1f, 0.0f, 1.0f);

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

Ref<Actor> RoomScene::GetActor(const char* name_)
{
    for (auto actor_ : GetActorList()) {
        if (strcmp(actor_.first, name_) == 0) {
            return actor_.second;
        }
    }
    //Debug message when fail
    std::string message = std::string(name_) + " -> Actor not found in AssetManager";
    Debug::FatalError(message, __FILE__, __LINE__);
    return nullptr;
}
