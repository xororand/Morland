#pragma once

#include "defines.h"
#include "enjPacket/defines.h"
#include "Utils/Logger/Logger.h"

class Game;

class NetworkManager
{
public:
	enum Status {
		None,
		self_disconnected,
		pending_connection,
		connection_done,
		connection_failed,
	};
private:
	Game* m_game		= nullptr;
	TcpSocket* m_tcp	= nullptr;

	std::wstring	m_username = L"";

	bool is_auth = false;
	NetworkManager::Status status		= Status::None;
	int				last_try_time		= 0;
	int				try_connect_count	= 0;
	std::string		last_ip				= "";
	unsigned short	last_port			= 0;
	std::chrono::system_clock::time_point last_c_ping;
	std::wstring m_last_error_msg = L"";
public:
	NetworkManager(Game* game);
	
	Game* getGame() { return m_game; }
	
	void process();

	void process_packet();
	void send_packet(enjPacket p);

	void disconnect();

	void c_ping();

	void c_register_user(enjPacket& p); // From-server
	/*	������ ����� �� ������� � ����� �������� ��� ���������� �����������
		��� �� ���������, ���� �� �������� ���-����� */
	void c_login_user(enjPacket& p);	// From-server
	/* 
		�������� ������ ��� ����������� ������ �������
	*/
	void c_login_user(std::wstring username, std::string password); // To-server

	TcpSocket* getTCP() { return m_tcp; }

	void set_connection_data(std::string ip, unsigned short port);
	void get_connection_data(std::string& ip, unsigned short& port);

	void setStatus(Status s)	{ status = s; }
	Status getStatus()			{ return status; }

	void setLastErrMsg(std::wstring str)	{ m_last_error_msg = str; }
	std::wstring getLastErrMsg()			{ return m_last_error_msg; }

	bool isAuthed() { return is_auth; }
	void isAuthed(bool b) { is_auth = b; }

	int get_connection_trys()	{ return try_connect_count; }

	/*
	����� ������������ ����� ��������� ������ pending_connection � ������� ������ ��� �����������
	*/
	void connect_server();
};
