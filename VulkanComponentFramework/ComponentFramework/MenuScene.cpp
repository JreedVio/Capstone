#include "MenuScene.h"
#include <iostream>
#include "Debug.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"

MenuScene::MenuScene(VulkanRenderer* renderer_):Scene(renderer_) {
    //camera = std::make_shared<CameraActor>(nullptr);
}

MenuScene::~MenuScene(){
    OnDestroy();
}

bool MenuScene::OnCreate(){

    return true;
}

void MenuScene::OnDestroy(){
}

void MenuScene::Update(const float deltaTime){

}

void MenuScene::Render() const{
    renderer->Render();
}

void MenuScene::HandleEvents(const SDL_Event& sdlEvent){


}

