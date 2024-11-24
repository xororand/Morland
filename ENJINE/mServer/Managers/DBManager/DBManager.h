#pragma once
#include "../S_Manager.h"
#include "winsqlite/winsqlite3.h"
#include "box2d/box2d.h"

#include <string>

class Peer;
class S_PlayerObj;

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

	error add_user(Peer* peer, std::wstring username, std::wstring password);
	void update_user_pos(S_PlayerObj* pobj, b2Vec2 pos);
	b2Vec2 get_user_pos(Peer* peer);
};

