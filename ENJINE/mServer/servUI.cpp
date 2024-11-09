#include "servUI.h"
#include "Server.h"

servUI::servUI(Server* serv)
{
    this->m_serv = serv;
}

Server* servUI::getServer()
{
    return this->m_serv;
}

void servUI::drawDebug() {
    //ImGui::PushFont(getStandartFont());

    ImGui::Begin(Utils::encoding::to_utf8(L"Äåáàã ÈÍÔÎ").c_str(), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    //static float f = 0.0f;
    //ImGui::SliderFloat("MAX FPS", &f, 0.0f, 144.0f);
    //window->setFramerateLimit((int)f);


    //ImGui::Text("CPU:\t%s", Utils::computer::getCpu().c_str());
    //ImGui::Text("GPU:\t%s", Utils::encoding::to_utf8(Utils::computer::getGpu()).c_str());
    //DWORD max, used;
    //Utils::computer::getRAM(max, used);
    //ImGui::Text("RAM:\t%i MB / %i%s %s", max, used, "%", Utils::encoding::to_utf8(L"çàðóæåíî").c_str());
    ImVec2 m_vec = ImGui::GetMousePos();

    Server::debug_stats* ds = getServer()->getDebugStats();

    float ms = 1000.0f / ds->m_current_tps;
    float tps = ds->m_current_tps;

    //ImGui::Text("PLAYER POS:\t(\t%.1f,\t%.1f\t)", m_peer->get_character_node()->get_pos().x, m_peer->get_character_node()->get_pos().y);


    if (ds->tps_point == 500)
        ds->tps_point = 0;

    ds->tps_samples[ds->tps_point] = tps;
    ds->tps_point++;

    tps = 0;
    for (auto v : ds->tps_samples)
        tps += v;

    tps /= 500;
    ms = 1000.0f / tps;

    ImGui::Text("MOUSE POS:\t(%.1f,%.1f)", m_vec.x, m_vec.y);
    ImGui::Text("TIME:\t%.1fsec; TICKS:\t%it", ImGui::GetTime(), ImGui::GetFrameCount());
    ImGui::Text("Application average %.3f ms/tick (%.1f TPS)", ms, tps);

    // âû÷èñëÿåì ñàìûé áîëüøîé ôïñ èç íàøåãî ñïèñêà
    float max_tps = 0.0f;
    for (auto sample : ds->tps_samples)
        if (sample > max_tps)
            max_tps = sample;

    ImGui::PlotLines("TPS", ds->tps_samples, 500, 0, NULL, 0.0, max_tps * 2.0, ImVec2(0.0, 70.0));

    //ImGui::ShowDemoWindow();

    ImGui::End();
    //ImGui::PopFont();
}