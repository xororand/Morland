#include "enjsocket.h"
#include "mServer/Server.h"

enjsocket::enjsocket(Server* ptr_serv, TcpSocket* tcp) {
	m_server = ptr_serv;
	m_tcp = tcp;
}

enjsocket::~enjsocket() { 
	delete m_server;
}

void enjsocket::process() {
	// TODO:
	// Обработка подключенного сокета
	// Синхра / Проверка пинга
}
