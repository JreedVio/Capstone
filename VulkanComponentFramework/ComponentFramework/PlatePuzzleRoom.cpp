#include "PlatePuzzleRoom.h"
#include "SceneManager.h"
#include "Physics.h"

using namespace PHYSICS;

PlatePuzzleRoom::PlatePuzzleRoom(int width_, int length_, int height_):
                    Room(width_, length_, height_){

}

PlatePuzzleRoom::~PlatePuzzleRoom(){
}

bool PlatePuzzleRoom::OnCreate(){

    plate1 = GetActor(plate1Name);
    plate2 = GetActor(plate2Name);

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

void PlatePuzzleRoom::Update(float deltaTime){

}

void PlatePuzzleRoom::OnDestroy(){

}

void PlatePuzzleRoom::CheckPuzzle(){

    SceneManager* sceneManager = SceneManager::GetInstance();

    Ref<Actor> localPlayer = sceneManager->GetLocalPlayer()->GetPawn();
    Ref<Actor> remotePlayer = sceneManager->GetRemotePlayer()->GetPawn();

    auto localPlayerPhysics = localPlayer->GetComponent<Physics>();
    auto remotePlayerPhysics = remotePlayer->GetComponent<Physics>();


    if (plate2 && plate1)
    {

        bool status1 = localPlayerPhysics->TestTwoAABB(localPlayer->GetComponent<AABB>(), plate2->GetComponent<AABB>());
        bool status2 = remotePlayerPhysics->TestTwoAABB(remotePlayer->GetComponent<AABB>(), plate2->GetComponent<AABB>());

        bool status3 = localPlayerPhysics->TestTwoAABB(localPlayer->GetComponent<AABB>(), plate1->GetComponent<AABB>());
        bool status4 = remotePlayerPhysics->TestTwoAABB(remotePlayer->GetComponent<AABB>(), plate1->GetComponent<AABB>());

        // change alpha
        if (status1 || status2)
        {
            plate2->SetAlpha(1.0f);
        }
        else if (status3 || status4)
        {
            plate1->SetAlpha(1.0f);
        }
        else
        {
            plate2->SetAlpha(0.5f);
            plate1->SetAlpha(0.5f);
        }

        // open door
        if ((status1 || status2) && (status3 || status4))
        {
            puzzleSolved = true;
        }
    }
}
