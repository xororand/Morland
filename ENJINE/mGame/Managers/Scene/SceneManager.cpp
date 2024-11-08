#include "SceneManager.h"
#include "mGame/Game.h"

SceneManager::SceneManager(Game* game) {
	m_game = game;

	getGame()->getLogger()->info(L"Инициализация SceneManager успешна.");
}

void SceneManager::setScene(Scene::Type type) {
	// TODO: wait for overing scene
	switch (type) {
		case Scene::LauncherScene:
			setScene(new LauncherScene( getGame() ));
			break;

		case Scene::WorldScene:
			setScene(new WorldScene( getGame() ));
			break;
	}
}
void SceneManager::setScene(Scene* scene) {
	// TODO: wait for overing scene
	m_current_scene = scene;
}

void SceneManager::setSceneName(std::wstring name)
{
	if (m_current_scene == NULL) return;
	m_current_scene->setName(name);
}

void SceneManager::processScene() {
	m_current_scene->process();
}