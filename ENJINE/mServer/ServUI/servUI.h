#pragma once

#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <windows.h>
#include <format>

#include "Utils/Utils.h"
#include "version.h"

class Server;

class servUI {
private:
    Server* m_serv = nullptr;
public:
    servUI(Server* serv);
    Server* getServer();

    void drawDebug();
};