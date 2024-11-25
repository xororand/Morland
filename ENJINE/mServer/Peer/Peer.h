#pragma once
#include "mGame/Managers/Network/defines.h"
#include <Object/mServer/S_PlayerObj/S_PlayerObj.h>
#include "Utils/Utils.h"

#include "box2d/box2d.h"

#include <chrono>


using namespace std::chrono;

class Server;

/*
Обрабатывается каждый тик сервера
*/
class Peer
{
public:
	/*
	* disconnected - Отключен сервером
	* not_verifed - не прошел верификацию протокола
	* verifed - прошел верификацию протокола
	* logged_in - прислал логин-данные которые есть в базе
	* sync_ready - прошедший авторизацию и готовый принимать синхру
	*/
	enum status {
		disconnected,
		not_verifed,
		verifed,
		logged_in,
		sync_ready
	};
private:
	// pid а также индекс в массиве пиров
	sf::Uint16 m_pid = 0;
	std::wstring m_username = L"None";

	Server*			m_server		= nullptr;
	S_PlayerObj*	m_playerobj		= nullptr;

	std::vector<Peer*> sync_peers;

	TcpSocket*		m_tcp			= nullptr;;

	time_t first_connect_t;
	time_t last_packet_t;
	time_t last_ping_t;
	time_t last_not_ping_packet_t;

	system_clock::time_point ping_sent_t;
	long long ping_ms = 0;

	sf::Uint8 unk_packets_c = 0;
	sf::Uint8 failed_logins_c = 0;

	status m_status = not_verifed;
	std::wstring disconnect_reason = L"Unknown";
public:
	Peer(sf::Uint16 idx, Server* serv, TcpSocket* tcp);
	~Peer();

	void process();
	Server* getServer() { return m_server; }

	std::vector<Peer*> getSyncPeers() { return sync_peers; }
	void onPeerAdd2Sync(Peer* peer);
	void onPeerRemove2Sync(Peer* peer);

	static std::wstring to_wstring(status s);

	system_clock::time_point getWhenPingSent()				{ return ping_sent_t; }
	void setWhenPingSent(system_clock::time_point tp)		{ ping_sent_t = tp; }
	void setPing(long long ms)								{ ping_ms = ms; }

	void setUsername(std::wstring name)						{ m_username = name; }
	std::wstring getUsername()								{ return m_username; }

	void disconnect();
	void ping();

	long long getPingMS()									{ return ping_ms;	}
	void setPingMS(long long ms)							{ ping_ms = ms;		}
	
	void setPlayerObj(S_PlayerObj* pobj)					{ m_playerobj = pobj; }
	S_PlayerObj* getPlayerObj()								{ return m_playerobj; }

	void addUnkPacket()										{ unk_packets_c++;		}
	void addFailedLogin()									{ failed_logins_c++;	}
	void setLastNotPingPacketReceive(time_t time)			{ last_not_ping_packet_t = time; }

	TcpSocket*		getTcp()								{ return m_tcp; }
	void			setStatus(status s)						{ m_status = s; }
	status			getStatus()								{ return m_status; }
	void			setDisconnectReason(std::wstring str)	{ disconnect_reason = str; }
	std::wstring	getDisconnectReason()					{ return disconnect_reason; }
	sf::Uint16		getID()									{ return m_pid; }
};

