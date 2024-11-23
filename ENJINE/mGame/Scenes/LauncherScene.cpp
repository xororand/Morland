#include "LauncherScene.h"
#include <format>

#include "mGame/Managers/Scene/SceneManager.h"
#include "Utils/Utils.h"

#include "mGame/Game.h"

using namespace Utils::encoding;

LauncherScene::LauncherScene(Game* game) {
	setType(Scene::LauncherScene);
	this->setPage(main);
	setGame(game);

	RenderWindow* rw = getGame()->getRenderWindow();

	m_root = G_RootObj();
	
	ui = std::make_shared<UI>( getGame() );

	if (!getGame()->getSceneManager()->isRWinit()) {
		getGame()->getSceneManager()->isRWinit(true);

		rw->create(sf::VideoMode(800, 500), "", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		rw->setFramerateLimit(144);
		rw->setVerticalSyncEnabled(true);

		ImGui::SFML::Init(*rw);
	}
}

void LauncherScene::drawMain()
{
	RenderWindow* rw = getGame()->getRenderWindow();
	NetworkManager* nm = getGame()->getNetworkManager();

	// Панель подключения
	if (ImGui::Begin("connection_menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration)) {
		ImGui::SetWindowPos(ImVec2(ImGui::GetWindowWidth() / 2, rw->getSize().y - ImGui::GetWindowHeight()));

		std::string str_error = "";

		if (nm->getStatus() == NetworkManager::pending_connection and nm->get_connection_trys() > 0) {
			str_error = std::format("{} ({})",
				to_utf8(L"Connection error").c_str(),
				nm->get_connection_trys()
			);
		}
		if (nm->getStatus() == NetworkManager::self_disconnected) {
			str_error = to_ancii(L"We have disconnected.");
		}
		if (nm->getStatus() == NetworkManager::connection_failed) {
			str_error = to_ancii(L"Server is unavailable.");
		}
		if (nm->getStatus() == NetworkManager::connection_done) {
			str_error = to_utf8(L"Connected to Server!").c_str();
		}
		
		ImGui::Text(str_error.c_str());

		static char ip[256] = "localhost";
		ImGui::InputText(":IP", ip, IM_ARRAYSIZE(ip));

		ImGui::SameLine();

		// Логика подключения к серверу
		std::string buf_ip = std::string(ip);
		if (ImGui::Button("Connect") and buf_ip.size() > 0
			&&
			nm->getStatus() != NetworkManager::pending_connection
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
			nm->setStatus(NetworkManager::pending_connection);
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
}
void LauncherScene::drawLogin()
{
	RenderWindow* rw = getGame()->getRenderWindow();
	NetworkManager* nm = getGame()->getNetworkManager();
	
	float screen_weight = rw->getSize().x;
	float screen_height = rw->getSize().y;

	ImGui::SetNextWindowSize(ImVec2(screen_weight / 2.84f, screen_height / 3.16f));
	ImGui::SetNextWindowPos(ImVec2(screen_weight / 2 - (screen_weight / 2.84f) / 2, screen_height / 2 - (screen_height / 2.16f) / 5));
	if (ImGui::Begin("login_menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration)) {
		std::wstring welcome_text	= L"Welcome to our server !";
		std::wstring login_text		= L"Login:";
		std::wstring password_text	= L"Password:";
		
		ImGui::SetCursorPosX(ImGui::CalcTextSize(to_ancii(welcome_text)).x / 2.0f - 10.0f);
		ImGui::Text(to_ancii(welcome_text));

		ImGui::Separator();
		std::wstring error_msg = getGame()->getNetworkManager()->getLastErrMsg();
		ImGui::SetCursorPosX(ImGui::CalcTextSize(to_ancii(error_msg)).x / 2.0f - 10.0f);
		ImGui::Text(to_ancii(error_msg));
		if (ImGui::BeginChild(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y))) {
			ImGui::SetCursorPosY(5.0f);
			ImGui::Text(to_ancii(login_text)); ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::CalcTextSize(to_ancii(password_text)).x + 10.0f);
			static char login_input[256] = "";
			ImGui::InputText("##login_input", login_input, IM_ARRAYSIZE(login_input));

			ImGui::Text(to_ancii(password_text)); ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::CalcTextSize(to_ancii(password_text)).x + 10.0f);
			static char password_input[1024] = "";
			ImGui::InputText("##password_input", password_input, IM_ARRAYSIZE(password_input), ImGuiInputTextFlags_Password);

			//ImGui::SetCursorPosY(5.0f);

			ImGui::Separator();

			std::string enter_text = "ENTER";
			std::string back_text = "BACK";

			ImGui::SetCursorPosX(ImGui::CalcTextSize(to_ancii(password_text)).x + 10.0f);
			//ImGui::SetCursorPosX(screen_weight / 2.84f / 2.0f - (ImGui::CalcTextSize(enter_text.c_str()).x + ImGui::CalcTextSize(back_text.c_str()).x));
			if (ImGui::Button(enter_text.c_str(), ImVec2(75.0f, 25.0f))) {
				nm->c_login_user(to_wide(login_input), password_input);
			} ImGui::SameLine();
			if (ImGui::Button(back_text.c_str(), ImVec2(75.0f, 25.0f))) {
				nm->disconnect();
				setPage(main);
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
}
void LauncherScene::drawRegistration()
{
}

void LauncherScene::onProcess() {
	NetworkManager* nm = getGame()->getNetworkManager();
	RenderWindow* rw = getGame()->getRenderWindow();

	if (nm->isAuthed()) {
		getGame()->getSceneManager()->setScene(Scene::WorldScene);
		getGame()->getSceneManager()->setSceneName(L"Morland World");
	}

	if (nm->getStatus() == NetworkManager::connection_done && getPage() == main) {
		// Мы успешно подключились к серверу
		// Переходим к странице логина
		
		if (t_connect_serv) { 
			t_connect_serv->wait();
			delete t_connect_serv; 
		}	// Освобождаем поток из памяти

		setPage(login);
		return;
	} 
	if (nm->getStatus() != NetworkManager::connection_done && getPage() != main) {
		setPage(main);
	}
	if(nm->getStatus() == NetworkManager::connection_failed) {
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

	switch (getPage()) {

	case main:			drawMain();			break;
	case login:			drawLogin();		break;
	case registration:	drawRegistration(); break;

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