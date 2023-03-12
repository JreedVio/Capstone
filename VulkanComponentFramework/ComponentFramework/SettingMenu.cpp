#include "SettingMenu.h"
#include "TransformComponent.h"
#include "UIManager.h"

SettingMenu::~SettingMenu(){}

void SettingMenu::Display(){
    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("Settings", NULL, 
           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::SetWindowFontScale(1.5f);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    float windowW = width * 0.4f;
    float windowH = height * 0.6f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(width / 2.0f - windowW * 0.5f, height / 2.0f - windowH * 0.5f));

    ImVec2 buttonSize(windowW * 0.8f, windowH * 0.1f);
    ImVec2 buttonLocation(windowW * 0.1f, windowH * 0.3f);

    //Check hovering
    checkHovered();

    //Calculate tab width
    int tabNum = 2;
    float tabWidth = ImGui::GetWindowSize().x / tabNum;
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("OptionTab", tab_bar_flags))
    {
        //ImGui::SetNextItemWidth(tabWidth);
        const char* tabName = "Gameplay";
        float textWidth = ImGui::CalcTextSize(tabName).x;

        ImGui::SetNextItemWidth(tabWidth);
        tabName = "Audio";
        textWidth = ImGui::CalcTextSize(tabName).x;
        if (ImGui::BeginTabItem(tabName))
        {
            showAudioControl();
            ImGui::EndTabItem();
        }
        ImGui::SetNextItemWidth(tabWidth);
        tabName = "UI";
        textWidth = ImGui::CalcTextSize(tabName).x;
        if (ImGui::BeginTabItem(tabName)) {

            ImGui::Text("UI Theme");
            //Change Theme
            static int choice = 0;
            if (ImGui::RadioButton("Default Theme", &choice, 0)) {
                UIManager::getInstance()->defaultTheme();
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("White Theme", &choice, 1)) {
                UIManager::getInstance()->whiteTheme();
            }
            //Change alpha
            ImGui::Text("UI Window Transparency");
            ImGui::SliderFloat("##windowAlpha", &windowAlpha, 0.0f, 1.0f, "%.2f");            

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::SetCursorPos(ImVec2(buttonLocation.x, windowH * 0.85f));
    if (ImGui::Button("Back", buttonSize)) {
        //Close the window
        ShowWindow(false);
        toParent();
    }

    ImGui::End();

}

void SettingMenu::Update(Scene* scene_){

}


void SettingMenu::showAudioControl(){
    float itemWidth = ImGui::GetWindowSize().x * 0.9f;
}

void SettingMenu::toParent(){

    if (parent) {
        parent->ShowWindow(!parent->isOpened());
    }
}


