#pragma once
#include "SFML/Graphics.hpp"
#include "Scene/scenes/LauncherScene.h"
#include "Scene/scenes/WorldScene.h"
#include "Network/NetworkManager.h"
#include <Scene/SceneManager.h>

class Game {
private:
	RenderWindow m_window;
	NetworkManager* m_netmg;
	SceneManager* m_scenemg;
public:
	Game() {
		// INIT Managers
		m_netmg = new NetworkManager();
		m_scenemg = new SceneManager(&m_window, m_netmg);

		m_scenemg->setScene(Scene::LauncherScene);
	}

	void receiveData() {
		// TODO: Если подключились - сделать смену сцены
	}

	int run() {
		/*
		sf::Texture texture;
		if (!texture.loadFromFile("../ENJINE/Textures/Characters/green_character.png"))
		{
			exit(1);
		}

		sf::Sprite sprite;
		sprite.setTexture(texture);

		std::shared_ptr<Object> spriteObj = std::make_shared<SpriteObj>(sprite);


		std::shared_ptr<Object> spriteObj2 = std::make_shared<SpriteObj>(sprite);

		spriteObj2.get()->move(Vector2f(10, 10));

		spriteObj.get()->addObject(spriteObj2);

		m_root.addObject(spriteObj);

		HealthComponent c_topack = HealthComponent();
		c_topack.current_health = 80;

		HealthComponent c_tounpack;

		Packet packet;
		packet << c_topack;

		packet >> c_tounpack;

		Logger::info( std::format(L"{}", c_tounpack.current_health).c_str() );*/

		while (m_window.isOpen())
		{
			// Прием данных с сервера
			receiveData();
			// Процессим сцену
			m_scenemg->processScene();
		}

		ImGui::SFML::Shutdown();
		return 0;
	}
};

