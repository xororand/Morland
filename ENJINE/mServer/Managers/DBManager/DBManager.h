#pragma once
#include "../S_Manager.h"

class DBManager : public S_Manager
{

public:
	DBManager(Server* serv);
	~DBManager();
};

