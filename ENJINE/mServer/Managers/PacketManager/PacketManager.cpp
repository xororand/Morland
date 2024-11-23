#include "PacketManager.h"
#include "mServer/Server.h"

PacketManager::PacketManager(Server* serv) {
	setServer(serv);
	getServer()->getLogger()->info(L"Loading PacketManager done.");
}

void PacketManager::send_packet(Peer* peer, enjPacket p)
{
	while (1) {
		Socket::Status status = peer->getTcp()->send(p);
		if (status == Socket::Status::Done) return; // NOTSURE: ‚ÓÁÏÓÊÌÓ sleep ÎË¯ÌËÈ
		else if (status == Socket::Status::Partial) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue; }
		else if (status == Socket::Status::Disconnected or status == Socket::Error) {
			peer->setStatus(Peer::disconnected);
			return;
		}
	}
}

int PacketManager::process_packet(Peer* peer) {
	// œ»– Õ≈ —”Ÿ≈—“¬”≈“
	if (peer == nullptr) return Socket::Status::Error;

	enjPacket p;
	Socket::Status tcp_status = peer->getTcp()->receive(p);
	if (tcp_status != Socket::Status::Done) return tcp_status;
	
	// œ¿ ≈“ Õ≈ œ–Œÿ≈À œ–Œ¬≈– ” œ–Œ“Œ ŒÀ¿
	if (!p.isValid()) return Socket::Status::Error;

	sf::Uint16 p_c = -1;

	p >> p_c;

	// TODO: ¡ŒÀ≈≈ ”ƒŒ¡Õ”ﬁ ¬€¡Œ– ” ÍÓÏ‡Ì‰‡ = ÙÛÌÍˆËˇ
	switch ( p_c ) {

	case C_PING:			c_ping(peer, p); break;
	case C_REGISTER_USER:	c_register_user(peer, p); break;
	case C_LOGIN_USER:		c_login_user(peer, p); break;

	default:
		getServer()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}", 
				to_wide( peer->getTcp()->getRemoteAddress().toString() ), 
				peer->getTcp()->getLocalPort()
			).c_str()
		);
		peer->addUnkPacket();
		break;
	}
	return tcp_status;
}

void PacketManager::c_ping(Peer* peer) {
	enjPacket p;
	p << C_PING;
	send_packet(peer, p);
	peer->setWhenPingSent(system_clock::now());
}
void PacketManager::c_ping(Peer* peer, enjPacket& p) {
	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - peer->getWhenPingSent()).count();
	peer->setPingMS(ms);

	// —“¿¬»Ã —“¿“”— ◊“Œ  À»≈Õ“ œ–Œÿ≈À œ–Œ¬≈– ” œ–Œ“Œ ŒÀ¿
	if (peer->getStatus() == Peer::not_verifed) {
		getServer()->getLogger()->info(std::format(L"[p+] {}:{} verifed protocol",
			to_wide(peer->getTcp()->getRemoteAddress().toString()),
			peer->getTcp()->getRemotePort()).c_str());
		peer->setStatus(Peer::verifed);
	}
}

// TODO: REGISTER USER
void PacketManager::c_register_user(Peer* peer, sf::Uint8 status) {
	enjPacket p;
	p << status; // ŒÚÔ‡‚ÎˇÂÏ ÒÓÓ·˘ÂÌËÂ Ó· Â„ËÒÚ‡ˆËË
	send_packet(peer, p);
}
void PacketManager::c_register_user(Peer* peer, enjPacket& p) {
	std::wstring username;
	std::wstring password;

	if (!(p >> username >> password)) return;

	DBManager* dbmngr = getServer()->getDBManager();
	Logger* log = getServer()->getLogger();

	DBManager::error er =  dbmngr->add_user(peer, username, password);
	if (er != DBManager::error::success) {
		c_register_user(peer, (sf::Uint8)P_FAIL);
	}

	c_register_user(peer, (sf::Uint8)P_SUCCESS);
	std::string ip = peer->getTcp()->getRemoteAddress().toString();
	auto port = peer->getTcp()->getLocalPort();

	log->info(std::format(L"[u+] User {} - {}:{}, has successful registered", username, to_wide(ip), port).c_str());
}

// TODO: ¿¬“Œ–»«¿÷»ﬂ ﬁ«≈–¿
void PacketManager::c_login_user(Peer* peer, sf::Uint8 status) {
	enjPacket p;
	p << (sf::Uint16)C_LOGIN_USER << status; // ŒÚÔ‡‚ÎˇÂÏ ÒÓÓ·˘ÂÌËÂ Ó· ‡‚ÚÓËÁ‡ˆËË
	send_packet(peer, p);
}
void PacketManager::c_login_user(Peer* peer, enjPacket& p) {
	// TODO: œ–Œ¬≈– ¿ ¿¬“Œ–»«¿÷»ŒÕÕ€… ƒ¿ÕÕ€’, ≈—À» ’”≈¬€≈ - Œ“ Àﬁ◊¿≈Ã ¬ œ»«ƒ”
	if (peer->getStatus() == Peer::logged_in) return; // ≈ÒÎË ÍÎËÂÌÚ ÛÊÂ ‡‚ÚÓËÁÓ‚‡Ì - Ì‡ıÛÈ ÓÌ ¯ÎÂÚ Ì‡Ï ˝ÚÛ Â·‡ÌËÌÛ?
	std::wstring username;
	std::string password;

	if (!(p >> username >> password)) return;

	DBManager* dbmngr = getServer()->getDBManager();
	Logger* log = getServer()->getLogger();

	std::string passhash = Utils::hashing::sha256(password);

	bool is_auth = dbmngr->is_user_auth(username, passhash);
	if (!is_auth) {
		c_login_user(peer, P_FAIL);
		peer->addFailedLogin();
		return;
	}

	std::string ip = peer->getTcp()->getRemoteAddress().toString();
	auto port = peer->getTcp()->getLocalPort();

	dbmngr->set_user_last_ip(username, ip);
	peer->setStatus(Peer::status::logged_in);
	c_login_user(peer, P_SUCCESS); // ŒÚÔ‡‚ÎˇÂÏ Ô‡ÍÂÚ ÍÎËÂÌÚÛ ˜ÚÓ ÓÌ ‡‚ÚÓËÁÓ‚‡Ì

	log->info(std::format(L"[u+] User {} - {}:{}, has successful login", username, to_wide(ip), port).c_str());
}
