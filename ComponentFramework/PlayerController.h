#pragma once

#include "Input.h"
#include "Component.h"
#include "TransformComponent.h"

// THIS IS A COMPONENT CLASS
// it will need to added to the actor like a component would

class Actor;
class UIManager;

class PlayerController : public Input, public Component {

private:

    Ref<Actor> pawnActor;
    const char* actorName;
    const char* checkPoint;
    int roomSurvived;
    UIManager* uiManager;
    bool isCreated;


public:

    PlayerController(Component* parent_, const char* actorName_);   
    
    void GetPlayerInput(const SDL_Event& Event);

    virtual ~PlayerController();
    
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

