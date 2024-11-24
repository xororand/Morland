#include "WorldManager.h"
#include "mServer/Server.h"

WorldManager::WorldManager(Server* serv) /* : m_root(new S_RootObj())*/ {
    setServer(serv);

    // СОЗДАНИЕ МИРА m_b2worldId
    b2WorldDef worldDef = b2DefaultWorldDef();
    //worldDef.enableSleep = false;
    worldDef.gravity = b2Vec2(0.0f, -9.8f);
    m_b2worldId = b2CreateWorld(&worldDef);
    
    getServer()->getLogger()->info(L"Loading WorldManager done.");
}

WorldManager::~WorldManager() {
    b2DestroyWorld(getWorld());
}

void WorldManager::process() {
    // Обработка всех объектов в мире
    // 
    // Обработка игроков
    for (auto p : m_players) p->process();
    

    //dynamic_cast<S_Object*>(getRoot())->process();
    // Обработка физических объектов
    b2World_Step(m_b2worldId, getServer()->getMaxTPS(), m_subStepCount);
}

S_PlayerObj* WorldManager::spawnPlayer(Peer* peer)
{
    if (peer == nullptr) return nullptr;

    S_PlayerObj* player = new S_PlayerObj(peer);

    m_players.push_back(player);

    Logger* log = getServer()->getLogger();

    b2Vec2 pos = b2Body_GetPosition(player->getBodyID());

    log->info(std::format(L"Player {} spawned in x:{},y:{}", peer->getUsername(), pos.x, pos.y).c_str());

    return player;
}

// Вызывается при отключении пира от сервера по любой причине
void WorldManager::despawnPlayer(Peer* peer) {
    if (peer == nullptr) return;

    S_PlayerObj* obj = peer->getPlayerObj();

    if (obj == nullptr) return;
    if (!b2Body_IsValid(obj->getBodyID())) return;

    Logger* log = getServer()->getLogger();
    DBManager* db = getServer()->getDBManager();
    b2Vec2 pos = b2Body_GetPosition(obj->getBodyID());

    // Сохраняем позицию в базе данных
    db->update_user_pos(obj, pos); 

    log->info(std::format(L"Player {} DEspawned in x:{},y:{}", peer->getUsername(), pos.x, pos.y).c_str());

    auto it = std::find(m_players.begin(), m_players.end(), obj);
    m_players.erase(it);
    // TODO: умная систему удаления на deque как для пиров
    // TODO: сохранение состояния куклы

    delete obj;
    
    peer->setPlayerObj(nullptr);
}
