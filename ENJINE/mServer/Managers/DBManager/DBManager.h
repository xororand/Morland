#pragma once
#include "../S_Manager.h"
#include "winsqlite/winsqlite3.h"
#include <string>

class Peer;

class DBManager : public S_Manager
{
public:
	enum error {
		chars_not_allowed,
		str_size_not_allowed,
		not_exists,
		not_success,
		success
	};
private:
	sqlite3* m_db;
	
	std::wstring sqlite_path = L"";
public:
	DBManager(Server* serv);
	~DBManager();

	void init();

	static bool isStringAllowed(std::wstring str);
	static bool isStringAllowed(std::string str);

	bool is_user_exists(std::wstring username);
	bool is_user_auth(std::wstring username, std::string passhash);

	void set_user_last_ip(std::wstring username, std::string lastip);

	error add_user(Peer* peer, std::wstring username, std::wstring passhash);

};

