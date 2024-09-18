#include "Object/SpriteObj/SpriteObj.h"
#include <Object/RootObj/RootObj.h>

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
		m_window.clear();
		
		m_root.draw(m_window);

		ImGui::SFML::Render(m_window);
		m_window.display();
	}

	int run() {
        m_window.create(sf::VideoMode(1920, 1080), "Morland");
		m_window.setFramerateLimit(144);

		!ImGui::SFML::Init(m_window);

		sf::Texture texture;
		if (!texture.loadFromFile("../ENJINE/Textures/Characters/green_character.png"))
		{
			exit(1);
		}

		sf::Sprite sprite;
		sprite.setTexture(texture);

		std::shared_ptr<Object> spriteObj = std::make_shared<SpriteObj>(sprite);
		spriteObj.get()->setOrigin(Vector2f(64, 64));

		std::shared_ptr<Object> spriteObj2 = std::make_shared<SpriteObj>(sprite);

		spriteObj2.get()->move(Vector2f(10, 10));

		spriteObj.get()->addObject(spriteObj2);

		m_root.addObject(spriteObj);



        while (m_window.isOpen())
        {
			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_W))
				spriteObj.get()->rotate(1.0);
			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_A))
				spriteObj.get()->move(Vector2f(-1, 0));
			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_S))
				spriteObj.get()->move(Vector2f(0, 1));
			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_D))
				spriteObj.get()->move(Vector2f(1, 0));
            updateEvents();
			ImGui::SFML::Update(m_window, deltaClock.restart());
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