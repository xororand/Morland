#pragma once
#include "version.h"
#include "mServer/defines.h"
#include "mGame/Managers/Network/defines.h"

/*
* Создает автозаголовок с версией билда и конец пакета
*/
class enjPacket : public sf::Packet
{
private:
    bool m_is_valid = true;
    sf::Uint8 m_command = -1;
public:
    bool verife_packet(const void* data, size_t size);

    bool isValid()          { return m_is_valid; }
    sf::Uint8 getCommand()  { return m_command; }

    virtual const void* onSend(std::size_t& size);
    virtual void onReceive(const void* data, std::size_t size);
};

