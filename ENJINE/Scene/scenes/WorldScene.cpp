#include "WorldScene.h"
#include "../SceneManager.h"

WorldScene::WorldScene(SceneManager* scenemg) {
	setType(Scene::WorldScene);
	setSceneManager(scenemg);

	m_root = RootObj();
	rw = getSceneManager()->getRenderWindow();

	rw->create(sf::VideoMode::getDesktopMode(), "Morland", sf::Style::Fullscreen);
	rw->setFramerateLimit(144);

	ImGui::SFML::Init(*rw);
}

void WorldScene::onProcess() {

}

void WorldScene::onDraw() {
	ImGui::SFML::Update(*rw, deltaClock.restart());
	rw->clear();

	// Отрисовка всех объектов из рута
	m_root.draw(*rw);

	ImGui::SFML::Render(*rw);
	rw->display();
}

void WorldScene::omUpdateEvents() {
	sf::Event event;
	while (rw->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*rw, event);
		if (event.type == sf::Event::Closed)
			rw->close();
	}
}