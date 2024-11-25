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
	enum ShapeType
	{ // 0x ‘Œ–Ã¿“ Œ◊≈Õ‹ ¬¿∆≈Õ ¡Àﬂ“‹
		PLAYER					= 0x0001,
		PLAYER_DIST_DETECTOR	= 0x0002
	};

	WorldManager(Server* serv);
	~WorldManager();
	
	b2WorldId		getWorld()	{ return m_b2worldId; }
	//S_RootObj*	getRoot()	{ return m_root; }

	void sensorsProcess();

	S_PlayerObj*	spawnPlayer(Peer* peer);
	void			despawnPlayer(Peer* peer);

	void process();
	void save();
};

