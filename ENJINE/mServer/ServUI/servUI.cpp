#include "servUI.h"
#include "../Server.h"

servUI::servUI(Server* serv)
{
    this->m_serv = serv;
}

Server* servUI::getServer()
{
    return this->m_serv;
}

void servUI::drawDebug() {
    // ДЕБАГ ПАРАМЕТРОВ СЕРВЕРА
    ImGuiWindowFlags style1 = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;
    if(ImGui::Begin(to_ancii(L"ПАРАМЕТРЫ СЕРВЕРА"), NULL, style1)) {
        //ImGui::PushFont(getStandartFont());
        ImGui::SetWindowPos(ImVec2(0.0, 0.0));
        
        static float f = 35.0f;
        getServer()->getRenderWindow()->setFramerateLimit((int)f);

        ImVec2 m_vec = ImGui::GetMousePos();
        Server::debug_stats* ds = getServer()->getDebugStats();

        // TPS
        float ms;
        float tps;
        {
            
            ms = 1000.0f / ds->m_current_tps;
            tps = ds->m_current_tps;
            if (ds->tps_point == 500)
                ds->tps_point = 0;
            ds->tps_samples[ds->tps_point] = tps;
            ds->tps_point++;
            tps = 0;
            for (auto v : ds->tps_samples)
                tps += v;
            tps /= 500;
            ms = 1000.0f / tps;
        }

        // PLOT TPS
        float max_tps = 0.0f;
        {
            max_tps = 0.0f;
            for (auto sample : ds->tps_samples)
                if (sample > max_tps)
                    max_tps = sample;
        }

        ImGui::SliderFloat("MAX FPS", &f, 0.0f, 144.0f);
        ImGui::Text("MOUSE POS:\t(%.1f,%.1f)", m_vec.x, m_vec.y);
        ImGui::Text("TIME:\t%.1fsec; FRAMES:\t%it", ImGui::GetTime(), ImGui::GetFrameCount());
        ImGui::Text("Application average %.3f ms/tick (%.1f TPS)", ms, tps);
        ImGui::PlotLines("TPS", ds->tps_samples, 500, 0, NULL, 0.0, max_tps * 2.0, ImVec2(0.0, 70.0));

        ImGui::ShowDemoWindow();

        ImGui::End();
        //ImGui::PopFont();
    }
    
    // ДЕБАГ ИГРОКОВ
    if(ImGui::Begin(to_ancii(L"ИГРОКИ"), NULL)){
        // ID | STATUS | IP:PORT
        std::vector<std::wstring> cols_names {
            L"ID",
            L"STATUS",
            L"IP:PORT"
        };

        std::deque<Player*> players = getServer()->getPlayers();

        if (ImGui::BeginTable(to_ancii(L"ИГРОКИ_таблица"), cols_names.size()))
        {
            for (int row = 0; row < players.size(); row++)
            {
                if (players[row] == NULL) continue; // пропуск пустых ячеек, освобожденных после выхода игроков

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", players[row]->getID());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", to_ancii(Player::to_wstring(players[row]->getStatus())) );

                TcpSocket* tcp = players[row]->getTcp();
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s:%d", tcp->getRemoteAddress().toString().c_str(), tcp->getLocalPort());
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }
}