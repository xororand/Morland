#include "NetworkManager.h"
#include "mGame/Game.h"
#include "mServer/Managers/DBManager/DBManager.h"

NetworkManager::NetworkManager(Game* game) {
	m_game = game;
	last_c_ping = system_clock::now();
	getGame()->getLogger()->info(L"Èíèöèàëèçàöèÿ NetworkManager óñïåøíà.");
}
void NetworkManager::process() {
	// ÏÐÎÈÇÎØËÀ ÎØÈÁÊÀ ÑÎÅÄÈÍÅÍÈß - ÏÅÐÅÕÎÄÈÌ Â ÎÊÍÎ ÏÎÄÊËÞ×ÅÍÈß
	if (getGame()->getSceneManager()->getCurrentScene()->getType() != Scene::LauncherScene and getStatus() == connection_failed) {
		setLastErrMsg(L"");
		getGame()->getSceneManager()->setScene(Scene::LauncherScene, L"Morland Launcher");
	}
	// ÅÑËÈ ÌÛ ÍÅ ÏÎÄÊËÞ×ÅÍÛ Ê ÑÅÐÂÅÐÓ - ÏÐÎÏÓÑÊ ÝÒÎÉ ÔÓÍÊÖÈÈ
	if (getStatus() != connection_done)	return;

	// ÏÎËÓ×ÅÍÈÅ ÏÀÊÅÒÎÂ Ñ ÑÅÐÂÅÐÀ
	process_packet();
}

void NetworkManager::process_packet() {
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
	case C_SYNC_READY:		c_sync_ready(p);	break;
	case C_CREATE_PLAYER:	

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
Îòïðàâêà ïàêåòà ñ ñèñòåìîé àâòîìàòè÷åñêîãî äîñûëà â ñëó÷àå åãî ïîòåðè.
Â ñëó÷àå åñëè áûëà îøèáêà ñåòè - ïàêåò íå äîéäåò.
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
	p >> status;
	if (status == P_SUCCESS) {
		is_registered_this_session = true;
		setLastErrMsg(L"Registration is success!");
	}
	else {
		setLastErrMsg(L"Registration failed.");
	}
}
void NetworkManager::c_register_user(std::wstring username, std::wstring password, std::wstring password2)
{
	if (is_registered_this_session) {
		setLastErrMsg(L"You have already registered this session!");
		return;
	}
	if (username.size() > 256) { 
		setLastErrMsg(L"Username len is too much!");
		return; 
	}
	if (password.size() > 1024) {
		setLastErrMsg(L"Password len is too much!");
		return;
	}
	if (password != password2) {
		setLastErrMsg(L"Passwords are not the same!");
		return;
	}
	if (!DBManager::isStringAllowed(username)) {
		setLastErrMsg(L"Impossible chars in username!");
		return;
	}
	enjPacket p;
	p << (sf::Uint16)C_REGISTER_USER << username << password;
	send_packet(p);
}

void NetworkManager::c_login_user(enjPacket& p)
{
	sf::Uint8 status = 0;
	p >> status;
	if (status == P_SUCCESS) {
		getGame()->getSceneManager()->setScene(Scene::WorldScene, L"Morland World");
	}
	else {
		setLastErrMsg(L"Authentication failed.");
	}
}
void NetworkManager::c_login_user(std::wstring username, std::wstring password) {
	if (username.size() > 256 || password.size() > 1024) return;
	enjPacket p;
	p << (sf::Uint16)C_LOGIN_USER << username << password;
	send_packet(p);
}

void NetworkManager::c_sync_ready()
{
	enjPacket p;
	sf::Uint8 status = P_SUCCESS;
	p << (sf::Uint16)C_SYNC_READY << status;
	send_packet(p);
}
void NetworkManager::c_sync_ready(enjPacket& p)
{
	sf::Uint8 status = P_FAIL;
	p >> status;
	if (status == P_SUCCESS)	is_sync = true;
	else						is_sync = false;
}

void NetworkManager::c_create_player(enjPacket& p) {
	sf::Uint16 pID = -1;
	std::wstring username = L"";
	p >> pID >> username;
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

