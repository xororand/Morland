#include "WorldScene.h"
#include "mGame/Managers/Scene/SceneManager.h"

#include "mGame/Game.h"

WorldScene::WorldScene(Game* game) {
	setType(Scene::WorldScene);
	setGame(game);
	
	RenderWindow* rw = getGame()->getRenderWindow();

	m_root = G_RootObj();

	ui = std::make_shared<UI>(getGame());
	
	if (!getGame()->getSceneManager()->isRWinit()) {
		getGame()->getSceneManager()->isRWinit(true);

		rw->create(sf::VideoMode(800, 501), "", sf::Style::Titlebar | sf::Style::Close);
		rw->setFramerateLimit(144);
		rw->setVerticalSyncEnabled(true);

		ImGui::SFML::Init(*rw);
	}
}

void WorldScene::drawMain(Time delta_t)
{
	ImGui::ShowDemoWindow();

	ui->drawVersion();
	//ui->drawBackgroundSpaceCircleEffect(delta_t.asSeconds());
}

void WorldScene::onProcess() {

}

void WorldScene::onDraw() {
	RenderWindow* rw = getGame()->getRenderWindow();
	NetworkManager* nm = getGame()->getNetworkManager();

	Time delta_t = deltaClock.restart();
	ImGui::SFML::Update(*rw, delta_t);
	rw->clear();

	// Отрисовка всех объектов из рута
	m_root.draw(*rw);

	switch (getPage()) {

	case main:	drawMain(delta_t);	break;

	}

	ImGui::SFML::Render(*rw);
	rw->display();
}

void WorldScene::onUpdateEvents() {
	sf::Event event;
	RenderWindow* rw = getGame()->getRenderWindow();
	while (rw->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*rw, event);
		if (event.type == sf::Event::Closed)
			rw->close();
	}
}