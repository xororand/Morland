#include "Player.h"
#include "mServer/Server.h"

Player::Player(size_t idx, Server* ptr_serv, TcpSocket* tcp) {
	this->idx = idx;

	time(&first_connect_t);
	time(&last_packet_t);

	m_server = ptr_serv;
	m_tcp = tcp;
	
	setStatus(status::not_verifed);

	getServer()->getLogger()->info(std::format(L"[p+] {}:{} New connection...", 
		Utils::encoding::to_wide(tcp->getRemoteAddress().toString()), 
		tcp->getRemotePort() ).c_str() );
}

Player::~Player() {
	delete m_tcp;
}

void Player::process() {
	// CHECK TIMEOUT
	time_t timeout = time(0) - last_packet_t;
	if (timeout > MAX_PLAYER_TIMEOUT)
		setStatus(status::disconnected);

}
