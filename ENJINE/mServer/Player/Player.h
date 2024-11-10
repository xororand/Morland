#pragma once

#include "mGame/Managers/Network/defines.h"
#include "Utils/Utils.h"

#include <chrono>

class Server;

class Player
{
public:
	/*
	* disconnected - �������� ��������
	* not_verifed - �� ������ ����������� ���������
	* verifed - ������ ����������� ���������
	* logged_in - ������� �����-������ ������� ���� � ����
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
	std::chrono::system_clock::time_point last_ping_ms;
	long long ping_ms;

	status m_status = not_verifed;
private:
	void setStatus(status s) { m_status = s; }
public:
	Player(size_t idx, Server* ptr_serv, TcpSocket* tcp);
	~Player();

	void process();

	static std::wstring to_wstring(status s);

	std::chrono::system_clock::time_point getLastPing_tp()			{ return last_ping_ms; }
	void setLastPing_tp(std::chrono::system_clock::time_point tp)			{ last_ping_ms = tp; }

	long long getPingMS()											{ return ping_ms; }

	Server* getServer() { return m_server; }
	TcpSocket* getTcp() { return m_tcp; }
	status getStatus()	{ return m_status; }
	size_t getID()		{ return idx; }
};

