#pragma once

#include "defines.h"
class NetworkManager
{
public:
	enum Status {
		None,
		pending_connection,
		connection_done,
		connection_failed,
	};
private:
	TcpSocket* m_tcp = nullptr;

	NetworkManager::Status status		= Status::None;
	int			last_try_time			= 0;
	int				try_connect_count	= 0;
	std::string		last_ip				= "";
	unsigned short	last_port			= 0;
public:
	NetworkManager() {
		// TODO: INIT NETWORK
	}
	void set_connection_data(std::string ip, unsigned short port) {
		last_ip = ip;
		last_port = port;
	}
	void get_connection_data(std::string& ip, unsigned short& port) {
		ip = last_ip;
		port = last_port;
	}
	void set_status(Status s)	{ status = s; }
	Status get_status()			{ return status; }
	int get_connection_trys()	{ return try_connect_count; }
	/*
	Перед подключением нужно поставить статус pending_connection и указать данные для подключения

	true - подключились / false - ошибка подключения
	*/
	Status connect_server();

};

