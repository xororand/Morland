#include "UI.h"
#include "mGame/Game.h"

UI::UI(Game* game) { 
    srand(time(0));
    m_game = game;

    RenderWindow* rw = getGame()->getRenderWindow();
    m_settings::space_points = VertexArray(Points, m_settings::space_points_count);
    toview_center = Vector2f(rw->getSize().x / 2, rw->getSize().y / 2);
    view_center = toview_center;

    for (int i = 0; i < m_settings::space_points_count; i++) {
        int rand_ = rand();

        int width = rw->getSize().x; //(float)(rand_ % rw->getSize().x);
        int height = rw->getSize().y; //(float)(rand_ % rw->getSize().y);

        if (width == 0 or height == 0) continue;

        width = rand_ % width;
        height = rand_ % height;

        m_settings::space_points[i].position = Vector2f(width, height);
    }
}
Game* UI::getGame() { return m_game; }

void UI::drawVersion() {
    RenderWindow* rw = getGame()->getRenderWindow();
    ImGui::Begin("credits", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowPos(ImVec2(0, rw->getSize().y - ImGui::GetWindowHeight()));
    ImGui::Text("ver%i.%i.%i%s", MAJOR_VER, MINOR_VER, PATCH_VER, DEFICE);
    ImGui::End();
}

void UI::drawBackgroundSpaceCircleEffect(float deltatime) {
    RenderWindow* rw = getGame()->getRenderWindow();

    float max = 0.0f;
    for (int i = 0; i < m_settings::space_points_count; i++) {
        float scale_delta_y = sinf(i * 0.2f + ImGui::GetTime() * 0.25f);
        float scale_delta_x = cosf(i * 0.2f + ImGui::GetTime() * 0.25f);

        float rad_delta = i / m_settings::space_points_count;

        double angle = (.0015 * (rad_delta - 1.0 + 0.5));

        

        float dist_tocenter = vmath::distance(Vector2f(rw->getSize().x / 2, rw->getSize().y / 2), m_settings::space_points[i].position);
        if (dist_tocenter > max)
            max = dist_tocenter;


        Vector2f to_center = (Vector2f(rw->getSize().x, rw->getSize().y) - m_settings::space_points[i].position);
        to_center.x += (20.0f * scale_delta_x);
        to_center.y += (20.0f * scale_delta_y);


        m_settings::space_points[i].position = Vector2f(to_center.x * cos(angle) - to_center.y * sin(angle),
            to_center.x * sin(angle) + to_center.y * cos(angle));
    }
    
    sf::View view = rw->getDefaultView();
    view.zoom(0.5f - cosf(ImGui::GetTime()) * 0.02f);
    toview_center += vmath::tovec2f(ImGui::GetIO().MouseDelta) * deltatime * 2.0f;
    view_center += (toview_center - view_center) * deltatime * 2.0f;
    view.setCenter(view_center);

    rw->setView(view);
    rw->draw(m_settings::space_points);
}