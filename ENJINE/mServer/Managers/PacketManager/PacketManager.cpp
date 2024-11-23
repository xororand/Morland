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
		if (status == Socket::Status::Done) return; // NOTSURE: âîçìîæíî sleep ëèøíèé
		else if (status == Socket::Status::Partial) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue; }
		else if (status == Socket::Status::Disconnected or status == Socket::Error) {
			peer->setStatus(Peer::disconnected);
			return;
		}
	}
}

int PacketManager::process_packet(Peer* peer) {
	// ÏÈÐ ÍÅ ÑÓÙÅÑÒÂÓÅÒ
	if (peer == nullptr) return Socket::Status::Error;

	enjPacket p;
	Socket::Status tcp_status = peer->getTcp()->receive(p);
	if (tcp_status != Socket::Status::Done) return tcp_status;
	
	// ÏÀÊÅÒ ÍÅ ÏÐÎØÅË ÏÐÎÂÅÐÊÓ ÏÐÎÒÎÊÎËÀ
	if (!p.isValid()) return Socket::Status::Error;

	sf::Uint16 p_c = -1;

	p >> p_c;

	// ÍÓÆÍÎ ÄËß ÑÈÑÒÅÌÛ AFK
	if (p_c != (sf::Uint16)C_PING) peer->setLastNotPingPacketReceive(time(0));

	// TODO: ÁÎËÅÅ ÓÄÎÁÍÓÞ ÂÛÁÎÐÊÓ êîìàíäà = ôóíêöèÿ
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

	// ÑÒÀÂÈÌ ÑÒÀÒÓÑ ×ÒÎ ÊËÈÅÍÒ ÏÐÎØÅË ÏÐÎÂÅÐÊÓ ÏÐÎÒÎÊÎËÀ
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
	p << status; // Îòïðàâëÿåì ñîîáùåíèå îá ðåãèñòðàöèè
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

void PacketManager::c_login_user(Peer* peer, sf::Uint8 status) {
	enjPacket p;
	p << (sf::Uint16)C_LOGIN_USER << status; // Îòïðàâëÿåì ñîîáùåíèå îá àâòîðèçàöèè
	send_packet(peer, p);
}
void PacketManager::c_login_user(Peer* peer, enjPacket& p) {
	// TODO: ÏÐÎÂÅÐÊÀ ÀÂÒÎÐÈÇÀÖÈÎÍÍÛÉ ÄÀÍÍÛÕ, ÅÑËÈ ÕÓÅÂÛÅ - ÎÒÊËÞ×ÀÅÌ Â ÏÈÇÄÓ
	if (peer->getStatus() == Peer::logged_in) return; // Åñëè êëèåíò óæå àâòîðèçîâàí - íàõóé îí øëåò íàì ýòó åáàíèíó?
	
	std::wstring username;
	std::string password;

	if (!(p >> username >> password)) return;

	// Åñëè òàêîé þçåð óæå àâòîðèçîâàí - íå ïðèíèìàåì àâòîðèçàöèþ
	if(getServer()->isUsernameConnected(username)) {
		peer->disconnect();
		return;
	}

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
	peer->setLastNotPingPacketReceive(time(0)); // Îáíóëÿåì ñ÷åò÷èê AFK ñèñòåìû
	peer->setStatus(Peer::status::logged_in);
	peer->setUsername(username);
	

	c_login_user(peer, P_SUCCESS); // Îòïðàâëÿåì ïàêåò êëèåíòó ÷òî îí àâòîðèçîâàí

	log->info(std::format(L"[u+] User {} - {}:{}, has successful login", username, to_wide(ip), port).c_str());
}
