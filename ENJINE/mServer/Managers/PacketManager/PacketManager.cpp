#include "PacketManager.h"
#include "mServer/Server.h"

bool PacketManager::verife_packet(const void* data, size_t size)
{
	// TODO: ������� ������ �� ������ �������� �������

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

	// �������� �� ������ � �����
	if (data == NULL or size == NULL) return FALSE;

	// �������� ������ � �����
	if(h_b != P_HEAD and ((const char*)data)[size] != P_END) return FALSE;

	// �������� ������ ������
	if (majver != MAJOR_VER or minver != MINOR_VER or pathver != PATCH_VER) return FALSE;

	return TRUE;
}

PacketManager::PacketManager(Server* serv) {
	this->m_server = serv;
}

int PacketManager::process_packet(Player* pl)
{
	// ����� �� ����������
	if (pl == NULL) return Socket::Status::Error;

	enjPacket p;
	Socket::Status tcp_status = pl->getTcp()->receive(p);
	if (tcp_status != Socket::Status::Done) return tcp_status;
	
	// �������� ������
	if (verife_packet(p.getData(), p.getDataSize()) == FALSE) return Socket::Status::Error;
	
	sf::Uint8 h_b;
	sf::Uint8 majver;
	sf::Uint8 minver;
	sf::Uint8 pathver;
	sf::Uint16 c_p;
	// �������� �� ������� ������� ������
	if( !(p >> h_b >> majver >> minver >> pathver >> c_p) ) return Socket::Status::Error;

	// TODO: ����� ������� ������� ������� = �������
	switch (c_p) {
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
	pl->setPingMS(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pl->getLastPing_tp()).count());
	pl->setLastPing_tp(std::chrono::system_clock::now());
}
