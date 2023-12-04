#include "WinMenu.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "UIManager.h"

WinMenu::WinMenu(UserInterface* parent_) : UserInterface(parent_) {

}

WinMenu::~WinMenu(){}

void WinMenu::Display(){


    if (showWindow) {
        showWinMenu();
    }



    //Show Child Elements
    ChildDisplay();

}

void WinMenu::Update(Scene* scene_){
    for (auto ui_ : UIChildList) {
       ui_.second->setSize(width, height);
    }
}

void WinMenu::showWinMenu(){

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Win Menu", &showWindow,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowFontScale(1.5f);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    float windowW = width * 0.4f;
    float windowH = height * 0.6f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(width / 2.0f - windowW * 0.5f, height / 2.0f - windowH * 0.5f));

    //Calculate button size and location
    float buttonSizeX = 0.6f;
    float buttonLocX = windowW * ((1.0f - buttonSizeX) / 2.0f);
    ImVec2 buttonSize(windowW * buttonSizeX, windowH * 0.1f);
    ImVec2 buttonLocation(buttonLocX, windowH * 0.35f);
    float heightInterval = windowH * 0.13f;

    //Calculate Text size and location
    ImGui::SetWindowFontScale(5.0f);
    const char* gameTitle = "You Win!";
    float textWidth = ImGui::CalcTextSize(gameTitle).x;
    ImGui::SetCursorPos(ImVec2((windowW - textWidth) * 0.5f, windowH * 0.1f));
    ImGui::Text(gameTitle);

    ImGui::SetWindowFontScale(2.0f);
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Main Menu", buttonSize)) {
        //Close the window
        SceneManager::GetInstance()->MainMenu();
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Quit", buttonSize)) {
        SceneManager::GetInstance()->QuitGame();
    }

    ImGui::End();
}


