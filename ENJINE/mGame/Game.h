#pragma once
#include "SFML/Graphics.hpp"
#include "mGame/Scenes/LauncherScene.h"
#include "mGame/Scenes/WorldScene.h"
#include "mGame/Managers/Network/NetworkManager.h"
#include <mGame/Managers/Scene/SceneManager.h>
#include <mGame/Managers/Texture/TextureManager.h>
#include "Utils/Logger/Logger.h"

class Game {
private:
	Logger m_logger;
	RenderWindow* m_window;
	NetworkManager* m_netmg		= nullptr;
	SceneManager* m_scenemg		= nullptr;
	TextureManager* m_texturemg = nullptr;
public:
	Game();

	Logger getLogger() { return m_logger; }
	RenderWindow*	getRenderWindow()	{ return m_window; }
	NetworkManager* getNetworkManager() { return m_netmg; }
	SceneManager*	getSceneManager()	{ return m_scenemg; }
	TextureManager* getTextureManager() { return m_texturemg; }

	int run();
};

