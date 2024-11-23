#pragma once
#include "mGame/Managers/Network/defines.h"
#include <Object/mServer/S_PlayerObj/S_PlayerObj.h>
#include "Utils/Utils.h"

#include "box2d/box2d.h"

#include <chrono>


using namespace std::chrono;

class Server;

/*
�������������� ������ ��� �������
*/
class Peer
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
	std::wstring m_username = L"None";

	Server*			m_server		= nullptr;
	S_PlayerObj*	m_playerobj		= nullptr;;

	TcpSocket*		m_tcp			= nullptr;;

	time_t first_connect_t;
	time_t last_packet_t;
	time_t last_ping_t;
	time_t last_not_ping_packet_t;

	system_clock::time_point ping_sent_t;
	long long ping_ms = 0;

	int unk_packets_c = 0;
	int failed_logins_c = 0;

	status m_status = not_verifed;
	std::wstring disconnect_reason = L"Unknown";
public:
	Peer(size_t idx, Server* serv, TcpSocket* tcp);
	~Peer();

	void process();
	Server* getServer() { return m_server; }

	static std::wstring to_wstring(status s);

	system_clock::time_point getWhenPingSent()			{ return ping_sent_t; }
	void setWhenPingSent(system_clock::time_point tp)	{ ping_sent_t = tp; }
	void setPing(long long ms)							{ ping_ms = ms; }

	void setUsername(std::wstring name) { m_username = name; }
	std::wstring getUsername()			{ return m_username; }

	void disconnect();
	void ping();

	long long getPingMS()							{ return ping_ms;	}
	void setPingMS(long long ms)					{ ping_ms = ms;		}
	
	void addUnkPacket()								{ unk_packets_c++;		}
	void addFailedLogin()							{ failed_logins_c++;	}
	void setLastNotPingPacketReceive(time_t time)	{ last_not_ping_packet_t = time; }

	TcpSocket*		getTcp()								{ return m_tcp; }
	void			setStatus(status s)						{ m_status = s; }
	status			getStatus()								{ return m_status; }
	void			setDisconnectReason(std::wstring str)	{ disconnect_reason = str; }
	std::wstring	getDisconnectReason()					{ return disconnect_reason; }
	size_t			getID()									{ return idx; }
};

