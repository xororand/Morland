#include "UI.h"
#include "mGame/Game.h"

UI::UI(Game* game) { m_game = game; }
Game* UI::getGame() { return m_game; }

void UI::drawVersion() {
    RenderWindow* rw = getGame()->getRenderWindow();
    ImGui::Begin("credits", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowPos(ImVec2(0, rw->getSize().y - ImGui::GetWindowHeight()));
    ImGui::Text("ver%i.%i.%i%s", MAJOR_VER, MINOR_VER, PATCH_VER, DEFICE);
    ImGui::End();
}