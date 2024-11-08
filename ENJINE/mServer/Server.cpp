#include "Server.h"

void Server::accept_new_connections()
{
    TcpSocket* sock = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*sock);

    if (status == Socket::Status::Done) { 
        addPlayer(sock);
        return;
    }
    delete sock;
}

void Server::process_connections() {
    sf::Lock Lock(players_mutex);
    for (auto p : m_players) {
        if (p == nullptr) continue;
        if (p->getStatus() == Player::status::disconnected) {
            disconnect_player(p->getIDX());
            continue; 
        }

        p->process();
    }
}

void Server::tick()
{
    //getLogger().info(std::format(L"TPS: {}", getCurrentTPS()).c_str());
    accept_new_connections();
    process_connections();
}

void Server::thread_process_socket() {
    while (is_run) {
        tps_mutex.lock();
        float TFLT = m_tps_timer.getElapsedTime().asSeconds();     
        float tps_treshold = (float)(1.0 / MAX_TPS);

        if (TFLT < tps_treshold) {
            long long slt = (long long)((tps_treshold - TFLT) * 100);

            std::this_thread::sleep_for(std::chrono::milliseconds(slt));

            tps_mutex.unlock();
            continue;
        }

        m_tps_timer.restart();
        m_current_tps = 1.0 / TFLT;
        tps_mutex.unlock();

        tick(); // Обрабатываем 1 ТИК
    }
}

Server::Server()
{
    m_logger = new Logger();
}

void Server::addPlayer(TcpSocket* sock)
{
    sf::Lock Lock(players_mutex);

    Player* p = nullptr;
    size_t idx = 0;
    bool push = false;

    // Поиск пустой ячейки
    for (int i = 0; i < m_players.size(); i++) {
        if (m_players[i] == nullptr) {
            idx = i;
            push = true;
        }
    }

    // Пустая ячейка не была найдена
    if (!push) {
        if (m_players.size() == 0)
            idx = 0;
        else
            idx = m_players.size() + 1;

        p = new Player(idx, this, sock);
        m_players.push_back(p); 
    }
    else {
        p = new Player(idx, this, sock);
        m_players[idx] = p;
    } 
}
void Server::disconnect_player(Player* p)
{
    sf::Lock Lock(players_mutex);
    std::deque<Player*>::iterator itr;
    
    itr = std::find(m_players.begin(), m_players.end(), p);
    if (itr != m_players.end()) {
        auto idx = std::distance(m_players.begin(), itr);

        Player* p = m_players[idx];

        getLogger()->info(std::format(L"[p-] {}:{} disconnected",
            Utils::encoding::to_wide(p->getTcp()->getRemoteAddress().toString()),
            p->getTcp()->getRemotePort()
        ).c_str());

        delete m_players[idx];
    }
}
void Server::disconnect_player(size_t idx) {
    sf::Lock Lock(players_mutex);
    // Индекс не может быть больше чем кол-во игроков
    if (idx > m_players.size()) {
        return; 
    }
    Player* p = m_players[idx];

    getLogger()->info(std::format(L"[p-] {}:{} disconnected",
        Utils::encoding::to_wide(p->getTcp()->getRemoteAddress().toString()),
        p->getTcp()->getRemotePort()
    ).c_str());

    delete m_players[idx];
    m_players[idx] = nullptr;
}
int Server::run() {
    getLogger()->info(L"Starting server...");

    if (m_tcp_listener.listen(SERVER_DEF_PORT) != sf::Socket::Done)	getLogger()->exit_error(L"Listening TCP port error!\n");
    m_tcp_listener.setBlocking(false);
    
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT; i++) {
        m_threads.push_back(new Thread(&Server::thread_process_socket, this));
    }

    if(m_threads.size() > 0) getLogger()->info(L"Server started!");

    for (auto& t : m_threads) t->launch();          // Запуск потоков

    for (auto& t : m_threads) t->wait();            // Ждем окончания работы потоков

    for (auto p : m_players) disconnect_player(p);  // Отключаем каждого игрока

    return 0;
}
