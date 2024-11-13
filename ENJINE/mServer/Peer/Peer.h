#pragma once
#include "mGame/Managers/Network/defines.h"
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
	*/
	enum status {
		disconnected,
		not_verifed,
		verifed,
		logged_in
	};
private:
	size_t idx = 0;
	Server* m_server;

	TcpSocket* m_tcp;

	time_t first_connect_t;
	time_t last_packet_t;
	system_clock::time_point last_ping_ms;
	long long ping_ms;

	status m_status = not_verifed;
	std::wstring disconnect_reason = L"Unknown";
public:
	Peer(size_t idx, Server* serv, TcpSocket* tcp);
	~Peer();

	void process();
	Server* getServer() { return m_server; }

	static std::wstring to_wstring(status s);

	system_clock::time_point getLastPing_tp()			{ return last_ping_ms; }
	void setLastPing_tp(system_clock::time_point tp)	{ last_ping_ms = tp; }


	long long getPingMS()								{ return ping_ms; }
	void setPingMS(long long ms)						{ ping_ms = ms; }
	
	TcpSocket*		getTcp()								{ return m_tcp; }
	void			setStatus(status s)						{ m_status = s; }
	status			getStatus()								{ return m_status; }
	void			disconnect();
	void			setDisconnectReason(std::wstring str)	{ disconnect_reason = str; }
	std::wstring	getDisconnectReason()					{ return disconnect_reason; }
	size_t			getID()									{ return idx; }
};

