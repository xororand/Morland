#pragma once

#include "mServer/defines.h"
#include "mGame/defines.h"
#include "mGame/Managers/Network/defines.h"
#include "enjsocket.h"
#include "Utils/Logger/Logger.h"

#include "windows.h"
#include <format>
#include <thread>
#include <vector>

class Server
{
private:
	std::vector<std::thread> m_threads;
	bool is_start = true;

	Clock m_tps_timer;
	float m_current_tps = 0.0;
	TcpListener m_tcp_listener;
	std::vector<enjsocket> m_sockets;

	Logger m_logger;

	std::mutex tps_mutex;
private:
	void accept_new_connections();
	void process_connections();

	void tick();

	void thread_process_socket();
public:
	Server() {};

	Logger getLogger()		{ return m_logger; }
	
	float getCurrentTPS()	{ return m_current_tps; }

	int run();
};

