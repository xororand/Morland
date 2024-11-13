#include "PacketManager.h"
#include "mServer/Server.h"

bool PacketManager::verife_packet(const void* data, size_t size)
{
	// TODO: ÏÐÈ×ÈÍÀ ÎÒÊÀÇÀ ÎÒ ÏÀÊÅÒÀ ÏÐÈÑËÀÒÜ ÊËÈÅÍÒÓ

	/*
	* PACKET
	* b0		- P_HEAD
	* b1		- MAJOR_VER
	* b2		- MINOR_VER
	* b3		- PATCH_VER
	* last	- P_END
	*/

	Packet p; p.append(data, size);

	sf::Uint8 h_b;
	sf::Uint8 majver;
	sf::Uint8 minver;
	sf::Uint8 pathver;

	p >> h_b >> majver >> minver >> pathver;

	// ÏÐÎÂÅÐÊÀ ÍÀ ÑÑÛËÊÓ È ÄËÈÍÓ
	if (data == NULL or size == NULL) return FALSE;

	// ÏÐÎÂÅÐÊÀ ÍÀ×ÀËÀ È ÊÎÍÖÀ
	if(h_b != P_HEAD and ((const char*)data)[size] != P_END) return FALSE;

	// ÏÐÎÂÅÐÊÀ ÂÅÐÑÈÈ ÏÀÊÅÒÀ
	if (majver != MAJOR_VER or minver != MINOR_VER or pathver != PATCH_VER) return FALSE;

	return TRUE;
}

PacketManager::PacketManager(Server* serv) {
	this->m_server = serv;
}

int PacketManager::process_packet(Player* pl)
{
	// ÈÃÐÎÊ ÍÅ ÑÓÙÅÑÒÂÓÅÒ
	if (pl == NULL) return Socket::Status::Error;

	enjPacket p;
	Socket::Status tcp_status = pl->getTcp()->receive(p);
	if (tcp_status != Socket::Status::Done) return tcp_status;
	
	// ÏÀÊÅÒ ÍÅ ÏÐÎØÅË ÏÐÎÂÅÐÊÓ ÏÐÎÒÎÊÎËÀ
	if (!p.isValid()) return Socket::Status::Error;

	// TODO: ÁÎËÅÅ ÓÄÎÁÍÓÞ ÂÛÁÎÐÊÓ êîìàíäà = ôóíêöèÿ
	switch (p.getCommand()) {
	case C_PING: c_ping(pl, p.getData(), p.getDataSize()); break;
	default:
		getServer()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}", 
				Utils::encoding::to_wide(pl->getTcp()->getRemoteAddress().toString()), 
				pl->getTcp()->getLocalPort()
			).c_str()
		);
		break;
	}
	return tcp_status;
}

void PacketManager::c_ping(Player* pl, const void* data, size_t size)
{
	// ÑÒÀÂÈÌ ÑÒÀÒÓÑ ×ÒÎ ÊËÈÅÍÒ ÏÐÎØÅË ÏÐÎÂÅÐÊÓ ÏÐÎÒÎÊÎËÀ
	if (pl->getStatus() == Player::not_verifed) pl->setStatus(Player::verifed);

	pl->setPingMS(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pl->getLastPing_tp()).count());
	pl->setLastPing_tp(std::chrono::system_clock::now());
}
