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
		if (status == Socket::Status::Done) return;
		else if (status == Socket::Status::Partial) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue; }
		else if (status == Socket::Status::Disconnected or status == Socket::Error) {
			peer->setStatus(Peer::disconnected);
			return;
		}
	}
}

int PacketManager::process_packet(Peer* peer)
{
	// œ»– Õ≈ —”Ÿ≈—“¬”≈“
	if (peer == nullptr) return Socket::Status::Error;

	enjPacket p;
	Socket::Status tcp_status = peer->getTcp()->receive(p);
	if (tcp_status != Socket::Status::Done) return tcp_status;
	
	// œ¿ ≈“ Õ≈ œ–Œÿ≈À œ–Œ¬≈– ” œ–Œ“Œ ŒÀ¿
	if (!p.isValid()) return Socket::Status::Error;

	sf::Uint8 p_c = -1;

	p >> p_c;

	// TODO: ¡ŒÀ≈≈ ”ƒŒ¡Õ”ﬁ ¬€¡Œ– ” ÍÓÏ‡Ì‰‡ = ÙÛÌÍˆËˇ
	switch ( p_c ) {

	case C_PING: c_ping(peer, p.getData(), p.getDataSize()); break;
	case C_LOGIN: c_login(peer, p.getData(), p.getDataSize()); break;

	default:
		getServer()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}", 
				Utils::encoding::to_wide( peer->getTcp()->getRemoteAddress().toString() ), 
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
void PacketManager::c_ping(Peer* peer, const void* data, size_t size) {
	long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - peer->getWhenPingSent()).count();
	peer->setPingMS(ms);

	// —“¿¬»Ã —“¿“”— ◊“Œ  À»≈Õ“ œ–Œÿ≈À œ–Œ¬≈– ” œ–Œ“Œ ŒÀ¿
	if (peer->getStatus() == Peer::not_verifed) {
		getServer()->getLogger()->info(std::format(L"[p+] {}:{} verifed protocol",
			Utils::encoding::to_wide(peer->getTcp()->getRemoteAddress().toString()),
			peer->getTcp()->getRemotePort()).c_str());
		peer->setStatus(Peer::verifed);
	}
}

// TODO: ¿¬“Œ–»«¿÷»ﬂ ﬁ«≈–¿
void PacketManager::c_login(Peer* peer) {

}
void PacketManager::c_login(Peer* peer, const void* data, size_t size) {

}
