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
void SceneManager::setScene(Scene::Type type, std::wstring name) {
	// TODO: wait for overing scene
	std::lock_guard lock(onswitchscene);
	setScene(type);
	setSceneName(name);
}
void SceneManager::setSceneName(std::wstring name)
{
	if (m_current_scene == NULL) return;
	m_current_scene->setName(name);
}

void SceneManager::processScene() {
	std::lock_guard lock(onswitchscene);
	m_current_scene->process();
}