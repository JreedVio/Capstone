#include "CodeUI.h"

CodeUI::CodeUI(UserInterface* parent_) : UserInterface(parent_) {

}

CodeUI::~CodeUI(){

}

void CodeUI::Display(){

    if (!showWindow) return;

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
    float buttonSizeY = 0.2f;
    float buttonLocX = windowW * ((1.0f - buttonSizeX) * (buttonSizeX + 0.02f));
    float buttonLocY = windowH * 0.22f;
    ImVec2 buttonSize(windowW * buttonSizeX, windowH * 0.15f);
    ImVec2 buttonLocation(buttonLocX, buttonLocY);
    float heightInterval = windowH * 0.18f;
    float widthInterval = windowW * 0.22f;

    //Calculate Text size and location
    ImGui::SetWindowFontScale(4.0f);
    std::string codeStr;
    for (auto num : entered) {
        codeStr.append(std::to_string(num));
    }

    const char* code = codeStr.c_str();
    float textWidth = ImGui::CalcTextSize(code).x;
    ImGui::SetCursorPos(ImVec2((windowW - textWidth) * 0.5f, windowH * 0.05f));
    ImGui::Text(code);

    //Set up the number buttons
    ImGui::SetWindowFontScale(3.0f);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            buttonLocation.x = buttonLocX + widthInterval * j;
            buttonLocation.y = buttonLocY + heightInterval * i;
            ImGui::SetCursorPos(buttonLocation);
            int buttonNum = (i * 3) + j + 1;
            if (ImGui::Button(std::to_string(buttonNum).c_str(), buttonSize)){
                if (entered.size() < CODE_LENGTH) {
                    entered.push_back(buttonNum);
                }
            }
        }
    }

    //Enter
    ImGui::SetWindowFontScale(2.0f);
    buttonSizeX = 0.5f;
    buttonSizeY = 0.2f;
    buttonSize = ImVec2(windowW * 0.3f, windowH * 0.15f);
    buttonLocation.x = buttonLocX;
    buttonLocation.y = buttonLocY + heightInterval * 3.0f;
    //ImGui::SetCursorPos(buttonLocation);
    //if (ImGui::Button("Enter", buttonSize)) {

    //}
    //Delete
    buttonLocation.x = buttonLocX + heightInterval * 1.8f;
    ImGui::SetCursorPos(buttonLocation);
    if (ImGui::Button("Delete", buttonSize)) {
        if (entered.size() > 0) {
            entered.pop_back();
        }
    }

    ImGui::End();
}

void CodeUI::Update(Scene* scene_){
    for (auto ui_ : UIChildList) {
        ui_.second->setSize(width, height);
    }
}

