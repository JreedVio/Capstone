#include "RoomScene.h"
#include <iostream>
#include "Debug.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "Camera.h"
#include "GlobalLighting.h"
#include "SceneManager.h"
#include "CameraActor.h"
#include "TransformComponent.h"
#include "Physics.h"

using namespace PHYSICS;

RoomScene::RoomScene(VulkanRenderer* renderer_):Scene(renderer_) {
    camera = std::make_shared<CameraActor>(nullptr);
}

RoomScene::RoomScene(VulkanRenderer* renderer_, Ref<Room> room_):Scene(renderer_), room(room_){
    camera = std::make_shared<CameraActor>(nullptr);

}

RoomScene::~RoomScene(){
    OnDestroy();
}

bool RoomScene::OnCreate(){

    float aspectRatio = static_cast<float>(renderer->GetWidth()) / static_cast<float>(renderer->GetHeight());


    //Add the players to the scene, and spawn at the desired location
    remotePlayer = SceneManager::GetInstance()->GetRemotePlayer();
    localPlayer = SceneManager::GetInstance()->GetLocalPlayer();
    Ref<Actor> remotePawn = remotePlayer->GetPawn();
    Ref<Actor> localPawn = localPlayer->GetPawn();
    Ref<TransformComponent> remoteTransform_ = remotePawn->GetComponent<TransformComponent>();
    Ref<TransformComponent> localTransform_ = localPawn->GetComponent<TransformComponent>();
    
    //Set the enter location
    Vec3 playerStart = Vec3(0.0f, 1.0f, 0.0f);
    remoteTransform_->SetTransform(playerStart, remoteTransform_->GetOrientation(), remoteTransform_->GetScale());
    localTransform_->SetTransform(playerStart, localTransform_->GetOrientation(), localTransform_->GetScale());
    AddActor("RemotePlayer", remotePlayer->GetPawn());
    AddActor("LocalPlayer", localPlayer->GetPawn());

    //Setup the camera
    camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, -0.8f, 0.0f), Quaternion());
    camera->OnCreate();
    if (localPlayer->GetPawn()->GetComponent<CameraActor>()) {
        localPlayer->GetPawn()->RemoveComponent<CameraActor>();
    }
    camera->SetParent(localPlayer->GetPawn().get());
    localPlayer->GetPawn()->AddComponent(camera);
    
    //** example on how to currently use the collision
    auto floor = GetActor("Bottom");
    //localPlayer->GetPawn()->AddComponent<AABB>(localPlayer.get(), localPlayer->GetPawn()->GetComponent<TransformComponent>(),
    //                                           localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition(), Vec3(1.0f, 1.0f, 1.0f), Quaternion());
    //localPlayer->GetPawn()->AddComponent<DynamicLinearMovement>(nullptr, localPlayer->GetPawn()->GetComponent<TransformComponent>());
    //localPlayer->GetPawn()->AddComponent<Physics>(nullptr);
    
   
    //floor->AddComponent<AABB>(floor.get(), floor->GetComponent<TransformComponent>(), floor->GetComponent<TransformComponent>()->GetPosition(), Vec3(50.0f, 1.0f, 50.0f), Quaternion());
    //**

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

void RoomScene::Update(const float deltaTime) {

    /*TODO:
      Check for collision and update actors, room
      When door collision happens, call RoomTransittion()
    */
    //
    Ref<Actor> localPawn = localPlayer->GetPawn();
    Ref<AABB> localPawnCollision = localPawn->GetComponent<AABB>();
    localPawn->GetComponent<AABB>()->SetCentre(localPawn->GetComponent<TransformComponent>()->GetPosition());
    localPawn->Update(deltaTime);
    room->Update(deltaTime);
    camera->Update(deltaTime);

    for (auto e : GetActorList()) {
        Ref<Actor> actor_ = e.second;
        Ref<AABB> actorCollision = actor_->GetComponent<AABB>();
        if (std::dynamic_pointer_cast<DoorActor>(actor_)) {
            actor_->Update(deltaTime);
            if (!actorCollision) continue;

            if (localPawn->GetComponent<Physics>()->TestTwoAABB(localPawnCollision.get(), actorCollision.get())) {
                actor_->CollisionResponse();

            }

        }

        if (actorCollision) {
            //localPawn->GetComponent<Physics>()->TestTwoAABB(localPawnCollision.get(), actorCollision.get());
        }

    }

    localPawn->GetComponent<Physics>()->TestTwoAABB(localPawnCollision.get(), GetActor("Bottom")->GetComponent<AABB>().get());

    //localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition().print();
}

void RoomScene::Render() const{
    renderer->SetUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
    renderer->SetGLightsUbo(globalLights);
    renderer->Render();
}

void RoomScene::HandleEvents(const SDL_Event& sdlEvent){
    //Ref<TransformComponent> tf = player->GetComponent<TransformComponent>();
    localPlayer->GetPlayerInput(sdlEvent, nullptr);

    //Short cut to open door
    if (sdlEvent.type == SDL_KEYDOWN) {
        if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_O){
            Ref<DoorActor> doorActor = std::dynamic_pointer_cast<DoorActor>(GetActor("Door"));
            doorActor->SetIsOpened(!doorActor->GetIsOpened());
        }
    }


}

Ref<Actor> RoomScene::GetActor(const char* name_){

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
