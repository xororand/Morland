#pragma once
#include <SFML/Network.hpp>

using namespace sf;

#define SERVER_DEF_PORT 55055

#define TCP_MAX_SECS_TO_CONNECT 5
#define TCP_MAX_TRY_TO_CONNECT 2

#define PACKET_HEAD 0xAC
#define PACKET_END 0xDC