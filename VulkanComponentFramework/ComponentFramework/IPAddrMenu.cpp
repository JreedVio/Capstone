#include "IPAddrMenu.h"
#include "UIManager.h"
#include "AudioManager.h"

void IPAddrMenu::showEnterIPAddr()
{
    /*float itemWidth = ImGui::GetWindowSize().x * 0.9f;
    ImGui::Text("Master Volume");
    ImGui::SetNextItemWidth(itemWidth);
    int IPaddr = AudioManager::getInstance()->getMasterVolume() * 100;
    if (ImGui::SliderInt("##masterVol", &masterVol_, 0, 100)) {
        AudioManager::getInstance()->setMasterVolume(masterVol_ * 0.01f);
    }*/
    
}

IPAddrMenu::~IPAddrMenu()
{
    
}

void IPAddrMenu::Display()
{
    if (!showWindow) return;

    ImGui::SetNextWindowBgAlpha(windowAlpha);
    ImGui::Begin("IP Address", &showWindow,
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
    ImVec2 buttonSize(windowW * 0.43f, windowH * 0.15f);
    ImVec2 buttonLocation(windowW * 0.52f, windowH * 0.8f);

    ImGui::SetWindowFontScale(1.0f);
    ImGui::Text("Enter Server IP Address");

    //Calculate Text size and location
    ImGui::SetWindowFontScale(4.0f);
    
    static char buf[IP_LENGTH];
    ImGui::InputText("##IP Address", buf, IM_ARRAYSIZE(buf));    

    ImGui::SetCursorPos(ImVec2(buttonLocation.x * 0.1f, buttonLocation.y));
    if (ImGui::Button("Enter", buttonSize))
    {
        entered = buf;
        
    }
    if (entered == "")
    {
        ImGui::SetWindowFontScale(1.0f);
        ImGui::SetCursorPos(ImVec2(buttonLocation.x * 0.1f, buttonLocation.y * 0.5f));
        ImGui::Text("If you wish to enter a server enter a valid IP!");
    }

    ImGui::SetWindowFontScale(4.0f);
    ImGui::SetCursorPos(ImVec2(buttonLocation.x, buttonLocation.y));
    if (ImGui::Button("Back", buttonSize))
    {
        ShowWindow(false);
        toParent();
    }

    ImGui::End();
}

void IPAddrMenu::Update(Scene* scene_)
{
    for (auto ui_ : UIChildList) {
        ui_.second->setSize(width, height);
    }
}

void IPAddrMenu::toParent() {

    if (parent) {
        parent->ShowWindow(!parent->isOpened());
    }
}
