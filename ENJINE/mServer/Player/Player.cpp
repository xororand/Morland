#include "Player.h"
#include "mServer/Server.h"

Player::Player(size_t idx, Server* ptr_serv, TcpSocket* tcp) {
	this->idx = idx;

	time(&first_connect_t);
	time(&last_packet_t);
	last_ping_ms = std::chrono::system_clock::now();

	m_server = ptr_serv;
	m_tcp = tcp;
	m_tcp->setBlocking(false);

	setStatus(status::not_verifed);

	getServer()->getLogger()->info(std::format(L"[p+] {}:{} New connection...", 
		Utils::encoding::to_wide(tcp->getRemoteAddress().toString()), 
		tcp->getRemotePort() ).c_str() );
}

Player::~Player() {
	delete m_tcp;
}

// Œ·‡·ÓÚÍ‡ ÓÚ‰ÂÎ¸ÌÓ„Ó Ë„ÓÍ‡ ‚ 1 “»  —Â‚Â‡
void Player::process() {
	Server* serv = getServer();

	time_t timeout = time(0) - last_packet_t;
	// “¿…Ã¿”“ »—“≈ 
	if (timeout > MAX_PLAYER_TIMEOUT)	setStatus(status::disconnected); 
	// —À»ÿ ŒÃ ¡ŒÀ‹ÿŒ… œ»Õ√
	if(ping_ms >= MAX_PLAYER_PING)		setStatus(status::disconnected);
	// —À»ÿ ŒÃ ¡ŒÀ‹ÿŒ… “¿…Ã¿”“ œ¿ ≈“Œ¬ - Õ¿◊»Õ¿≈Ã Œ“œ–¿¬ ” œ»Õ√¿
	if (timeout >= MAX_PLAYER_TIMEOUT / 2) serv->ping_player(this->getID()); 
	// Œ“ Àﬁ◊¿≈Ã œŒ œ–»◊»Õ≈ ¬€—Œ Œ√Œ “¿…Ã¿”“¿ œ¿ ≈“Œ¬
	if (this->getStatus() == Player::status::disconnected) {
		getServer()->disconnect_player(this->getID());
		return;
	}

	// œ–»Õ»Ã¿≈Ã œ¿ ≈“€
	Packet p;
	Socket::Status tcp_status = getTcp()->receive(p);

	// Œ“œ–¿¬Àﬂ≈Ã œ¿ ≈“ ¬ Ã≈Õ≈ƒ∆≈– œ¿ ≈“Œ¬
	if (tcp_status == Socket::Status::Done) {
		last_packet_t = time(0);
		serv->getPacketManager()->process_packet(this, p.getData(), p.getDataSize()); 
	}

	// –¿—◊≈“ œ»Õ√¿
	ping_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - getLastPing_tp()).count();
}

std::wstring Player::to_wstring(status s)
{
	switch (s) {
	case disconnected:	return L"Disconnected";
	case not_verifed:	return L"Not verifed";
	case verifed:		return L"Verifed";
	case logged_in:		return L"Logged in";
	default:			return L"UNK_STR";
	}
	return L"UNK_STR";
}
