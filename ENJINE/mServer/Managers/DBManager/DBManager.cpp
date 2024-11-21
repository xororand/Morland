#include "DBManager.h"
#include "mServer/Server.h"

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
    if (NULL != pDB.get()) sqlite3_close(pDB.get());
    sqlite3_shutdown();
}

int DBManager::init()
{
    Logger* log = getServer()->getLogger();
    sqlite3* db = pDB.get();
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
        if (SQLITE_OK != (ret = sqlite3_open_v2(to_ancii(sqlite_path), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
        {
            sqlite3_close_v2(db);
            log->exit_error(std::format(L"Failed to open conn: %d\n", ret).c_str());
            break;
        }
    } while (false);

    const char* sql_create_ifnexist =
        "CREATE TABLE IF NOT EXISTS `users`("
        "`ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `username` VARCHAR(256) NOT NULL, "
        "`password` VARCHAR(256) NOT NULL, `lastXpos` DOUBLE NOT NULL DEFAULT 0, `lastYpos` DOUBLE NOT NULL DEFAULT 0);";

    if (SQLITE_OK != (ret = sqlite3_exec(db, sql_create_ifnexist, NULL, NULL, NULL))) {
        sqlite3_close_v2(db);
        log->exit_error(std::format(L"Failed to create new database table USERS: %d\n", ret).c_str());
    }

    if (db != NULL) {

    }

    return ret;
}
