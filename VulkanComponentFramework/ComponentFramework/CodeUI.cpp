#include "CodeUI.h"

CodeUI::CodeUI(UserInterface* parent_) : UserInterface(parent_) {
}

CodeUI::~CodeUI(){
}

void CodeUI::Display(){

    //if (!showWindow) return;

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("CodeUI", &showWindow,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowFontScale(1.5f);
    //Calculate ui window size
    //It needs to be dependent to the scene window size, so that when resizing the scene window,
    //ui window will change accordingly
    float windowW = width * 0.3f;
    float windowH = height * 0.55f;
    ImGui::SetWindowSize(ImVec2(windowW, windowH));
    ImGui::SetWindowPos(ImVec2(width / 2.0f - windowW * 0.5f, height / 2.0f - windowH * 0.5f));


    //Calculate button size and location
    float buttonSizeX = 0.2f;
    float buttonLocX = windowW * ((1.0f - buttonSizeX) / 6.0f);
    ImVec2 buttonSize(windowW * buttonSizeX, windowH * 0.15f);
    ImVec2 buttonLocation(buttonLocX, windowH * 0.35f);
    float heightInterval = windowH * 0.25f;
    float widthInterval = windowW * 0.2f;

    //Calculate Text size and location
    ImGui::SetWindowFontScale(5.0f);
    const char* code = "Empty";
    float textWidth = ImGui::CalcTextSize(code).x;
    ImGui::SetCursorPos(ImVec2((windowW - textWidth) * 0.5f, windowH * 0.1f));
    ImGui::Text(code);

    ImGui::SetWindowFontScale(2.0f);
    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 3; j++) {

            buttonLocation.x = buttonLocX + heightInterval * j;
            buttonLocation.y = windowH * 0.35f + widthInterval * i;
            ImGui::SetCursorPos(buttonLocation);
            if (ImGui::Button(std::to_string(i + j + 1).c_str(), buttonSize)){
            }
        }


    }



    ImGui::End();
}

void CodeUI::Update(Scene* scene_){
    for (auto ui_ : UIChildList) {
        ui_.second->setSize(width, height);
    }
}

