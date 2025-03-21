﻿#include "servUI.h"
#include "../Server.h"
#include "mGame/UI/UI.h"

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
            int count = 0;
            for (auto v : ds->tps_samples)
                if (v != 0) { 
                    count++;
                    tps += v; 
                }
            tps /= count;
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
        ImGui::PlotLines("TPS", ds->tps_samples, 500, 0, NULL, 0.0, max_tps * 2.0f, ImVec2(0.0, 70.0));

        ImGui::ShowDemoWindow();

        ImGui::End();
        //ImGui::PopFont();
    }
    
    // ДЕБАГ ИГРОКОВ
    if(ImGui::Begin(to_ancii(L"PLAYERS"), NULL)){
        // ID | STATUS | IP:PORT
        std::vector<std::wstring> cols_names {
            L"ID",
            L"PING",
            L"USERNAME",
            L"POS",
            L"STATUS",
            L"IP:PORT",
            L""
        };
        
        std::deque<Peer*> peers = getServer()->getPeers();

        if (ImGui::BeginTable(to_ancii(L"PLAYERS table"), (int)cols_names.size()))
        {
            
            for (int col = 0; col < cols_names.size(); col++)
            {
                ImGui::TableSetupColumn(to_ancii(cols_names[col]), ImGuiTableColumnFlags_WidthStretch, 0.0f, (ImGuiID)(col / static_cast<float>(cols_names.size()) ));
            }
            ImGui::TableHeadersRow();
            for (int row = 0; row < peers.size(); row++)
            {
                Peer* peer = peers[row];
                if (peer == nullptr) continue; // пропуск пустых ячеек, освобожденных после выхода игроков
                S_PlayerObj* pobj = peer->getPlayerObj();

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", peer->getID());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%dms", peer->getPingMS());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", to_ancii(peer->getUsername()));

                b2Vec2 pos = b2Vec2(0.0f, 0.0f);
                if (pobj != nullptr) pos = b2Body_GetPosition(pobj->getBodyID());

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%f|%f", pos.x, pos.y);

                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%s", to_ancii(Peer::to_wstring(peer->getStatus())) );

                TcpSocket* tcp = peer->getTcp();
                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%s:%d", tcp->getRemoteAddress().toString().c_str(), tcp->getLocalPort());

                ImGui::TableSetColumnIndex(6);
                std::string but_dis_name = std::format("Disconnect###{}", row);
                if (ImGui::Button(but_dis_name.c_str())) {
                    peer->setDisconnectReason(L"Admin");
                    peer->disconnect();
                }
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }

    if(ImGui::Begin("credits", NULL, style1)) {
        RenderWindow* rw = getServer()->getRenderWindow();
        ImGui::SetWindowPos(ImVec2(0, rw->getSize().y - ImGui::GetWindowHeight()));
        ImGui::Text("ver%i.%i.%i%s", MAJOR_VER, MINOR_VER, PATCH_VER, DEFICE);
        ImGui::End();
    }
}