#include "LauncherScene.h"
#include <format>

#include "mGame/Managers/Scene/SceneManager.h"
#include "Utils/Utils.h"

#include "mGame/Game.h"

using namespace Utils::encoding;

LauncherScene::LauncherScene(Game* game) {
	setType(Scene::LauncherScene);
	setGame(game);

	RenderWindow* rw = getGame()->getRenderWindow();

	m_root = RootObj();
	
	ui = std::make_shared<UI>( getGame() );

	rw->create(sf::VideoMode(800, 500), "", sf::Style::Titlebar | sf::Style::Close);
	rw->setFramerateLimit(144);
	
	ImGui::SFML::Init(*rw);
}

void LauncherScene::onProcess() {
	NetworkManager* nm = getGame()->getNetworkManager();
	RenderWindow* rw = getGame()->getRenderWindow();

	if (nm->get_status() == NetworkManager::connection_done) {
		// Мы успешно подключились к серверу
		// Переходим к синхронизации с миром
		
		if (t_connect_serv) { 
			t_connect_serv->wait();
			delete t_connect_serv; 
		}	// Освобождаем поток из памяти

		// УБРАТЬ ПОСЛЕ ТЕСТОВ!!!!
		nm->set_status(NetworkManager::None);
		// УБРАТЬ ПОСЛЕ ТЕСТОВ!!!!

		//getGame()->getSceneManager()->setScene(Scene::WorldScene);
		//getGame()->getSceneManager()->setSceneName(L"Morland");
		return;
	} 
	if(nm->get_status() == NetworkManager::connection_failed) {
		// Ошибка при подключении, скорее всего были исчерпаны попытки подключения
		return;
	}
}

void LauncherScene::onDraw() {
	RenderWindow* rw = getGame()->getRenderWindow();
	NetworkManager* nm = getGame()->getNetworkManager();

	ImGui::SFML::Update(*rw, deltaClock.restart());
	rw->clear();
	
	// Отрисовка всех объектов из рута
	m_root.draw(*rw);

	// Панель подключения
	{
		ImGui::Begin("connection_menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowPos(ImVec2(ImGui::GetWindowWidth() / 2, rw->getSize().y - ImGui::GetWindowHeight()));

		std::string str_error = "";

		if (nm->get_status() == NetworkManager::pending_connection and nm->get_connection_trys() > 0) {
			str_error = std::format("{} ({})", 
				to_utf8(L"Connection error").c_str(), 
				nm->get_connection_trys()
			);
		}
		if (nm->get_status() == NetworkManager::connection_failed) {
			str_error = to_utf8(L"Server is unavailable.").c_str();
		}
		if (nm->get_status() == NetworkManager::connection_done) {
			str_error = to_utf8(L"Connected to Server!").c_str();
		}
		ImGui::Text( str_error.c_str() );

		static char ip[256] = "";
		ImGui::InputText(":IP", ip, IM_ARRAYSIZE(ip));
		
		ImGui::SameLine();

		// Логика подключения к серверу
		std::string buf_ip = std::string(ip);
		if (ImGui::Button("Connect") and buf_ip.size() > 0 
			&& 
			nm->get_status() != NetworkManager::pending_connection
		) {

			std::string str_ip;
			std::string str_port;
			int port = NULL;
			int del_pos = buf_ip.find(':');

			str_ip = buf_ip.substr(0, del_pos);

			if (del_pos != std::string::npos) {
				str_port = buf_ip.substr(del_pos + 1, buf_ip.size());
				port = atoi(str_port.c_str());
			}
			else port = SERVER_DEF_PORT;

			// Установка данных для подключения
			nm->set_connection_data(str_ip, port);
			// Установка статуса на попытку подключения
			nm->set_status(NetworkManager::pending_connection);
			// Создание потока подключения к серверу
			t_connect_serv = new Thread(&NetworkManager::connect_server, nm);
			t_connect_serv->launch();
		}

		ImGui::SameLine();

		// Логика создания сервера
		if (ImGui::Button("Create server")) {

		}

		ImGui::End();
	}

	ui->drawVersion();

	ImGui::SFML::Render(*rw);
	rw->display();
}

void LauncherScene::onUpdateEvents() {
	sf::Event event;
	RenderWindow* rw = getGame()->getRenderWindow();
	while (rw->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*rw, event);
		if (event.type == sf::Event::Closed)
			rw->close();
	}
}