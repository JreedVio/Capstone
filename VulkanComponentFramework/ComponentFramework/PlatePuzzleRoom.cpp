#include "PlatePuzzleRoom.h"
#include "SceneManager.h"
#include "Physics.h"
#include "PlateActor.h"

using namespace PHYSICS;

PlatePuzzleRoom::PlatePuzzleRoom(int width_, int length_, int height_):
                    Room(width_, length_, height_){
    isPlateEnabledLocally1 = false;
    isPlateEnabledLocally2 = false;
    isPlateEnabledRemotely1 = false;
    isPlateEnabledRemotely2 = false;
}

PlatePuzzleRoom::~PlatePuzzleRoom(){
}

bool PlatePuzzleRoom::OnCreate(){

    plate1 = std::make_shared<PlateActor>(nullptr);

    auto temp = GetActor(plate1Name);
    Ref<MeshComponent> mesh_ = temp->GetComponent<MeshComponent>();
    Ref<MaterialComponent> material_ = temp->GetComponent<MaterialComponent>();
    Ref<ShaderComponent> shader_ = temp->GetComponent<ShaderComponent>();
    Ref<TransformComponent> transform_ = temp->GetComponent<TransformComponent>();
    plate1->AddComponent(mesh_);
    plate1->AddComponent(material_);
    plate1->AddComponent(shader_);
    plate1->AddComponent(transform_);
    plate1->OnCreate();
    AddActor("Plate1Actor", plate1);
    plate1->SetAlpha(0.5f);
    temp->SetVisible(false);


    plate2 = std::make_shared<PlateActor>(nullptr);

    temp = GetActor(plate2Name);
    mesh_ = temp->GetComponent<MeshComponent>();
    material_ = temp->GetComponent<MaterialComponent>();
    shader_ = temp->GetComponent<ShaderComponent>();
    transform_ = temp->GetComponent<TransformComponent>();
    plate2->AddComponent(mesh_);
    plate2->AddComponent(material_);
    plate2->AddComponent(shader_);
    plate2->AddComponent(transform_);
    plate2->OnCreate();
    AddActor("Plate2Actor", plate2);
    plate2->SetAlpha(0.5f);
    temp->SetVisible(false);


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

    if (plate2 && plate1)
    {
        bool status1 = Physics::TestTwoAABB(localPlayer->GetComponent<AABB>(), plate1->GetComponent<AABB>());
        bool status2 = Physics::TestTwoAABB(localPlayer->GetComponent<AABB>(), plate2->GetComponent<AABB>());

        if (plate1->GetIsEnabled() && !isPlateEnabledRemotely1) {
            plate1->SetAlpha(1.0f);
            isPlateEnabledRemotely1 = true;
        }
        else if (!plate1->GetIsEnabled() && isPlateEnabledRemotely1) {
            plate1->SetAlpha(0.5f);
            isPlateEnabledRemotely1 = false;
        }

        if (plate2->GetIsEnabled() && !isPlateEnabledRemotely2) {
            plate2->SetAlpha(1.0f);
            isPlateEnabledRemotely2 = true;
        }
        else if (!plate2->GetIsEnabled() && isPlateEnabledRemotely2) {
            plate2->SetAlpha(0.5f);
            isPlateEnabledRemotely2 = false;
        }

        // It triggers when a player steps on a plate that is disabled both locally and remotely
        if (status1 && !isPlateEnabledLocally1 && !isPlateEnabledRemotely1)
        {
            plate1->SetAlpha(1.0f);
            isPlateEnabledLocally1 = true;
            NetworkManager::GetInstance()->GetUnit()->SendObjectState("Plate1Actor", true);
        }
        // It triggers when a player is not on a plate that is enabled locally and disabled remotely
        else if (!status1 && isPlateEnabledLocally1 && !isPlateEnabledRemotely1) {
            plate1->SetAlpha(0.5f);
            isPlateEnabledLocally1 = false;
            NetworkManager::GetInstance()->GetUnit()->SendObjectState("Plate1Actor", false);
        }

        // It triggers when a player steps on a plate that is disabled both locally and remotely
        if (status2 && !isPlateEnabledLocally2 && !isPlateEnabledRemotely2)
        {
            plate2->SetAlpha(1.0f);
            isPlateEnabledLocally2 = true;
            NetworkManager::GetInstance()->GetUnit()->SendObjectState("Plate2Actor", true);
        }
        // It triggers when a player is not on a plate that is enabled locally and disabled remotely
        else if (!status2 && isPlateEnabledLocally2 && !isPlateEnabledRemotely2) {
            plate2->SetAlpha(0.5f);
            isPlateEnabledLocally2 = false;
            NetworkManager::GetInstance()->GetUnit()->SendObjectState("Plate2Actor", false);
        }

        if ((isPlateEnabledLocally1 || isPlateEnabledRemotely1) &&
            (isPlateEnabledLocally2 || isPlateEnabledRemotely2)) {
            puzzleSolved = true;
        }
    }
}
