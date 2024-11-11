#pragma once
#include "version.h"
#include "mServer/defines.h"
#include "mGame/Managers/Network/defines.h"

/*
* Создает автозаголовок с версией билда и конец пакета
*/
class enjPacket : public sf::Packet
{
    virtual const void* onSend(std::size_t& size);
    virtual void onReceive(const void* data, std::size_t size)
    {
        append(data, size);
    }
};

