#include "PacketManager.h"
#include "PacketManager.h"
#include "PacketManager.h"
#include "PacketManager.h"
#include "mServer/Server.h"

bool PacketManager::verife_packet(Packet p)
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

	const BYTE* data = (const BYTE*)p.getData();
	size_t		size = p.getDataSize();

	// ÏÐÎÂÅÐÊÀ ÍÀ ÑÑÛËÊÓ È ÄËÈÍÓ
	if (data == NULL or size == NULL) return FALSE;

	// ÏÐÎÂÅÐÊÀ ÍÀ×ÀËÀ È ÊÎÍÖÀ
	if( data[0] != P_HEAD or data[size] != P_END) return FALSE;

	// ÏÐÎÂÅÐÊÀ ÂÅÐÑÈÈ
	if (data[1] != MAJOR_VER or data[2] != MINOR_VER or data[3] != PATCH_VER) return FALSE;

	return TRUE;
}

PacketManager::PacketManager(Server* serv) {
	this->m_server = serv;
}

void PacketManager::process_packet(Player* pl, Packet p)
{
	// ÈÃÐÎÊ ÍÅ ÑÓÙÅÑÒÂÓÅÒ
	if (pl == NULL) return;
	// ÏÐÎÂÅÐÊÀ ÏÀÊÅÒÀ
	if (verife_packet(p) == FALSE) return;
	Int8 h_b;
	Int8 majver;
	Int8 minver;
	Int8 pathver;
	Int16 c_p;

	p >> h_b >> majver >> minver >> pathver >> c_p;

	switch (c_p) {
	case C_PING:
		c_ping(pl, p);
		break;
	default:
		getServer()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}", 
				pl->getTcp()->getRemoteAddress().toString(), 
				pl->getTcp()->getLocalPort()).c_str()
		);
	}
}

void PacketManager::c_ping(Player* pl, Packet p)
{
	std::chrono::system_clock::now();
}
