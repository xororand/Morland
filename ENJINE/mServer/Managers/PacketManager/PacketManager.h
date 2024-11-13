#pragma once
class Peer;
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

	int process_packet(Peer* pl);

	void c_ping(Peer* pl, const void* data, size_t size);
};

