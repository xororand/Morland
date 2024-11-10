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

	const BYTE* data_ = (const BYTE*)data;

	// �������� �� ������ � �����
	if (data_ == NULL or size == NULL) return FALSE;

	// �������� ������ � �����
	if(data_[0] != P_HEAD or data_[size] != P_END) return FALSE;

	// �������� ������ ������
	if (data_[1] != MAJOR_VER or data_[2] != MINOR_VER or data_[3] != PATCH_VER) return FALSE;

	return TRUE;
}

PacketManager::PacketManager(Server* serv) {
	this->m_server = serv;
}

void PacketManager::process_packet(Player* pl, const void* data, size_t size)
{
	// ����� �� ����������
	if (pl == NULL) return;
	// �������� ������
	if (verife_packet(pdata) == FALSE) return;
	
	// ������� ����� �� ������
	Packet p; p.append(pdata);
	
	sf::Uint8 h_b;
	sf::Uint8 majver;
	sf::Uint8 minver;
	sf::Uint8 pathver;
	sf::Uint16 c_p;

	p >> h_b >> majver >> minver >> pathver >> c_p;

	// TODO: ����� ������ ���
	switch (c_p) {
	case C_PING: c_ping(pl, pdata); break;
	default:
		getServer()->getLogger()->info(
			std::format(L"UNK Packet Type by {}:{}", 
				Utils::encoding::to_wide(pl->getTcp()->getRemoteAddress().toString()), 
				pl->getTcp()->getLocalPort()
			).c_str()
		);
		break;
	}
}

void PacketManager::c_ping(Player* pl, const void* data, size_t size)
{
	Packet p; p.append(pdata); // TODO:: ���������� ���� �� ���� �����

	pl->setLastPing_tp(std::chrono::system_clock::now());
}
