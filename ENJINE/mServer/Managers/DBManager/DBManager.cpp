                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  #include "DBManager.h"
#include "mServer/Server.h"
#include "mServer/Peer/Peer.h"

DBManager::DBManager(Server* serv) {
	setServer(serv);

	sqlite_path = std::format( L"{}\\database.sqlite3", DATA_DIR );

	//TODO: создание базы данных если ее не обнаружится в папке DATA_DIR
    fs::create_directory(DATA_DIR);

    this->init();

	getServer()->getLogger()->info(L"Loading DBManager done.");
}

DBManager::~DBManager()
{
    // cleanup
    if (NULL != m_db) sqlite3_close(m_db);
    sqlite3_shutdown();
}

void DBManager::init()
{
    Logger* log = getServer()->getLogger();
    sqlite3_stmt* query = NULL;
    int ret = 0;
    do // avoid nested if's
    {
        // initialize engine
        if (SQLITE_OK != (ret = sqlite3_initialize()))
        {
            log->exit_error(std::format(L"Failed to initialize library: %d\n", ret).c_str());
            break;
        }
        // open connection to a DB
        if (SQLITE_OK != (ret = sqlite3_open_v2(to_ancii(sqlite_path), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
        {
            sqlite3_close_v2(m_db);
            log->exit_error(std::format(L"Failed to open conn: %d\n", ret).c_str());
            break;
        }
    } while (false);

    // Создание таблицы users если ее не существует или база только что была создана

    const char* sql_create_users_ifnexist =
        "CREATE TABLE IF NOT EXISTS "
        "`users`("
        "`ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "`username` VARCHAR(256) NOT NULL, "
        "`password` VARCHAR(256) NOT NULL,"
        "`lastXpos` DOUBLE NOT NULL DEFAULT 0,"
        "`lastYpos` DOUBLE NOT NULL DEFAULT 0,"
        "`regIP`    VARCHAR(39) NOT NULL,"
        "`lastIP`   VARCHAR(39) NOT NULL"
        ");";

    if (SQLITE_OK != (ret = sqlite3_exec(m_db, sql_create_users_ifnexist, NULL, NULL, NULL))) {
        sqlite3_close_v2(m_db);
        log->exit_error(std::format(L"Failed to create new database table USERS: %d\n", ret).c_str());
    }
}

bool DBManager::isStringAllowed(std::wstring str)
{
    std::wstring b_chars = L"!@#$%^&*()+\"'\\|}{][-=;:>.<,?/*";

    for (auto c : str)
        for (auto bc : b_chars)
            if (c == bc) return false;

    return true;
}
bool DBManager::isStringAllowed(std::string str)
{
    return isStringAllowed(to_wide(str));
}

bool DBManager::is_user_exists(std::wstring username)
{
    if (!isStringAllowed(username)) return false;

    Logger* log = getServer()->getLogger();
    sqlite3_stmt* stmt;
    int ret = 0;

    std::string query = std::format("SELECT username FROM users WHERE username = '{}'", to_ancii(username));
    ret = sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL);

    if (stmt == NULL) return false;

    sqlite3_step(stmt);
    const wchar_t* usernamedb = (const wchar_t*)sqlite3_column_text16(stmt, 0);
    if (usernamedb == NULL) { 
        sqlite3_finalize(stmt); 
        return false; 
    }
    sqlite3_finalize(stmt);
    return true;
}
bool DBManager::is_user_auth(std::wstring username, std::string passhash)
{
    if (!isStringAllowed(username) or !isStringAllowed(passhash)) return false;

    Logger* log = getServer()->getLogger();
    sqlite3_stmt* stmt;
    int ret = 0;

    std::string query = std::format("SELECT username FROM users WHERE username = '{}' AND password = '{}'", 
        to_ancii(username),
        passhash
    );
    ret = sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL);

    if (stmt == NULL) return false;

    sqlite3_step(stmt);
    const wchar_t* usernamedb = (const wchar_t*)sqlite3_column_text16(stmt, 0);
    if (usernamedb == NULL) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

void DBManager::set_user_last_ip(std::wstring username, std::string lastip)
{
    std::string query = std::format("UPDATE users SET lastIP = '{}' WHERE username = '{}'", lastip, to_ancii(username) );
    sqlite3_exec(m_db, query.c_str(), NULL, NULL, NULL);
}

/* false - юзер уже существует, true - учетная запись была создана */
DBManager::error DBManager::add_user(Peer* peer, std::wstring username, std::wstring password)
{
    if (username.size() > 256)      return str_size_not_allowed;
    if (password.size() > 1024)     return str_size_not_allowed;
    if (!isStringAllowed(username)) return chars_not_allowed;

    if (is_user_exists(username))   return not_exists;

    std::string passhash = to_ancii(Utils::hashing::sha256(password));
    std::string ip = peer->getTcp()->getRemoteAddress().toString();

    std::string query = std::format("INSERT INTO `users` (`username`, `password`, `regIP`, `lastIP`) VALUES "
    "('{}','{}','{}','{}')", to_ancii(username), passhash, ip, ip);

    if (sqlite3_exec(m_db, query.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        int error = sqlite3_errcode(m_db);
        return not_success;
    }

    return success;
}
void DBManager::update_user_pos(S_PlayerObj* pobj, b2Vec2 pos) {
    Peer* peer = pobj->getPeer();

    // Если пира нет - нет и имени игрока - нельзя записать новые значения в базу
    if (peer == nullptr) return;

    std::wstring username = peer->getUsername();

    std::string query = std::format("UPDATE users SET lastXpos = '{}', lastYpos = '{}' WHERE username = '{}'", pos.x, pos.y, to_ancii(username));
    sqlite3_exec(m_db, query.c_str(), NULL, NULL, NULL);
}
b2Vec2 DBManager::get_user_pos(Peer* peer)
{
    sqlite3_stmt* stmt;
    int ret = 0;

    std::string query = std::format("SELECT lastXpos, lastYpos FROM users WHERE username = '{}'",
        to_ancii(peer->getUsername())
    );
    ret = sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL);

    sqlite3_step(stmt);

    float x = (float)sqlite3_column_double(stmt, 0);
    float y = (float)sqlite3_column_double(stmt, 1);

    b2Vec2 pos = b2Vec2(x, y);

    sqlite3_finalize(stmt);
    return pos;
}
                                                                                                                                                                                                                                                                            