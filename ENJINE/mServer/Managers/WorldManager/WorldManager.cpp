#include "WorldManager.h"
#include "mServer/Server.h"

WorldManager::WorldManager(Server* serv) : m_root(new S_RootObj()) {
    setServer(serv);
    // СОЗДАНИЕ m_b2worldId
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2)(0.0f, 0.0f);
    m_b2worldId = b2CreateWorld(&worldDef);

    getServer()->getLogger()->info(L"Loading WorldManager done.");
}

WorldManager::~WorldManager() {
}

void WorldManager::process() {
    // Обработка всех объектов в мире
    dynamic_cast<S_Object*>(getRoot())->process();
    // Обработка физики объектов
    b2World_Step(m_b2worldId, getServer()->getMaxTPS(), m_subStepCount);
}

S_PlayerObj* WorldManager::spawnPlayer(Peer* peer)
{
    if (peer == nullptr) return nullptr;

    S_PlayerObj* player = new S_PlayerObj(peer);
    S_Object* pl_obj = dynamic_cast<S_Object*>(player);
    
    m_root->addObject(pl_obj);
    m_players.push_back(player);

    return player;
}