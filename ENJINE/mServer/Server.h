#pragma once

#include "SFML/Graphics.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

#include "defines.h"
#include "mGame/Managers/Network/defines.h"
#include "enjPacket/defines.h"
#include "Utils/Logger/Logger.h"
#include "Managers/PacketManager/PacketManager.h"
#include "Managers/DBManager/DBManager.h"
#include "Managers/WorldManager/WorldManager.h"
#include "servUI/servUI.h"
#include "Peer/Peer.h"

#include "version.h"

#include "windows.h"
#include <format>
#include <thread>
#include <deque>
#include <vector>
#include <functional>
#include <filesystem>

using namespace std::chrono;
namespace fs = std::filesystem;

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
	std::vector<std::thread*>			m_threads;
	std::deque<Peer*>	m_peers;

	bool is_run	= true;
	servUI*			ui					= nullptr;
	Logger*			m_logger			= nullptr;
	DBManager*		m_dbmanager			= nullptr;
	WorldManager*	m_worldmanager		= nullptr;
	PacketManager*	m_packetmng			= nullptr;
	RenderWindow*	m_window			= nullptr;
	
	sf::Clock		deltaClock;
	
	TcpListener		m_tcp_listener;
	float			m_tps_treshold;
	debug_stats*	d_stats;

	// MUTEXES
	std::mutex		tps_mutex;
	std::mutex		peers_mutex;
	std::mutex		peers_internal_mutex;
public:
	Server(float tps = (float)(1.0 / MAX_TPS));
	int run();
	void stop() { is_run = false; }

	std::deque<Peer*> getPeers();
	void addPeer(TcpSocket* sock);
	void disconnectPeer(size_t idx);
	bool isPeerExists(size_t idx);
	bool isUsernameConnected(std::wstring name);

	Logger*			getLogger()			{ return m_logger;		}
	DBManager*		getDBManager()		{ return m_dbmanager; }
	WorldManager*	getWorldManager()	{ return m_worldmanager; }
	PacketManager*	getPacketManager()	{ return m_packetmng;	}
	RenderWindow*	getRenderWindow()	{ return m_window;		}

	float getMaxTPS()					{ return m_tps_treshold;			}
	void setMaxTPS(float tps);
	float getCurrentTPS()				{ return d_stats->m_current_tps;	}
	debug_stats* getDebugStats()		{ return d_stats;					}
private:
	void accept_new_connections();
	void process_peers();

	void drawUI();

	void tick();
	void thread_process(int id);
};

