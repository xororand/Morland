#include "Server.h"

void Server::accept_new_connections() {
    TcpSocket* sock = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*sock);

    if (status == Socket::Status::Done) { 
        addPlayer(sock);
        return;
    }
    delete sock;
}
void Server::process_connections() {
    std::lock_guard<std::mutex> lock(players_mutex);

    for (auto p : m_players) {
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
            std::lock_guard<std::mutex> lock(players_mutex);
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
    process_connections();
}
void Server::thread_process(int id) {
    while (is_run) {
        std::lock_guard lock(tps_mutex);

        float TFLT = d_stats->m_tps_timer.getElapsedTime().asSeconds();

        if (TFLT < tps_treshold) {
            float slt = ( (tps_treshold - TFLT)) * 100;

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

Server::Server()
{
    setlocale(LC_ALL, "RU");
    m_logger = new Logger();

    m_packetmng = new PacketManager(this);
    m_window = new RenderWindow();

    m_window->create(sf::VideoMode(800, 500), "", sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
    m_window->setFramerateLimit(50);

    ImGui::SFML::Init(*m_window);

    ui = new servUI(this);

    d_stats = new debug_stats;
    for (int i = 0; i < 500; i++) d_stats->tps_samples[i] = 0.0f;
}

void Server::addPlayer(TcpSocket* sock) {
    std::lock_guard<std::mutex> lock(players_internal_mutex);

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
            idx = m_players.size();

        p = new Player(idx, this, sock);
        m_players.push_back(p); 
    }
    else {
        p = new Player(idx, this, sock);
        m_players[idx] = p;
    } 
}

void Server::ping_player(size_t idx)
{
    std::lock_guard<std::mutex> lock(players_internal_mutex);
    // TODO: PING PLAYER, IF NOT RESPONDE = DISCONNECT
}
void Server::disconnect_player(size_t idx) {
    // Индекс не может быть больше чем кол-во игроков
    if (idx > getPlayers().size()) return;

    if (m_players[idx] == NULL) return;

    getLogger()->info(std::format(L"[p-] {}:{} disconnected by {}",
        Utils::encoding::to_wide(m_players[idx]->getTcp()->getRemoteAddress().toString()),
        m_players[idx]->getTcp()->getRemotePort(),
        m_players[idx]->getDisconnectReason()
    ).c_str());

    delete m_players[idx];
    m_players[idx] = nullptr;
}

std::deque<Player*> Server::getPlayers()
{
    std::lock_guard<std::mutex> lock(players_internal_mutex);
    return m_players;
}

int Server::run(int ticksPerSecond) {
    getLogger()->info(L"Starting server...");

    tickInterval = std::chrono::milliseconds(1000 / ticksPerSecond);

    if (m_tcp_listener.listen(SERVER_DEF_PORT) != sf::Socket::Done)	getLogger()->exit_error(L"Listening TCP port error!\n");
    m_tcp_listener.setBlocking(false);
    
    // Запуск поток логики сервера
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT * 4; i++) {
        m_threads.push_back(new std::thread(&Server::thread_process, this, i));
    }
    
    if(m_threads.size() > 0) getLogger()->info(L"Server started!");

    // Запуск обработки UI
    drawUI();

    for (auto& t : m_threads) t->join();            // Ждем окончания работы потоков

    for (auto p : m_players) disconnect_player(p->getID());  // Отключаем каждого игрока

    return 0;
}
