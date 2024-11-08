#pragma once

#include "mGame/Managers/Network/defines.h"
#include "Utils/Utils.h"

class Server;

class Player
{
public:
	enum status {
		disconnected,
		not_verifed,
		verifed,
		logged_in
	};
private:
	size_t idx = 0;
	
	Server* m_server;

	TcpSocket* m_tcp;
	time_t first_connect_t;
	time_t last_packet_t;
	status m_status = not_verifed;
private:
	void setStatus(status s) { m_status = s; }
public:
	Player(size_t idx, Server* ptr_serv, TcpSocket* tcp);
	~Player();

	void process();

	Server* getServer() { return m_server; }
	TcpSocket* getTcp() { return m_tcp; }
	status getStatus()	{ return m_status; }
	size_t getIDX()		{ return idx; }
};

