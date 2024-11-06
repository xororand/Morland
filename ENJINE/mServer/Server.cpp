#include "Server.h"

void Server::accept_new_connections()
{
    TcpSocket* temp = new TcpSocket();
    Socket::Status status = m_tcp_listener.accept(*temp);

    if (status == Socket::Status::Done) { 
        enjsocket enj_sock = enjsocket(this, temp);
        m_sockets.push_back(enj_sock);
    }
}

void Server::process_connections() {
    for (auto sock : m_sockets) {
        sock.process();
    }
}

void Server::tick()
{
    getLogger().info(std::format(L"TPS: {}", getCurrentTPS()).c_str());
    accept_new_connections();
    process_connections();
}

void Server::thread_process_socket() {
    while (is_start) {
        tps_mutex.lock();
        float TFLT = m_tps_timer.getElapsedTime().asSeconds();
        tps_mutex.unlock();
        float coef_tps = (1.0 / getCurrentTPS()) / m_threads.size();
        float tps_treshold = (float)(1.0 / MAX_TPS) * m_threads.size() - coef_tps;


        if (TFLT < tps_treshold) {
            float t_sleep = (float)((tps_treshold - TFLT));
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)t_sleep));
            continue;
        }

        
        tick(); // Обрабатываем 1 ТИК

        m_tps_timer.restart();
        m_current_tps = 1.0 / TFLT * m_threads.size();
    }
}
int Server::run()
{
    m_tcp_listener.listen(SERVER_DEF_PORT);
    m_tcp_listener.setBlocking(false);
    
    for (int i = 0; i < SERVER_PROCESS_THREADS_COUNT; i++) {
        m_threads.push_back(std::thread(&Server::thread_process_socket, this));
    }

    for (auto& t : m_threads) t.join();

    return 0;
}
