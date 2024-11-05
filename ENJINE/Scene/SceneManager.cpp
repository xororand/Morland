#include "SceneManager.h"

SceneManager::SceneManager(RenderWindow* window, NetworkManager* netmg) {
	m_window = window;
	m_netmg = netmg;
}

RenderWindow* SceneManager::getRenderWindow() { return m_window; }
NetworkManager* SceneManager::getNetworkManager() { return m_netmg; }

void SceneManager::setScene(Scene::Type type) {
	// TODO: wait for overing scene
	switch (type) {
		case Scene::LauncherScene:
			setScene(new LauncherScene(this));
			break;

		case Scene::WorldScene:
			setScene(new WorldScene(this));
			break;
	}
}
void SceneManager::setScene(Scene* scene) {
	// TODO: wait for overing scene
	m_current_scene = scene;
}

void SceneManager::processScene() {
	m_current_scene->process();
}