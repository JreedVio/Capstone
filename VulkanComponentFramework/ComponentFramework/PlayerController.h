#pragma once

#include "Input.h"
#include "Component.h"
#include "TransformComponent.h"

// THIS IS A COMPONENT CLASS
// it will need to added to the actor like a component would

class Actor;

class PlayerController : public Input, public Component {

private:

    Ref<Actor> pawnActor;
    const char* checkPoint;
    int roomSurvived;

public:   

    PlayerController(Component* parent_);   

    // (TODO): will need to pass a ref of the Transform Component to this function so it can manipulate the transform
    void GetPlayerInput(const SDL_Event& Event, TransformComponent* TF_Component);

    virtual ~PlayerController();

    // (TODO): later they won't override
    bool OnCreate() override;
    void OnDestroy() override;
    void Update(const float deltaTime_) {}
    void Render() const {}

    Ref<Actor> GetPawn() { return pawnActor; }
    void SetPawn(Ref<Actor> pawn_) { pawnActor = pawn_; }

    const char* GetCheckPoint() { return checkPoint; }
    void SetCheckPoint(const char* checkPoint_) { checkPoint = checkPoint_; }

    int GetSurvivedRoom() { return roomSurvived; }
    /*Add the value to the survived room*/
    void AddSurvivedRoom(int value_) { roomSurvived += value_; }
};

