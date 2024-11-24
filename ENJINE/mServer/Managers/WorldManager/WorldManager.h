#pragma once
#include "../S_Manager.h"
#include "box2d/box2d.h"
//#include "Object/mServer/S_RootObj/S_RootObj.h"
#include "Object/mServer/S_PlayerObj/S_PlayerObj.h"

class Server;
class Peer;

class WorldManager : public S_Manager
{
private:
	//S_RootObj*		m_root		= nullptr;
	std::vector<S_PlayerObj*> m_players;
	b2WorldId		m_b2worldId;
	int				m_subStepCount = 4;
public:
	WorldManager(Server* serv);
	~WorldManager();
	
	b2WorldId		getWorld()	{ return m_b2worldId; }
	//S_RootObj*	getRoot()	{ return m_root; }

	S_PlayerObj*	spawnPlayer(Peer* peer);
	void			despawnPlayer(Peer* peer);

	void process();
};

