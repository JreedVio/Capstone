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
    //auto wallBackward = GetActor("Backward");
    /*localPlayer->GetPawn()->AddComponent<AABB>(localPlayer->GetPawn().get(), localPlayer->GetPawn()->GetComponent<TransformComponent>(),
                                               localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition(),
                                               Vec3(1.0f, 1.0f, 1.0f), Quaternion());


    localPlayer->GetPawn()->AddComponent<DynamicLinearMovement>(localPlayer->GetPawn().get(), localPlayer->GetPawn()->GetComponent<TransformComponent>());
    localPlayer->GetPawn()->AddComponent<Physics>(nullptr);
    localPlayer->GetPawn()->GetComponent<TransformComponent>()->SetTransform(localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition(),
                                                                             localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetOrientation());*/


    floor->AddComponent<AABB>(floor.get(), floor->GetComponent<TransformComponent>(),
        floor->GetComponent<TransformComponent>()->GetPosition(),
        Vec3(50.0f, 1.0f, 50.0f), Quaternion());


    Vec3 wallScale = wallForward->GetComponent<TransformComponent>()->GetScale();
    wallForward->AddComponent<AABB>(wallForward.get(), wallForward->GetComponent<TransformComponent>(),
        wallForward->GetComponent<TransformComponent>()->GetPosition(),
        Vec3(wallScale.x, wallScale.y, wallScale.z),
        wallForward->GetComponent<TransformComponent>()->GetOrientation());
    //**

    // Setup for Pressure plate puzzle
    //**

    //auto plateA3 = GetActor("PlateA3");
    //auto plateB2 = GetActor("PlateB2");

    //if (plateB2 && plateA3)
    //{
    //    plateA3->AddComponent<AABB>(plateA3.get(), plateA3->GetComponent<TransformComponent>(),
    //        plateA3->GetComponent<TransformComponent>()->GetPosition(),
    //        Vec3(plateA3->GetComponent<TransformComponent>()->GetScale().x - 0.5f, 1.0f,
    //            plateA3->GetComponent<TransformComponent>()->GetScale().z - 0.5f));

    //    plateB2->AddComponent<AABB>(plateB2.get(), plateB2->GetComponent<TransformComponent>(),
    //        plateB2->GetComponent<TransformComponent>()->GetPosition(),
    //        Vec3(plateB2->GetComponent<TransformComponent>()->GetScale().x - 0.5f, 1.0f,
    //            plateB2->GetComponent<TransformComponent>()->GetScale().z - 0.5f));
    //}   

    //**
    //Play Background music
    //AudioManager::getInstance()->PlayBGM("audio/ByeByeBrain320bit.wav");
    return false;
}

void RoomScene::OnDestroy() {
    //for (auto actor_ : actorList) {
    //    actor_.second->OnDestroy();
    //}
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

    localPawn->GetComponent<AABB>()->SetCentre(localPawn->GetComponent<TransformComponent>()->GetPosition());
    localPawn->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(0.0f,-9.81f / 1.0f, 0.0f));
    for (auto e : GetActorList()) {
        if (strcmp(e.first, "LocalPlayer") == 0) continue;

        Ref<Actor> actor_ = e.second;
        Ref<AABB> actorCollision = actor_->GetComponent<AABB>();

        if (localPawn->GetComponent<Physics>()->TestTwoAABB(localPawnCollision, actorCollision)) {
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
    

    //localPlayer->GetPawn()->GetComponent<Physics>()->TestTwoAABB(localPlayer->GetPawn()->GetComponent<AABB>(), GetActor("Forward")->GetComponent<AABB>());
    remotePlayer->GetPawn()->GetComponent<AABB>()->SetCentre(remotePlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition());
    //remotePlayer->GetPawn()->GetComponent<AABB>()->GetCentre().print();
/*    auto plate2 = GetActor("Plate2");
    auto plate3 = GetActor("Plate3");
    if (plate2 && plate3)
    remotePlayer->GetPawn()->GetComponent<AABB>()->SetCentre(remotePlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition());    
  */  
    //auto plateA3 = GetActor("PlateA3");
    //auto plateB2 = GetActor("PlateB2");

    //auto localPlayerPhysics = localPlayer->GetPawn()->GetComponent<Physics>();
    //auto remotePlayerPhysics = remotePlayer->GetPawn()->GetComponent<Physics>();
    //if (plateB2 && plateA3)
    //{
    //    bool status1 = localPlayerPhysics->TestTwoAABB(localPlayer->GetPawn()->GetComponent<AABB>(), plateB2->GetComponent<AABB>());
    //    bool status2 = remotePlayerPhysics->TestTwoAABB(remotePlayer->GetPawn()->GetComponent<AABB>(), plateB2->GetComponent<AABB>());
    //    
    //    bool status3 = localPlayerPhysics->TestTwoAABB(localPlayer->GetPawn()->GetComponent<AABB>(), plateA3->GetComponent<AABB>());
    //    bool status4 = remotePlayerPhysics->TestTwoAABB(remotePlayer->GetPawn()->GetComponent<AABB>(), plateA3->GetComponent<AABB>());

    //    // change alpha
    //    if (status1 || status2)
    //    {
    //        plateB2->SetAlpha(1.0f);            
    //    }
    //    else if (status3 || status4)
    //    {
    //        plateA3->SetAlpha(1.0f);
    //    }
    //    else
    //    {
    //        plateB2->SetAlpha(0.5f);
    //        plateA3->SetAlpha(0.5f);
    //    }

    //    // open door
    //    if ((status1 || status2) && (status3 || status4))
    //    {
    //        localPlayer->GetPawn()->GetComponent<Physics>()->UpdatePuzzle(deltaTime);
    //    }
    //}
    //plate3->GetComponent<AABB>()->GetCentre().print();
    // needs to be the last test called or it will not update properly
    //localPlayer->GetPawn()->GetComponent<Physics>()->TestTwoAABB(localPlayer->GetPawn()->GetComponent<AABB>(), GetActor("Bottom")->GetComponent<AABB>());   
       
    localPlayer->GetPawn()->Update(deltaTime);

    //localPlayer->GetPawn()->GetComponent<TransformComponent>()->GetPosition().print();
    
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


