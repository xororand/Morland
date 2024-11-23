#include "enjPacket.h"

bool enjPacket::verife_packet(const void* data, size_t size)
{
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
	sf::Uint16 c_p;

	p >> h_b >> majver >> minver >> pathver >> c_p;

	// опнбепйю мю яяшкйс х дкхмс
	if (data == NULL or size == NULL) return false;

	// опнбепйю мювюкю х йнмжю
	if (h_b != (sf::Uint8)P_HEAD and (sf::Uint8)((const char*)data)[size] != (sf::Uint8)P_END) return false;

	// опнбепйю бепяхх оюйерю
	if (majver != (sf::Uint8)MAJOR_VER or minver != (sf::Uint8)MINOR_VER or pathver != (sf::Uint8)PATCH_VER) return false;

	return true;
}

const void* enjPacket::onSend(std::size_t& size)
{
    const void* srcData = getData();
    std::size_t srcSize = getDataSize();

    //sentdata = new sf::Uint8[srcSize + 5];

	sentdata.push_back((sf::Uint8)P_HEAD);
	sentdata.push_back((sf::Uint8)MAJOR_VER);
	sentdata.push_back((sf::Uint8)MINOR_VER);
	sentdata.push_back((sf::Uint8)PATCH_VER);

	for (int i = 0; i < srcSize; i++)
		sentdata.push_back((sf::Uint8)((const char*)srcData)[i]);

	sentdata.push_back((sf::Uint8)P_END);
    size = sentdata.size();
    return (const void*)sentdata.data();
}

void enjPacket::onReceive(const void* data, std::size_t size)
{
	if (!verife_packet(data, size)) { 
		m_is_valid = false;
		return; 
	}
	std::string newdata = "";
	for (int i = 4; i < size; i++) newdata += ((const char*)data)[i];

    append(newdata.c_str(), size - 5);
}
