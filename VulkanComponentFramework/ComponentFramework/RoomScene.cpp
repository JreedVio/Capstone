#include "RoomScene.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Debug.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "UIManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "GlobalLighting.h"
#include "CameraActor.h"
#include "TransformComponent.h"
#include "Physics.h"
#include "CodeActor.h"


using namespace PHYSICS;

RoomScene::RoomScene(VulkanRenderer* renderer_):Scene(renderer_) {
    camera = std::make_shared<CameraActor>(nullptr);
}

RoomScene::RoomScene(VulkanRenderer* renderer_, Ref<Room> room_):Scene(renderer_), room(room_) {
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
    remotePawn->OnCreate();
    Ref<Actor> localPawn = localPlayer->GetPawn();
    localPawn->OnCreate();
    Ref<TransformComponent> remoteTransform_ = remotePawn->GetComponent<TransformComponent>();
    Ref<TransformComponent> localTransform_ = localPawn->GetComponent<TransformComponent>();

    //Set the enter location
    Vec3 playerStart = Vec3(0.0f, 3.0f, 0.0f);

    remoteTransform_->SetTransform(Vec3(-1.0, -0.5f, 0.0f), QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)), remoteTransform_->GetScale());
    remoteTransform_->SetTransform(playerStart, QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)), remoteTransform_->GetScale());

    localTransform_->SetTransform(playerStart, QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)), localTransform_->GetScale());
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

    //Oncreate the room
    room->OnCreate();

    //** example on how to currently use the collision
    auto floor = GetActor("Bottom");
    auto wallForward = GetActor("Forward");
 
    floor->AddComponent<AABB>(floor.get(), floor->GetComponent<TransformComponent>(),
        floor->GetComponent<TransformComponent>()->GetPosition(),
        Vec3(50.0f, 0.1f, 50.0f), Quaternion());

    Vec3 wallScale = wallForward->GetComponent<TransformComponent>()->GetScale();
    wallForward->AddComponent<AABB>(wallForward.get(), wallForward->GetComponent<TransformComponent>(),
        wallForward->GetComponent<TransformComponent>()->GetPosition(),
        Vec3(wallScale.x, wallScale.y, wallScale.z),
        wallForward->GetComponent<TransformComponent>()->GetOrientation());

    //Play Background music
    AudioManager::getInstance()->PlayBGM("audio/ByeByeBrain320bit.wav");

    return false;
}

void RoomScene::OnDestroy() {
    room->OnDestroy();
}

void RoomScene::Update(const float deltaTime) {

    if (room->IsSolved() && !room->GetDoor()->GetIsOpened()) {
        room->OpenDoor();
        AudioManager::getInstance()->PlaySoundEffects("audio/gameLevelComplete.wav");
        NetworkManager::GetInstance()->GetUnit()->SendPuzzleSolved();
    }
    else if(!room->IsSolved()) {
        room->CheckPuzzle();
    }

    Ref<Actor> localPawn = localPlayer->GetPawn();
    Ref<AABB> localPawnCollision = localPawn->GetComponent<AABB>();    

    room->Update(deltaTime);
    camera->Update(deltaTime);

    localPawn->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(0.0f,-9.81f / 1.0f, 0.0f));   
    
    for (auto e : GetActorList()) {
        if (strcmp(e.first, "LocalPlayer") == 0) continue;

        Ref<Actor> actor_ = e.second;
        Ref<AABB> actorCollision = actor_->GetComponent<AABB>();        

        if (Physics::TestTwoAABB(localPawnCollision, actorCollision)) {
            actor_->CollisionResponse();

            if (strcmp(e.first, "Bottom") == 0)
            {
                Vec3 tempVel = localPawn->GetComponent<DynamicLinearMovement>()->GetVel();
                Vec3 tempAccel = localPawn->GetComponent<DynamicLinearMovement>()->GetAccel();
                localPawn->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(tempAccel.x, 0.0f, tempAccel.z));
                localPawn->GetComponent<DynamicLinearMovement>()->SetVel(Vec3(tempVel.x , 0.0f, tempVel.z));
             }
        }
        else {
            actor_->NotCollided();
        }
                
        actor_->Update(deltaTime);

    }    

    remotePlayer->GetPawn()->GetComponent<AABB>()->SetCentre(remotePlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition()); 
    localPawn->Update(deltaTime);
    
}

void RoomScene::Render() const{
    renderer->SetUBO(camera->GetProjectionMatrix(), camera->GetViewMatrix());
    renderer->SetGLightsUbo(globalLights);
    renderer->Render();
}

void RoomScene::HandleEvents(const SDL_Event& sdlEvent) {
    //Ref<TransformComponent> tf = player->GetComponent<TransformComponent>();
    localPlayer->GetPlayerInput(sdlEvent);   

    //Short cut to open door
    if (sdlEvent.type == SDL_KEYDOWN) {
        if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_O) {
            room->SetSolved(true);
        }
        else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_L) {
            UIManager::getInstance()->openMenu("CodeUI");

        }
    }
}



Ref<Actor> RoomScene::GetActor(const char* name_){

    return room->GetActor(name_);
}


