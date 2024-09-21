#pragma once
#include <Scene/Scene.h>
#include <Network/NetworkManager.h>

class LauncherScene : public Scene
{
private:
	RootObj m_root;
	RenderWindow* rw;
public:
	LauncherScene(SceneManager* scenemg);

	void onProcess();
	void onDraw();
	void omUpdateEvents();
};

