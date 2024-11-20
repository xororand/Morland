#include "DBManager.h"
#include "mServer/Server.h"

DBManager::DBManager(Server* serv) {
	setServer(serv);
	getServer()->getLogger()->info(L"Loading DBManager done.");
}

DBManager::~DBManager()
{
}
