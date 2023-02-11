#pragma once

#include "Input.h"
#include "Component.h"
#include "TransformComponent.h"

// THIS IS A COMPONENT CLASS
// it will need to added to the actor like a component would


class PlayerController :
    public Input, public Component
{              



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

};

