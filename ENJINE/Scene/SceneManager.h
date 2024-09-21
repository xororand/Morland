#pragma once
#include "Scene.h"
#include "Scene/scenes/LauncherScene.h"
#include "Scene/scenes/WorldScene.h"
class SceneManager
{
private:
	Scene* m_current_scene	= NULL;
	RenderWindow* m_window	= NULL;
	NetworkManager* m_netmg = NULL;
public:
	SceneManager(RenderWindow* window, NetworkManager* netmg);

	RenderWindow* getRenderWindow();
	NetworkManager* getNetworkManager();

	void setScene(Scene::Type type);
	void setScene(Scene* scene);
	void processScene();
};

