#include "NetworkManager.h"
#include "mGame/Game.h"

NetworkManager::NetworkManager(Game* game) {
	m_game = game;
	last_c_ping = system_clock::now();
	getGame()->getLogger()->info(L"Инициализация NetworkManager успешна.");
}
void NetworkManager::process()
{
	// ПРОИЗОШЛА ОШИБКА СОЕДИНЕНИЯ - ПЕРЕХОДИМ В ОКНО ПОДКЛЮЧЕНИЯ
	if (getGame()->getSceneManager()->getCurrentScene()->getType() != Scene::LauncherScene and get_status() == connection_failed) {
		getGame()->getSceneManager()->setScene(Scene::LauncherScene, L"Morland Launcher");
	}
	// ЕСЛИ МЫ НЕ ПОДКЛЮЧЕНЫ К СЕРВЕРУ - ПРОПУСК ЭТОЙ ФУНКЦИИ
	if (get_status() != connection_done)	return;


	// ОТПРАВКА ПИНГА НА СЕРВЕР РАЗ В TCP_C_PING_DELAY
	if (duration_cast<std::chrono::milliseconds>(system_clock::now() - last_c_ping).count() >= TCP_C_PING_DELAY) 
		this->send_ping();
}

/*
* Отправка пакета с проверкой на валидную отправку
*/
void NetworkManager::send_packet(enjPacket p)
{
	while (1) {
		Socket::Status status = getTCP()->send(p);
		if (status == Socket::Status::Done) return;
		else if (status == Socket::Status::Partial) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue; }
		else if (status == Socket::Status::Disconnected or status == Socket::Error) {
			set_status(NetworkManager::Status::connection_failed);
			return;
		}
	}
}

void NetworkManager::send_ping() {
	last_c_ping = system_clock::now();

	enjPacket p;
	p << (sf::Uint16)C_PING;
	send_packet(p);
}
void NetworkManager::set_connection_data(std::string ip, unsigned short port) {
	last_ip = ip;
	last_port = port;
}
void NetworkManager::get_connection_data(std::string& ip, unsigned short& port) {
	ip = last_ip;
	port = last_port;
}

void NetworkManager::connect_server() {
	m_tcp = new TcpSocket();
	while (1) {
		if (last_try_time == 0) {
			last_try_time = time(0);
		}
		// Проверка на максимально допустимый порог подключений
		if (try_connect_count > TCP_MAX_TRY_TO_CONNECT) {
			// Прекращаем попытки подключения через этот статус
			set_status(connection_failed);
			last_try_time = 0;
			// Обнуляем число попыток для будущего подключения
			try_connect_count = 0;
			break;
		}

		Socket::Status status = Socket::Status::Error;

		if (get_status() == Status::pending_connection) {
			status = m_tcp->connect(last_ip, last_port);
		}

		if (status == Socket::Status::Done) {
			set_status(Status::connection_done);
			break;
		}

		if (time(0) - last_try_time >= TCP_MAX_SECS_TO_CONNECT) {
			last_try_time = time(0);
			try_connect_count++;
		}
	}
}