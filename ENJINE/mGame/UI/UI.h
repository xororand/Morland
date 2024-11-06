#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "mGame/defines.h"
#include "Utils/Utils.h"

#include <format>

using namespace sf;

class Game;

class UI
{
private:
    Game* m_game = nullptr;
public:
    UI(Game* game);
    Game* getGame();

    void drawVersion();

};

