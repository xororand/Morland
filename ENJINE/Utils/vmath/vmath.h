#pragma once
#include "SFML/Graphics.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

using namespace sf;
using namespace std;

class vmath
{
public:
	static float distance(Vector2f a, Vector2f b);
	static Vector2f tovec2f(ImVec2 a);
};

