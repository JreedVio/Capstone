#include "PhysicsPuzzleRoom.h"
#include "UIManager.h"
#include "AudioManager.h"
#include "Physics.h"
#include "SceneManager.h"
#include "PlayerController.h"

using namespace PHYSICS;

PhysicsPuzzleRoom::PhysicsPuzzleRoom(int width_, int length_, int height_): Room(width_, length_, height_)
{
    
}

PhysicsPuzzleRoom::~PhysicsPuzzleRoom()
{

}

bool PhysicsPuzzleRoom::OnCreate()
{
    if (GetActorList().empty()) return false;

    plate1 = GetActor("Plate1");
    plate2 = GetActor("Plate2");

    Cube1 = GetActor("Cube1");
    Cube2 = GetActor("Cube2");
    Cubes.push_back(Cube1);
    Cubes.push_back(Cube2);
    
    // add components to each of the crates
    if (!Cubes.empty())
    {
        for (auto& obj : Cubes)
        {
            if (!obj)
            {
                continue;
            }
            obj->AddComponent<DynamicLinearMovement>(nullptr, obj->GetComponent<TransformComponent>());

            obj->AddComponent<AABB>(obj.get(), obj->GetComponent<TransformComponent>(),
                obj->GetComponent<TransformComponent>()->GetPosition(),
                obj->GetComponent<TransformComponent>()->GetScale() + Vec3(0.5f, 0.6f, 0.5f), //Vec3(0.5f, 0.35f, 0.5f)
                obj->GetComponent<TransformComponent>()->GetOrientation());
        }
    }

    // set up Bound box components for the plates
    if (plate1 && plate2)
    {
        plate1->AddComponent<AABB>(plate1.get(), plate1->GetComponent<TransformComponent>(),
                                   plate1->GetComponent<TransformComponent>()->GetPosition(),
                              Vec3(plate1->GetComponent<TransformComponent>()->GetScale().x - 0.5f, 1.0f,
                                   plate1->GetComponent<TransformComponent>()->GetScale().z - 0.5f));

        plate2->AddComponent<AABB>(plate2.get(), plate2->GetComponent<TransformComponent>(),
                                   plate2->GetComponent<TransformComponent>()->GetPosition(),
                              Vec3(plate2->GetComponent<TransformComponent>()->GetScale().x - 0.5f, 1.0f,
                                   plate2->GetComponent<TransformComponent>()->GetScale().z - 0.5f));
    }
    
	return true;
}

void PhysicsPuzzleRoom::OnDestroy()
{

}

void PhysicsPuzzleRoom::Update(const float deltaTime)
{
    auto localPlayer = SceneManager::GetInstance()->GetLocalPlayer()->GetPawn();
    auto remotePlayer = SceneManager::GetInstance()->GetRemotePlayer()->GetPawn();

    Cube1->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(0.0f, -9.81f / 1.0f, 0.0f));
    Cube2->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(0.0f, -9.81f / 1.0f, 0.0f));
    for (auto& obj : Cubes)
    {
        if (Physics::TestTwoAABB(obj->GetComponent<AABB>(), GetActor("Bottom")->GetComponent<AABB>()))
        {            
            Vec3 tempVel = obj->GetComponent<DynamicLinearMovement>()->GetVel();
            Vec3 tempAccel = obj->GetComponent<DynamicLinearMovement>()->GetAccel();
            obj->GetComponent<DynamicLinearMovement>()->SetAccel(Vec3(tempAccel.x, 0.0f, tempAccel.z));
            obj->GetComponent<DynamicLinearMovement>()->SetVel(Vec3(tempVel.x, 0.0f, tempVel.z));
        }
        obj->Update(deltaTime);
    }

    if (localPlayer && remotePlayer)
    {
        bool status1 = Physics::TestTwoAABB(localPlayer->GetComponent<AABB>(), Cube1->GetComponent<AABB>());
        bool status2 = Physics::TestTwoAABB(remotePlayer->GetComponent<AABB>(), Cube1->GetComponent<AABB>());

        bool status3 = Physics::TestTwoAABB(localPlayer->GetComponent<AABB>(), Cube2->GetComponent<AABB>());
        bool status4 = Physics::TestTwoAABB(remotePlayer->GetComponent<AABB>(), Cube2->GetComponent<AABB>());

        if (status1 && !status2)
        {
            Physics::RigidBodyMove(localPlayer, Cube1, status1, false);
            //AudioManager::getInstance()->PlaySoundEffects("audio/retroblockHit.wav");
        }        
        else if (status2 && !status1)
        {
            Physics::RigidBodyMove(remotePlayer, Cube1, status2, false);
            //AudioManager::getInstance()->PlaySoundEffects("audio/retroblockHit.wav");
        }
        else if (status3 && !status4)
        {
            Physics::RigidBodyMove(localPlayer, Cube2, status3, false);
            //AudioManager::getInstance()->PlaySoundEffects("audio/retroblockHit.wav");
        }
        else if (status4 && !status3)
        {
            Physics::RigidBodyMove(remotePlayer, Cube2, status4, false);
            //AudioManager::getInstance()->PlaySoundEffects("audio/retroblockHit.wav");
        }
        else
        {
            Physics::RigidBodyMove(localPlayer, Cube1, false, false);
            Physics::RigidBodyMove(remotePlayer, Cube1, false, false);

            Physics::RigidBodyMove(localPlayer, Cube2, false, false);
            Physics::RigidBodyMove(remotePlayer, Cube2, false, false);
        }

    }
}

void PhysicsPuzzleRoom::CheckPuzzle(){

    bool status1 = Physics::TestTwoAABB(plate1->GetComponent<AABB>(), Cube1->GetComponent<AABB>());
    bool status2 = Physics::TestTwoAABB(plate2->GetComponent<AABB>(), Cube2->GetComponent<AABB>());
    
    if (status1) {
        plate1->SetAlpha(1.0f);
    }
    else {
        plate1->SetAlpha(0.5f);
    }

    if (status2) {
        plate2->SetAlpha(1.0f);
    }
    else {
        plate2->SetAlpha(0.5f);
    }
    
    if (status1 && status2)
    {
        puzzleSolved = true;
    }
}
