#include "Server.h"

void Server::accept_new_connections() {
    TcpSocket* sock = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*sock);

    if (status == Socket::Status::Done) {
        // TODO: ����� �� ���� ������� ������, ���� ��� ����� �� ���������� ����
        addPlayer( sock, b2Vec2(0,0) );
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
        
        tick(); // ������������ 1 ���
        d_stats->m_tps_timer.restart();
    }
}

Server::Server(float tps)
{
    setlocale(LC_ALL, "RU");
    setMaxTPS(tps);

    m_logger = new Logger();

    // �������� m_b2worldId
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

void Server::addPlayer(TcpSocket* sock, b2Vec2 pos) {
    std::lock_guard<std::mutex> lock(players_internal_mutex);

    Player* p = nullptr;
    size_t idx = 0;
    bool push = false;

    // ����� ������ ������
    for (int i = 0; i < m_players.size(); i++) {
        if (m_players[i] == nullptr) {
            idx = i;
            push = true;
        }
    }

    // ������ ������ �� ���� �������
    if (!push) {
        if (m_players.size() == 0)
            idx = 0;
        else
            idx = m_players.size();

        p = new Player(idx, this, sock, pos);
        m_players.push_back(p); 
    }
    else {
        p = new Player(idx, this, sock, pos);
        m_players[idx] = p;
    } 
}

void Server::ping_player(size_t idx)
{
    // TODO: PING PLAYER, IF NOT RESPONDE = DISCONNECT
}
void Server::disconnect_player(size_t idx) {
    // ������ �� ����� ���� ������ ��� ���-�� �������
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

void Server::setMaxTPS(float tps) { 
    m_tps_treshold = (float)(1.0 / tps); 
}

int Server::run() {
    getLogger()->info(L"Starting server...");

    if (m_tcp_listener.listen(SERVER_DEF_PORT) != sf::Socket::Done)	getLogger()->exit_error(L"Listening TCP port error!\n");
    m_tcp_listener.setBlocking(false);
    
    // ������ ����� ������ �������
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT; i++) {
        m_threads.push_back(new std::thread(&Server::thread_process, this, i));
    }
    
    if(m_threads.size() > 0) getLogger()->info(L"Server started!");

    // ������ ��������� UI
    drawUI();

    for (auto& t : m_threads) t->join();            // ���� ��������� ������ �������

    for (auto p : m_players) disconnect_player(p->getID());  // ��������� ������� ������

    return 0;
}
