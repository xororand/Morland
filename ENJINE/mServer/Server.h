#pragma once
#include "SFML/Graphics.hpp"
#include <imgui-SFML.h>
#include <imgui.h>
#include "box2d/box2d.h"

#include "Managers/PacketManager/PacketManager.h"
#include "mServer/defines.h"
#include "mGame/Managers/Network/defines.h"
#include "Utils/Logger/Logger.h"
#include "servUI/servUI.h"
#include "Player/Player.h"
#include "version.h"

#include "windows.h"
#include <format>
#include <thread>
#include <deque>
#include <vector>
#include <functional>

using namespace std::chrono;

class Server
{
public:
	struct debug_stats {
		int tps_point = 0;
		float tps_samples[500];

		Clock m_tps_timer;
		float m_current_tps = 0.0;
	};
private:
	servUI* ui					= nullptr;
	Logger* m_logger			= nullptr;
	b2WorldId m_b2worldId;
	PacketManager* m_packetmng	= nullptr;
	RenderWindow* m_window		= nullptr;
	

	sf::Clock deltaClock;
	
	float tps_treshold = (float)(1.0 / MAX_TPS);

	bool is_run = true;
	std::vector<std::thread*> m_threads;
	
	TcpListener m_tcp_listener;
	std::deque<Player*> m_players;

	debug_stats* d_stats;

	// MUTEXES
	std::mutex tps_mutex;
	std::mutex players_mutex;
	std::mutex players_internal_mutex;
private:
	void accept_new_connections();
	void process_connections();
	void drawUI();

	void tick();

	void thread_process(int id);
public:
	Server();
	int run(int ticksPerSecond);

	void addPlayer(TcpSocket* sock);

	void ping_player(size_t idx);
	void disconnect_player(size_t idx);
	
	Logger* getLogger()					{ return m_logger; }
	PacketManager* getPacketManager()	{ return m_packetmng; }
	RenderWindow* getRenderWindow()		{ return m_window; }

	std::deque<Player*> getPlayers();
	float getCurrentTPS()				{ return d_stats->m_current_tps; }
	debug_stats* getDebugStats()		{ return d_stats; }
};

