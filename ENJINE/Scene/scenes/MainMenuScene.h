#pragma once
#include <Scene/Scene.h>
#include <Network/NetworkManager.h>

class MainMenuScene : public Scene
{
public:
	MainMenuScene(RenderWindow* window, NetworkManager* netmg) {
		setType(Scene::MainMenuScene);

		m_window = window;
		m_netmg = netmg;
		m_root = RootObj();

		m_window->create(sf::VideoMode::getDesktopMode(), "Morland", sf::Style::Fullscreen);
		m_window->setFramerateLimit(144);

		ImGui::SFML::Init(*m_window);
	}

	void onProcess() {

	}

	void onDraw() {
		ImGui::SFML::Update(*m_window, deltaClock.restart());
		m_window->clear();

		// Отрисовка всех объектов из рута
		m_root.draw(*m_window);

		ImGui::SFML::Render(*m_window);
		m_window->display();
	}

	void omUpdateEvents() {
		sf::Event event;
		while (m_window->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(*m_window, event);
			if (event.type == sf::Event::Closed)
				m_window->close();
		}
	}

};

