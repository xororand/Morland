#include "NetworkManager.h"
#include "mGame/Game.h"

NetworkManager::NetworkManager(Game* game) {
	m_game = game;

	getGame()->getLogger()->info(L"Инициализация NetworkManager успешна.");
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