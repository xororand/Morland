#include "WorldScene.h"
#include "mGame/Managers/Scene/SceneManager.h"

#include "mGame/Game.h"

WorldScene::WorldScene(Game* game) {
	setType(Scene::WorldScene);
	setGame(game);
	
	m_root = RootObj();
	RenderWindow* rw = getGame()->getRenderWindow();

	rw->create(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen);
	rw->setFramerateLimit(144);

	ImGui::SFML::Init(*rw);
}

void WorldScene::onProcess() {

}

void WorldScene::onDraw() {
	RenderWindow* rw = getGame()->getRenderWindow();
	NetworkManager* nm = getGame()->getNetworkManager();

	ImGui::SFML::Update(*rw, deltaClock.restart());
	rw->clear();

	// Отрисовка всех объектов из рута
	m_root.draw(*rw);

	ImGui::SFML::Render(*rw);
	rw->display();
}

void WorldScene::omUpdateEvents() {
	sf::Event event;
	RenderWindow* rw = getGame()->getRenderWindow();
	while (rw->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*rw, event);
		if (event.type == sf::Event::Closed)
			rw->close();
	}
}