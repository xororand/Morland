#pragma once
#include <mGame/Managers/Scene/Scene.h>
#include <mGame/Managers/Network/NetworkManager.h>
#include "mGame/UI/UI.h"

class WorldScene : public Scene
{
private:
	RootObj m_root;

	std::shared_ptr<UI> ui;
public:
	WorldScene(Game* game);

	void onProcess();
	void onDraw();
	void onUpdateEvents();
};

