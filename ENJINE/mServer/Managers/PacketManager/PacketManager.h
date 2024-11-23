#pragma once
#include "enjPacket/enjPacket.h"
#include "../S_Manager.h"

class Peer;

class PacketManager : public S_Manager
{
public:
	PacketManager(Server* serv);

	void send_packet(Peer* peer, enjPacket p);
	int process_packet(Peer* peer);

	void c_ping(Peer* peer);				// To-Client
	void c_ping(Peer* peer, enjPacket& p); // From-Client

	void c_register_user(Peer* peer, sf::Uint8 status);	// To-Client
	void c_register_user(Peer* peer, enjPacket& p);	// From-Client

	void c_login_user(Peer* peer, sf::Uint8 status);	// To-Client
	void c_login_user(Peer* peer, enjPacket& p);	// From-Client
};

