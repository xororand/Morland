#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include <format>

#include "Utils/Utils.h"
#include "version.h"

using namespace sf;

class Game;

class UI
{
private:
    Game* m_game = nullptr;
	struct m_settings {
		static inline int space_points_count = 4000;
		static inline int space_comets_count = 4000;
		static inline bool vsync = true;
		static inline VertexArray space_points;
		static inline VertexArray space_comets;
		static inline bool is_debug = false;
	};
	Vector2f view_center = Vector2f(0.0f, 0.0f);
	Vector2f toview_center = Vector2f(0.0f, 0.0f);
public:
    UI(Game* game);
    Game* getGame();
	
    void drawVersion();
	void drawBackgroundSpaceCircleEffect(float deltatime);
};

