#pragma once
#include <Scene/Scene.h>
#include <Network/NetworkManager.h>
#include "mGame/UI/UI.h"

class LauncherScene : public Scene
{
private:
	RootObj m_root;
	RenderWindow* rw;

	std::shared_ptr<UI> ui;
public:
	LauncherScene(SceneManager* scenemg);

	// ����� ������ �����
	void onProcess();
	// ��������� � ������ �������
	void onDraw();
	void onUpdateEvents();
};

