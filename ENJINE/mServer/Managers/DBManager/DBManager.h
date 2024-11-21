#pragma once
#include "../S_Manager.h"
#include "winsqlite/winsqlite3.h"
#include <string>

class DBManager : public S_Manager
{
private:
	std::shared_ptr<sqlite3> pDB;
	
	std::wstring sqlite_path = L"";
public:
	DBManager(Server* serv);
	~DBManager();

	int init();
};

