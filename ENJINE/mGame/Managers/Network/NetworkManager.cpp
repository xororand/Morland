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
	if (getGame()->getSceneManager()->getCurrentScene()->getType() != Scene::LauncherScene and getStatus() == connection_failed) {
		setLastErrMsg(L"");
		isAuthed(false);
		getGame()->getSceneManager()->setScene(Scene::LauncherScene, L"Morland Launcher");
	}
	// ÅÑËÈ ÌÛ ÍÅ ÏÎÄÊËÞ×ÅÍÛ Ê ÑÅÐÂÅÐÓ - ÏÐÎÏÓÑÊ ÝÒÎÉ ÔÓÍÊÖÈÈ
	if (getStatus() != connection_done)	return;

	// ÏÎËÓ×ÅÍÈÅ ÏÀÊÅÒÎÂ Ñ ÑÅÐÂÅÐÀ
	process_packet();
}

void NetworkManager::process_packet()
{
	enjPacket p;
	Socket::Status tcp_status = getTCP()->receive(p);
	if (p.isValid() == false) {
		setStatus(connection_failed);
		return;
	}
	if (tcp_status == Socket::Status::Disconnected or tcp_status == Socket::Status::Error) {
		setStatus(connection_failed);
		return;
	}

	if (tcp_status != Socket::Status::Done) return;

	sf::Uint16 c_p = 0;
	p >> c_p;
	switch (c_p) {
	case C_PING:			c_ping();			break;
	case C_REGISTER_USER:	c_register_user(p); break;
	case C_LOGIN_USER:		c_login_user(p);	break;
	default:
		getGame()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}",
				Utils::encoding::to_multibytes(getTCP()->getRemoteAddress().toString()),
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
			setStatus(NetworkManager::Status::connection_failed);
			return;
		}
	}
}

void NetworkManager::disconnect()
{
	getTCP()->disconnect();
	setStatus(self_disconnected);
}

void NetworkManager::c_ping() {
	enjPacket p;
	p << (sf::Uint16)C_PING;
	send_packet(p);
}

void NetworkManager::c_register_user(enjPacket& p)
{
	sf::Uint8 status = 0;
	p << status;
	if (status == P_SUCCESS) {
		is_auth = true;
	}
	else {
		// 
	}
}
void NetworkManager::c_login_user(enjPacket& p)
{
	sf::Uint8 status = 0;
	p >> status;
	if (status == P_SUCCESS) {
		is_auth = true;
	}
	else {
		setLastErrMsg(L"Authentication failed.");
		is_auth = false;
	}
}
void NetworkManager::c_login_user(std::wstring username, std::string password)
{
	if (username.size() > 256 || password.size() > 1024) return;
	enjPacket p;
	p << (sf::Uint16)C_LOGIN_USER << username << password;
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
			setStatus(connection_failed);
			last_try_time = 0;
			// Îáíóëÿåì ÷èñëî ïîïûòîê äëÿ áóäóùåãî ïîäêëþ÷åíèÿ
			try_connect_count = 0;
			break;
		}

		Socket::Status status = Socket::Status::Error;

		if (getStatus() == Status::pending_connection) {
			status = m_tcp->connect(last_ip, last_port);
		}

		if (status == Socket::Status::Done) {
			setStatus(Status::connection_done);
			break;
		}

		if (time(0) - last_try_time >= TCP_MAX_SECS_TO_CONNECT) {
			last_try_time = time(0);
			try_connect_count++;
		}
	}
}

