#include "Server.h"

void Server::accept_new_connections() {
    TcpSocket* sock = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*sock);

    if (status == Socket::Status::Done) {
        // TODO: ПОИСК ИЗ БАЗЫ ПОЗИЦИИ ИГРОКА, ЕСЛИ ЭТО БУДЕТ НЕ СЕССИОННЫЙ ЖАНР
        addPeer( sock );
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
    while (m_window->isOpen())
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
    //b2World_Step(m_b2worldId, m_tps_treshold, m_subStepCount);
}
void Server::thread_process(int id) {
    while (is_run) {
        std::lock_guard lock(tps_mutex);

        float TFLT = d_stats->m_tps_timer.getElapsedTime().asSeconds();

        if (TFLT < m_tps_treshold) {
            float slt = ( (m_tps_treshold - TFLT)) * 100;

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

Server::Server(float tps)
{
    setlocale(LC_ALL, "RU");
    setMaxTPS(tps);

    m_logger = new Logger();

    // СОЗДАНИЕ m_b2worldId
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2)( 0.0f, 0.0f );

    m_b2worldId = b2CreateWorld(&worldDef);

    m_packetmng = new PacketManager(this);
    m_window = new RenderWindow();

    m_window->create(sf::VideoMode(800, 500), "", sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
    m_window->setFramerateLimit(50);

    ImGui::SFML::Init(*m_window);

    ui = new servUI(this);

    d_stats = new debug_stats;
    for (int i = 0; i < 500; i++) d_stats->tps_samples[i] = 0.0f;
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
}

void Server::ping_peer(size_t idx)
{
    // TODO: PING PLAYER, IF NOT RESPONDE = DISCONNECT
}
void Server::disconnect_peer(size_t idx) {
    // Индекс не может быть больше чем кол-во игроков
    if (idx > getPeers().size()) return;

    if (m_peers[idx] == NULL) return;

    getLogger()->info(std::format(L"[p-] {}:{} disconnected by {}",
        Utils::encoding::to_wide(m_peers[idx]->getTcp()->getRemoteAddress().toString()),
        m_peers[idx]->getTcp()->getRemotePort(),
        m_peers[idx]->getDisconnectReason()
    ).c_str());

    delete m_peers[idx];
    m_peers[idx] = nullptr;
}

std::deque<Peer*> Server::getPeers()
{
    std::lock_guard<std::mutex> lock(peers_internal_mutex);
    return m_peers;
}

void Server::setMaxTPS(float tps) { 
    m_tps_treshold = (float)(1.0 / tps); 
}

int Server::run() {
    getLogger()->info(L"Starting server...");

    if (m_tcp_listener.listen(SERVER_DEF_PORT) != sf::Socket::Done)	getLogger()->exit_error(L"Listening TCP port error!\n");
    m_tcp_listener.setBlocking(false);
    
    // Запуск поток логики сервера
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT; i++) {
        m_threads.push_back(new std::thread(&Server::thread_process, this, i));
    }
    
    if(m_threads.size() > 0) getLogger()->info(L"Server started!");

    // Запуск обработки UI
    drawUI();

    for (auto& t : m_threads) t->join();            // Ждем окончания работы потоков

    for (auto p : m_peers) disconnect_peer(p->getID());  // Отключаем каждого игрока

    return 0;
}
