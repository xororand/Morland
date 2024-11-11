#pragma once
class Player;
class Server;

class PacketManager
{
private:
	Server* m_server;
private:
	bool verife_packet(const void* data, size_t size);
public:
	PacketManager(Server* serv);

	Server* getServer() { return m_server;}

	int process_packet(Player* pl);

	void c_ping(Player* pl, const void* data, size_t size);
};

