#include "S_PlayerObj.h"
#include "mServer/Peer/Peer.h"
#include "mServer/Server.h"


S_PlayerObj::S_PlayerObj(Peer* peer) 
	: m_peer(peer) 
{
	setType(Type::S_PlayerObj);

	peer->setPlayerObj(this);

	WorldManager* wm = peer->getServer()->getWorldManager();

	// Инициализация тела
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position = peer->getServer()->getDBManager()->get_user_pos(peer);
	bodyDef.type = b2_dynamicBody;
	m_bodyId = b2CreateBody(wm->getWorld(), &bodyDef);

	// Присвоение телу коллизии
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.friction = 0.42f;
	b2Polygon box = b2MakeRoundedBox(0.85f, 0.175f, 0.05f);
	b2CreatePolygonShape(m_bodyId, &shapeDef, &box);
}

S_PlayerObj::~S_PlayerObj() {
	b2DestroyBody(getBodyID());
	m_bodyId = b2_nullBodyId;
}

void S_PlayerObj::onProcess()
{
	// Обработка физики
}
