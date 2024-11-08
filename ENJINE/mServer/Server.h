#pragma once
#include "mServer/defines.h"
#include "mGame/Managers/Network/defines.h"
#include "Utils/Logger/Logger.h"
#include "Player.h"
#include "version.h"

#include "windows.h"
#include <format>
#include <thread>
#include <deque>
#include <vector>

class Server
{
private:
	std::vector<Thread*> m_threads;
	bool is_run = true;

	Clock m_tps_timer;
	float m_current_tps = 0.0;
	TcpListener m_tcp_listener;
	std::deque<Player*> m_players;

	Logger* m_logger;

	// MUTEXES
	Mutex tps_mutex;
	Mutex players_mutex;
	//Mutex players_internal_mutex;

private:
	void accept_new_connections();
	void process_connections();

	void tick();

	void thread_process_socket();
public:
	Server();

	void addPlayer(TcpSocket* sock);

	void disconnect_player(Player* p);
	void disconnect_player(size_t idx);

	Logger* getLogger()		{ return m_logger; }
	float getCurrentTPS()	{ return m_current_tps; }

	int run();
};

