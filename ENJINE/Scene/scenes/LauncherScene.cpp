#include "LauncherScene.h"
#include "../SceneManager.h"

LauncherScene::LauncherScene(SceneManager* scenemg) {
	setType(Scene::LauncherScene);
	m_scenemg = scenemg;
	m_root = RootObj();

	rw = m_scenemg->getRenderWindow();

	rw->create(sf::VideoMode(800, 500), "Morland");
	rw->setFramerateLimit(144);

	ImGui::SFML::Init(*rw);
}

void LauncherScene::onProcess() {
	if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
		m_scenemg->setScene(Scene::WorldScene);
	}
}

void LauncherScene::onDraw() {
	ImGui::SFML::Update(*rw, deltaClock.restart());
	rw->clear();

	// Отрисовка всех объектов из рута
	m_root.draw(*rw);

	ImGui::SFML::Render(*rw);
	rw->display();
}

void LauncherScene::omUpdateEvents() {
	sf::Event event;
	while (rw->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*rw, event);
		if (event.type == sf::Event::Closed)
			rw->close();
	}
}