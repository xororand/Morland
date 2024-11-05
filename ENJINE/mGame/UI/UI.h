#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "defines.h"
#include "Utils/Utils.h"

#include <format>

using namespace sf;

class UI
{
private:
    RenderWindow* m_rw;
public:
    UI(RenderWindow* rw) { m_rw = rw; }

    void drawVersion() {
        ImGui::Begin("credits", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration); 
        ImGui::SetWindowPos(ImVec2(0, m_rw->getSize().y - ImGui::GetWindowHeight()));
        ImGui::Text("ver%i.%i.%i%s", MAJOR_VER, MINOR_VER, PATCH_VER, DEFICE);
        ImGui::End();
    }

};

