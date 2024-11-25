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
	// userData = ссылка на наш S_PlayerObj*
	bodyDef.userData = this;
	m_bodyId = b2CreateBody(wm->getWorld(), &bodyDef);

	// Присвоение телу коллизии
	b2ShapeDef BshapeDef = b2DefaultShapeDef();
	BshapeDef.friction = 0.42f;
	BshapeDef.filter.categoryBits = WorldManager::PLAYER;
	BshapeDef.filter.maskBits = WorldManager::PLAYER | WorldManager::PLAYER_DIST_DETECTOR;
	BshapeDef.enableSensorEvents = true;
	b2Polygon box = b2MakeRoundedBox(0.85f, 0.175f, 0.05f);
	b2CreatePolygonShape(m_bodyId, &BshapeDef, &box);

	// Присвоение коллизии определения других игроков
	b2ShapeDef PDshapeDef = b2DefaultShapeDef();
	PDshapeDef.friction = 0.42f;
	PDshapeDef.filter.categoryBits = WorldManager::PLAYER_DIST_DETECTOR;
	PDshapeDef.filter.maskBits = WorldManager::PLAYER;
	PDshapeDef.isSensor = true;
	b2Circle circle;
	circle.center = b2Vec2(0.0f, 0.0f);
	circle.radius = PLAYERS_VIEW_PLAYERS;
	b2CreateCircleShape(m_bodyId, &PDshapeDef, &circle);
}

S_PlayerObj::~S_PlayerObj() {
	if (b2Body_IsValid( getBodyID() )) {
		b2DestroyBody( getBodyID() );
		m_bodyId = b2_nullBodyId;
	}
}

void S_PlayerObj::save()
{
	DBManager* db = getPeer()->getServer()->getDBManager();
	db->update_user_pos(this);
}

void S_PlayerObj::onProcess() {
	//b2WorldId worldid =	getPeer()->getServer()->getWorldManager()->getWorld();
	// Обработка физики
}
