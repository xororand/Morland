#include "NetworkManager.h"
#include "mGame/Game.h"

NetworkManager::NetworkManager(Game* game) {
	m_game = game;
	last_c_ping = system_clock::now();
	getGame()->getLogger()->info(L"Èíèöèàëèçàöèÿ NetworkManager óñïåøíà.");
}
void NetworkManager::process()
{
	// ÏÐÎÈÇÎØËÀ ÎØÈÁÊÀ ÑÎÅÄÈÍÅÍÈß - ÏÅÐÅÕÎÄÈÌ Â ÎÊÍÎ ÏÎÄÊËÞ×ÅÍÈß
	if (getGame()->getSceneManager()->getCurrentScene()->getType() != Scene::LauncherScene and get_status() == connection_failed) {
		getGame()->getSceneManager()->setScene(Scene::LauncherScene, L"Morland Launcher");
	}
	// ÅÑËÈ ÌÛ ÍÅ ÏÎÄÊËÞ×ÅÍÛ Ê ÑÅÐÂÅÐÓ - ÏÐÎÏÓÑÊ ÝÒÎÉ ÔÓÍÊÖÈÈ
	if (get_status() != connection_done)	return;

	// ÏÎËÓ×ÅÍÈÅ ÏÀÊÅÒÎÂ Ñ ÑÅÐÂÅÐÀ
	process_packet();
}

void NetworkManager::process_packet()
{
	enjPacket p;
	Socket::Status tcp_status = getTCP()->receive(p);
	if (p.isValid() == false) {
		set_status(connection_failed);
		return;
	}
	if (tcp_status == Socket::Status::Disconnected or tcp_status == Socket::Status::Error) {
		set_status(connection_failed);
		return;
	}

	if (tcp_status != Socket::Status::Done) return;

	sf::Uint8 c_p = -1;
	p >> c_p;
	switch (c_p) {
	case C_PING: c_ping(); break;
	default:
		getGame()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}",
				Utils::encoding::to_wide(getTCP()->getRemoteAddress().toString()),
				getTCP()->getLocalPort()
			).c_str()
		);
		break;
	}
}

/*
* Îòïðàâêà ïàêåòà ñ ïðîâåðêîé íà âàëèäíóþ îòïðàâêó
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

void NetworkManager::c_ping() {
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
		// Ïðîâåðêà íà ìàêñèìàëüíî äîïóñòèìûé ïîðîã ïîäêëþ÷åíèé
		if (try_connect_count > TCP_MAX_TRY_TO_CONNECT) {
			// Ïðåêðàùàåì ïîïûòêè ïîäêëþ÷åíèÿ ÷åðåç ýòîò ñòàòóñ
			set_status(connection_failed);
			last_try_time = 0;
			// Îáíóëÿåì ÷èñëî ïîïûòîê äëÿ áóäóùåãî ïîäêëþ÷åíèÿ
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