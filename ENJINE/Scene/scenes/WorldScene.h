#pragma once
#include <Scene/Scene.h>
#include <Network/NetworkManager.h>

class WorldScene : public Scene
{
private:
	RootObj m_root;
	RenderWindow* rw;
public:
	WorldScene(SceneManager* scenemg);

	void onProcess();
	void onDraw();
	void omUpdateEvents();
};

