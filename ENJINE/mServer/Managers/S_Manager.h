#pragma once
#include <memory>

class Server;

class S_Manager
{
private:
	Server* m_server;
public:
	Server* getServer()								{ return m_server; }
	void					setServer(Server* serv) { m_server = serv; }
};

