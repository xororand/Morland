#pragma once
#include "SFML/Graphics.hpp"

#include "Utils/vmath/vmath.h"
#include "mGame/Managers/Network/NetworkManager.h"
#include <mGame/Managers/Texture/TextureManager.h>
#include <mGame/Managers/Scene/SceneManager.h>
#include "mGame/Scenes/LauncherScene.h"
#include "mGame/Scenes/WorldScene.h"
#include "Utils/Logger/Logger.h"
#include "version.h"

#include <thread>

class Game {
private:
	Logger* m_logger			= nullptr;
	ImGuiIO* m_io				= nullptr;
	RenderWindow* m_window		= nullptr;
	NetworkManager* m_netmg		= nullptr;
	SceneManager* m_scenemg		= nullptr;
	TextureManager* m_texturemg = nullptr;
public:
	Game();

	Logger* getLogger()					{ return m_logger; }
	ImGuiIO* getIO()					{ return m_io; }
	RenderWindow*	getRenderWindow()	{ return m_window; }
	NetworkManager* getNetworkManager() { return m_netmg; }
	SceneManager*	getSceneManager()	{ return m_scenemg; }
	TextureManager* getTextureManager() { return m_texturemg; }

	int run();
};

