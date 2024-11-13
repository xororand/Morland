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
	bool is_run = true;
	std::vector<std::thread*> m_threads;
	std::deque<Player*> m_players;

	servUI* ui					= nullptr;
	Logger* m_logger			= nullptr;
	b2WorldId m_b2worldId;
	int m_subStepCount = 4;
	float m_tps_treshold;
	PacketManager* m_packetmng	= nullptr;
	RenderWindow* m_window		= nullptr;
	
	sf::Clock deltaClock;
	
	TcpListener m_tcp_listener;
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
	Server(float tps = (float)(1.0 / MAX_TPS));
	int run();

	void addPlayer(TcpSocket* sock, b2Vec2 pos);

	void ping_player(size_t idx);
	void disconnect_player(size_t idx);
	
	Logger* getLogger()					{ return m_logger; }
	b2WorldId getWorld()				{ return m_b2worldId; }
	PacketManager* getPacketManager()	{ return m_packetmng; }
	RenderWindow* getRenderWindow()		{ return m_window; }

	std::deque<Player*> getPlayers();

	float getMaxTPS()					{ return m_tps_treshold; }
	void setMaxTPS(float tps);
	float getCurrentTPS()				{ return d_stats->m_current_tps; }
	debug_stats* getDebugStats()		{ return d_stats; }
};

