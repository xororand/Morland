#pragma once
#include "version.h"
#include "defines.h"
#include "mGame/Managers/Network/defines.h"

/*
* Создает автозаголовок с версией билда и конец пакета
*/
class enjPacket : public sf::Packet
{
private:
    bool m_is_valid = true;
    std::vector<sf::Uint8> sentdata;
public:
    bool verife_packet(const void* data, size_t size);

    bool isValid()          { return m_is_valid; }

    virtual const void* onSend(std::size_t& size);
    virtual void onReceive(const void* data, std::size_t size);
};

