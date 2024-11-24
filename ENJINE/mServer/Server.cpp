#include "Server.h"

Server::Server(float tps) {
    setlocale(LC_ALL, "RU");
    setMaxTPS(tps);

    m_logger        = new Logger();

    getLogger()->info(L"Init managers...");
    
    ui              = new servUI(this);
    m_dbmanager     = new DBManager(this);
    m_worldmanager  = new WorldManager(this);
    m_packetmng     = new PacketManager(this);
    m_window        = new RenderWindow();

    m_window->create(sf::VideoMode(800, 500), "", sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
    m_window->setFramerateLimit(50);

    ImGui::SFML::Init(*m_window);
    
    d_stats = new debug_stats;
    for (int i = 0; i < 500; i++) d_stats->tps_samples[i] = 0.0f;
}
int Server::run() {
    getLogger()->info(L"Starting server...");

    if (m_tcp_listener.listen(SERVER_DEF_PORT) != sf::Socket::Done)	getLogger()->exit_error(L"Listening TCP port error!\n");
    m_tcp_listener.setBlocking(false);

    // Запуск потоков логики сервера
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT; i++)
        m_threads.push_back(new std::thread(&Server::thread_process, this, i));

    if (m_threads.size() > 0) getLogger()->info(L"Server started!");

    // Запуск обработки UI
    drawUI();

    // Ждем окончания работы потоков
    for (auto t : m_threads) t->join();     

    // TODO: сохранение состояния мира
    
    // при отключении пира удаляется и тело игрока если оно было
    disconnectAllPeers();

    // Удаляем менеджеры
    delete m_packetmng;
    delete m_worldmanager;
    delete m_dbmanager;
    delete m_logger;
    delete m_window;

    return 0;
}




std::deque<Peer*> Server::getPeers() {
    return m_peers;
}
void Server::addPeer(TcpSocket* sock) {
    std::lock_guard<std::mutex> lock(peers_internal_mutex);

    Peer* p = nullptr;
    size_t idx = 0;
    bool push = false;

    // Поиск пустой ячейки
    for (int i = 0; i < m_peers.size(); i++) {
        if (m_peers[i] == nullptr) {
            idx = i;
            push = true;
        }
    }

    // Пустая ячейка не была найдена
    if (!push) {
        if (m_peers.size() == 0)
            idx = 0;
        else
            idx = m_peers.size();

        p = new Peer(idx, this, sock);
        m_peers.push_back(p);
    }
    else {
        p = new Peer(idx, this, sock);
        m_peers[idx] = p;
    } 
    getLogger()->info(std::format(L"[p+] {}:{} New connection...",
        Utils::encoding::to_multibytes(sock->getRemoteAddress().toString()),
        sock->getRemotePort()).c_str());
}
void Server::delPeer(sf::Uint16 idx) {
    delp(m_peers[idx])
}
void Server::disconnectPeer(sf::Uint16 idx) {
    std::lock_guard<std::mutex> lock(peers_internal_mutex);
    if ( !isPeerExists(idx) ) return;
    Peer* peer = m_peers[idx];

    getLogger()->info(std::format(L"[p-] {}:{} disconnected by {}",
        to_wide( peer->getTcp()->getRemoteAddress().toString() ),
        peer->getTcp()->getRemotePort(),
        peer->getDisconnectReason()
    ).c_str());

    peer->setStatus(Peer::status::disconnected);
    peer->getTcp()->disconnect();

    // Удаляем из объектов мира
    getWorldManager()->despawnPlayer(peer);
    // Удаляем пир и замещаем его нулевой ссылкой
    delPeer(idx);
}
void Server::disconnectAllPeers() {
    for (auto p : getPeers()) {  // Отключаем каждого игрока
        if (p == nullptr) continue;
        p->setDisconnectReason(L"disconnectAllPeers");
        disconnectPeer(p->getID());
    }
}
bool Server::isPeerExists(sf::Uint16 idx)
{
    // Индекс не может быть больше чем кол-во игроков
    if (idx > getPeers().size())    return false;
    // Объект в массиве не должно быть нулем
    if (m_peers[idx] == nullptr)       return false;

    return true;
}
bool Server::isUsernameConnected(std::wstring name)
{
    std::lock_guard<std::mutex> lock(peers_internal_mutex);
    std::deque<Peer*> peers = getPeers();
    for (int i = 0; i < peers.size(); i++) {
        Peer* peer = peers[i];
        if (peer == NULL) continue; // пропуск пустых ячеек, освобожденных после выхода игроков

        if (peer->getUsername() == name) return true;
    }
    return false;
}

void Server::setMaxTPS(float tps) { 
    m_tps_treshold = (float)(1.0 / tps); 
}




void Server::accept_new_connections() {
    TcpSocket* sock = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*sock);

    if (status == Socket::Status::Done) {
        // TODO: ПОИСК ИЗ БАЗЫ ПОЗИЦИИ ИГРОКА, ЕСЛИ ЭТО БУДЕТ НЕ СЕССИОННЫЙ ЖАНР
        addPeer(sock);
        return;
    }
    delete sock;
}
void Server::process_peers() {
    std::lock_guard<std::mutex> lock(peers_mutex);

    for (auto p : m_peers) {
        if (p == nullptr) continue;
        p->process();
    }
}

void Server::drawUI()
{
    // Draw
    while (is_run && getRenderWindow()->isOpen())
    {
        ImGui::SFML::Update(*getRenderWindow(), deltaClock.restart());
        getRenderWindow()->clear();

        {
            std::lock_guard<std::mutex> lock(peers_mutex);
            ui->drawDebug();
        }

        ImGui::SFML::Render(*getRenderWindow());
        getRenderWindow()->display();

        // Events
        sf::Event event;
        while (getRenderWindow()->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*getRenderWindow(), event);
            if (event.type == sf::Event::Closed)
                getRenderWindow()->close();
        }
    }

    ImGui::SFML::Shutdown();
}

void Server::tick()
{
    accept_new_connections();
    process_peers();
    getWorldManager()->process();
}
void Server::thread_process(int id) {
    while (is_run && getRenderWindow()->isOpen()) {
        std::lock_guard lock(tps_mutex);

        float TFLT = d_stats->m_tps_timer.getElapsedTime().asSeconds();

        if (TFLT < m_tps_treshold) {
            float slt = ((m_tps_treshold - TFLT)) * 100;

            std::this_thread::sleep_until(
                std::chrono::steady_clock::now() + std::chrono::milliseconds((long long)slt)
            );
            continue;
        }

        //TFLT = d_stats->m_tps_timer.restart().asSeconds();
        d_stats->m_current_tps = 1.0 / TFLT;

        tick(); // Обрабатываем 1 ТИК
        d_stats->m_tps_timer.restart();
    }
}