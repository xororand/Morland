#include "WorldManager.h"
#include "mServer/Server.h"

WorldManager::WorldManager(Server* serv) /* : m_root(new S_RootObj())*/ {
    setServer(serv);

    // СОЗДАНИЕ МИРА m_b2worldId
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(0.0f, 0.1f);
    // Можно включить для оптимизации зависших тел, но трудно дебажить если true
    worldDef.enableSleep = false;
    m_b2worldId = b2CreateWorld(&worldDef);
    
    getServer()->getLogger()->info(L"Loading WorldManager done.");
}

WorldManager::~WorldManager() {
    b2DestroyWorld(getWorld());
}

void WorldManager::process() {
    sensorsProcess();

    // Обработка игроков
    for (auto p : m_players) p->process();

    // Обработка физических объектов
    b2World_Step(m_b2worldId, getServer()->getMaxTPS(), m_subStepCount);
}

void WorldManager::save()
{
    DBManager* db = getServer()->getDBManager();
    // TODO: СДЕЛАТЬ ПРОСТОЕ ПЕРЕЧИСЛЕНИЕ ВСЕХ ОБЪЕКТОВ В ОДНОМ МАССИВЕ
    // Сохраняем игроков
    for (auto p : m_players) p->save();
}

void WorldManager::sensorsProcess() {
    b2SensorEvents sensor_events = b2World_GetSensorEvents(getWorld());
    for (int32_t i = 0; i < sensor_events.beginCount; i++) {
        b2ShapeId	sensor_shape = sensor_events.beginEvents[i].sensorShapeId;
        if (!b2Shape_IsValid(sensor_shape)) continue;
        b2BodyId	sensor_body = b2Shape_GetBody(sensor_shape);
        if (!b2Body_IsValid(sensor_body)) continue;

        b2ShapeId	visitor_shape = sensor_events.beginEvents[i].visitorShapeId;
        if (!b2Shape_IsValid(visitor_shape)) continue;
        b2BodyId	visitor_body = b2Shape_GetBody(visitor_shape);
        if (!b2Body_IsValid(visitor_body)) continue;

        S_Object* sensor_obj = (S_Object*)b2Body_GetUserData(sensor_body);
        if (sensor_obj == nullptr) continue;
        S_Object* visitor_obj = (S_Object*)b2Body_GetUserData(visitor_body);
        if (visitor_obj == nullptr) continue;

        if (sensor_obj == visitor_obj) continue;

        // Обработка коллизий игроков
        if (sensor_obj->getType() == S_Object::S_PlayerObj && visitor_obj->getType() == S_Object::S_PlayerObj) {
            S_PlayerObj* sensor_pobj = nullptr;
            sensor_pobj = dynamic_cast<S_PlayerObj*>(sensor_obj);
            if (sensor_pobj == nullptr) continue;

            S_PlayerObj* visitor_pobj = nullptr;
            visitor_pobj = dynamic_cast<S_PlayerObj*>(visitor_obj);
            if (visitor_pobj == nullptr) continue;

            // Вызываем у тела сенсора ивент на добавление в список синхронизации
            sensor_pobj->getPeer()->onPeerAdd2Sync( visitor_pobj->getPeer() );
        }
    }
    for (int32_t i = 0; i < sensor_events.endCount; i++) {
        b2ShapeId	sensor_shape = sensor_events.endEvents[i].sensorShapeId;
        if (!b2Shape_IsValid(sensor_shape)) continue;
        b2BodyId	sensor_body = b2Shape_GetBody(sensor_shape);
        if (!b2Body_IsValid(sensor_body)) continue;

        b2ShapeId	visitor_shape = sensor_events.endEvents[i].visitorShapeId;
        if (!b2Shape_IsValid(visitor_shape)) continue;
        b2BodyId	visitor_body = b2Shape_GetBody(visitor_shape);
        if (!b2Body_IsValid(visitor_body)) continue;

        S_Object* sensor_obj = (S_Object*)b2Body_GetUserData(sensor_body);
        if (sensor_obj == nullptr) continue;
        S_Object* visitor_obj = (S_Object*)b2Body_GetUserData(visitor_body);
        if (visitor_obj == nullptr) continue;

        // Обработка коллизий игроков
        if (sensor_obj->getType() == S_Object::S_PlayerObj && visitor_obj->getType() == S_Object::S_PlayerObj) {
            S_PlayerObj* sensor_pobj = nullptr;
            sensor_pobj = dynamic_cast<S_PlayerObj*>(sensor_obj);
            if (sensor_pobj == nullptr) continue;

            S_PlayerObj* visitor_pobj = nullptr;
            visitor_pobj = dynamic_cast<S_PlayerObj*>(visitor_obj);
            if (visitor_pobj == nullptr) continue;

            // Вызываем у тела сенсора ивент на добавление в список синхронизации
            sensor_pobj->getPeer()->onPeerRemove2Sync(visitor_pobj->getPeer());
        }
    }
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
    // TODO: умная систему удаления на deque как для пиров
    // TODO: сохранение состояния куклы
    auto it = std::find(m_players.begin(), m_players.end(), obj);
    m_players.erase(it);


    delete obj; 
    peer->setPlayerObj(nullptr);
}
