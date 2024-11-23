#pragma once
#include <mGame/Managers/Scene/Scene.h>
#include <mGame/Managers/Network/NetworkManager.h>
#include "mGame/UI/UI.h"

class LauncherScene : public Scene
{
public:
	enum Page {
		main,
		login,
		registration
	};
private:
	G_RootObj m_root;

	std::shared_ptr<UI> ui;

	Thread* t_connect_serv = nullptr;
public:
	LauncherScene(Game* game);

	void drawMain();
	void drawLogin();
	void drawRegistration();

	void onProcess();
	void onDraw();
	void onUpdateEvents();
};

