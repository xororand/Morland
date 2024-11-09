#pragma once
#include <cstdint>
#include <map>

class Server;

class PacketManager
{
private:
	Server* m_server;

	std::map<INT16, std::function<void(PacketManager*)>> m_commands;
private:
	bool verife_packet(Packet p);
public:
	PacketManager(Server* serv);

	Server* getServer() { return m_server;}

	void process_packet(Player* pl, Packet p);
	void c_ping(Player* pl, Packet p);
};

