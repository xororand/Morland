#include <iostream>
#include <format>

#include "Logger/Logger.h"

#include "Object/SpriteObj/SpriteObj.h"
#include <Object/RootObj/RootObj.h>
#include <Component/HealthComponent.h>

class Game {
private:
	RenderStates m_states;
	RenderWindow m_window;
	RootObj m_root;

	sf::Clock deltaClock;
public:
	Game() { 
		m_root = RootObj();
	}

	void receiveData() {

	}
	void updateEvents() {
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(m_window, event);
			if (event.type == sf::Event::Closed)
				m_window.close();
		}
	}
	void drawAll() {
		ImGui::SFML::Update(m_window, deltaClock.restart());
		m_window.clear();
		
		m_root.draw(m_window);

		ImGui::SFML::Render(m_window);
		m_window.display();
	}

	int run() {
        m_window.create(sf::VideoMode(1920, 1080), "Morland");
		m_window.setFramerateLimit(144);

		ImGui::SFML::Init(m_window);

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
			receiveData();
            updateEvents();
			
            drawAll();
        }

		ImGui::SFML::Shutdown();
        return 0;
	}
};

int main() {
    Game game;

    return game.run();
}