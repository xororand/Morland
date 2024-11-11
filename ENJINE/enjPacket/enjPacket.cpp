#include "enjPacket.h"

const void* enjPacket::onSend(std::size_t& size)
{
    const void* srcData = getData();
    std::size_t srcSize = getDataSize();

    sf::Uint8* data = new sf::Uint8[srcSize + 5];

    data[0] = (sf::Uint8)P_HEAD;
    data[1] = (sf::Uint8)MAJOR_VER;
    data[2] = (sf::Uint8)MINOR_VER;
    data[3] = (sf::Uint8)PATCH_VER;

    for (int i = 0; i < srcSize; i++)
        data[i + 4] = ((const char*)srcData)[i];

    data[srcSize + 5] = (sf::Uint8)P_END;

    size = srcSize + 5;
    return (const void*)data;
}
