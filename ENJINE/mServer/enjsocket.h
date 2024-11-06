#pragma once

#include "mGame/Managers/Network/defines.h"

class Server;

class enjsocket
{
private:
	TcpSocket* m_tcp;
	Server* m_server;
public:
	enjsocket(Server* ptr_serv, TcpSocket* tcp);
	~enjsocket();

	void process();

	Server* getServer() { return m_server; }
	TcpSocket* getTcp() { return m_tcp; }
};

