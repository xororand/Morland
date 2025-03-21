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
	bool m_isRWinit			= false;

	std::mutex onswitchscene;
public:
	SceneManager(Game* game);

	Game* getGame() { return m_game; }

	bool isRWinit() { return m_isRWinit; }
	void isRWinit(bool b) { m_isRWinit = b; }

	void setScene(Scene::Type type);
	void setScene(Scene* scene);
	void setScene(Scene::Type type, std::wstring name);

	Scene* getCurrentScene() { return m_current_scene; }

	void setSceneName(std::wstring name);

	void processScene();
};

