#include "MainMenu.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "SettingMenu.h"
#include "IPAddrMenu.h"

MainMenu::MainMenu(UserInterface* parent_) : UserInterface(parent_) {

}

MainMenu::~MainMenu(){}

void MainMenu::Display(){

    if (showWindow) {
        showMainMenu();
    }

    //Show Child Elements
    ChildDisplay();

}

void MainMenu::Update(Scene* scene_){
    for (auto ui_ : UIChildList) {
       ui_.second->setSize(width, height);
    }
}

void MainMenu::showAudioControl(){
    float itemWidth = ImGui::GetWindowSize().x * 0.9f;



}

void MainMenu::showMainMenu(){

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Main Menu", &showWindow,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowFontScale(1.5f);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    float windowW = width * 0.4f;
    float windowH = height * 0.7f;
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
    const char* gameTitle = "Quantum Maze";
    float textWidth = ImGui::CalcTextSize(gameTitle).x;
    ImGui::SetCursorPos(ImVec2((windowW - textWidth) * 0.5f, windowH * 0.1f));
    ImGui::Text(gameTitle);

    ImGui::SetWindowFontScale(2.0f);
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Start Game", buttonSize)) {
        //Close the window
        SceneManager::GetInstance()->StartGame(USERTYPE::SERVER);
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Join Game", buttonSize)) {
        //Close the window
        SceneManager::GetInstance()->StartGame(USERTYPE::CLIENT);
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Enter IP", buttonSize)) {
        //Open Option menu
        Ref<UserInterface> IPMenu = GetChildUI("IPMenu");
        if (IPMenu) {
            IPMenu->ShowWindow(true);
            ShowWindow(false);
        }
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Settings", buttonSize)) {
        //Open Option menu
        Ref<UserInterface> settingMenu = GetChildUI("SettingMenu");
        if (settingMenu) {
            settingMenu->ShowWindow(true);
            ShowWindow(false);
        }
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Quit", buttonSize)) {
        SceneManager::GetInstance()->QuitGame();
    }

    ImGui::End();
}


