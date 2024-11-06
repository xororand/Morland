#include "Game.h"

Game::Game() {
	setlocale(LC_ALL, "RU");
	// INIT Managers
	Logger::info(L"Инициализация менеджеров...");

	m_window = new RenderWindow();

	m_texturemg = new TextureManager(this);
	m_texturemg->loadTexturesFromRootResources();

	m_netmg = new NetworkManager(this);

	m_scenemg = new SceneManager(this);
	{ // Установка начальной сцены
		m_scenemg->setScene(Scene::LauncherScene);
		m_scenemg->setSceneName(L"Morland Launcher");
	}

	if (m_netmg && m_scenemg && m_texturemg)
		Logger::info(L"Инициализация менеджеров прошла успешно!");
	else {
		Logger::info(L"Инициализация менеджеров потерпела крах !!!");
		exit(1);
	}
}

int Game::run() {
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
	while (m_window->isOpen())
	{
		// Процессим сцену
		m_scenemg->processScene();
	}

	ImGui::SFML::Shutdown();
	return 0;
}