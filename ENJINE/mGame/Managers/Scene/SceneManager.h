#pragma once
#include "Scene.h"
#include "mGame/Scenes/LauncherScene.h"
#include "mGame/Scenes/WorldScene.h"

class Game;

class SceneManager
{
private:
	Game* m_game			= nullptr;
	Scene* m_current_scene	= nullptr;
public:
	SceneManager(Game* game);

	Game* getGame() { return m_game; }

	void setScene(Scene::Type type);
	void setScene(Scene* scene);

	void setSceneName(std::wstring name);

	void processScene();
};

