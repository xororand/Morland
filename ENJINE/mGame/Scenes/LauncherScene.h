#pragma once
#include <mGame/Managers/Scene/Scene.h>
#include <mGame/Managers/Network/NetworkManager.h>
#include "mGame/UI/UI.h"

class LauncherScene : public Scene
{
private:
	G_RootObj m_root;

	std::shared_ptr<UI> ui;

	Thread* t_connect_serv = nullptr;
public:
	LauncherScene(Game* game);

	void onProcess();
	void onDraw();
	void onUpdateEvents();
};

