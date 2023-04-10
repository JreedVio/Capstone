#include "PauseMenu.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "SettingMenu.h"

PauseMenu::PauseMenu(UserInterface* parent_) : UserInterface(parent_) {

}

PauseMenu::~PauseMenu() {}

void PauseMenu::Display() {
    windowAlpha = UIManager::getInstance()->GetAlpha();

    if (showWindow) {
        showPauseMenu();
    }

    //Show Child Elements
    ChildDisplay();

}

void PauseMenu::Update(Scene* scene_) {
    for (auto ui_ : UIChildList) {
        ui_.second->setSize(width, height);
    }
}

void PauseMenu::showAudioControl() {
    float itemWidth = ImGui::GetWindowSize().x * 0.9f;
}

void PauseMenu::showPauseMenu() {

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Pause Menu", &showWindow,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar);

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
    const char* menuTitle = "Pause Menu";
    float textWidth = ImGui::CalcTextSize(menuTitle).x;
    ImGui::SetCursorPos(ImVec2((windowW - textWidth) * 0.5f, windowH * 0.1f));
    ImGui::Text(menuTitle);

    ImGui::SetWindowFontScale(2.0f);

    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Resume", buttonSize)) {
        //Close the window
        ShowWindow(false);
    }

    buttonLocation.y += heightInterval;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Main Menu", buttonSize)) {
        //Got back to main menu
        ShowWindow(false);
        SceneManager::GetInstance()->MainMenu();
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

