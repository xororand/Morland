#include "Peer.h"
#include "Peer.h"
#include "mServer/Server.h"

Peer::Peer(size_t idx, Server* serv, TcpSocket* tcp) {
	this->idx = idx;

	time(&first_connect_t);
	time(&last_packet_t);
	last_ping_ms = std::chrono::system_clock::now();

	m_server = serv;
	m_tcp = tcp;
	m_tcp->setBlocking(false);

	setStatus(status::not_verifed);
	// TODO: BODY SHAPE DEF
	getServer()->getLogger()->info(std::format(L"[p+] {}:{} New connection...", 
		Utils::encoding::to_wide(tcp->getRemoteAddress().toString()), 
		tcp->getRemotePort() ).c_str() );
}

Peer::~Peer() {
	delete m_tcp;
}

// Îáðàáîòêà îòäåëüíîãî èãðîêà â 1 ÒÈÊ Ñåðâåðà
void Peer::process() {
	Server* serv = getServer();

	time_t timeout = time(0) - last_packet_t;
	// ÒÀÉÌÀÓÒ ÈÑÒÅÊ
	if (timeout > MAX_PLAYER_TIMEOUT)	{ disconnect_reason = L"TIMEOUT"; setStatus(status::disconnected); }
	// ÑËÈØÊÎÌ ÁÎËÜØÎÉ ÏÈÍÃ
	if (ping_ms >= MAX_PLAYER_PING)		{ disconnect_reason = L"HIGH PING"; setStatus(status::disconnected); }
	// ÑËÈØÊÎÌ ÁÎËÜØÎÉ ÒÀÉÌÀÓÒ ÏÀÊÅÒÎÂ - ÍÀ×ÈÍÀÅÌ ÎÒÏÐÀÂÊÓ ÏÈÍÃÀ
	if (timeout >= MAX_PLAYER_TIMEOUT / 2) serv->ping_peer(this->getID()); 
	// ÎÒÊËÞ×ÀÅÌ ÏÎ ÏÐÈ×ÈÍÅ ÂÛÑÎÊÎÃÎ ÒÀÉÌÀÓÒÀ ÏÀÊÅÒÎÂ
	if (this->getStatus() == Peer::status::disconnected) {
		disconnect();
		return;
	}

	// ÏÐÈÍÈÌÀÅÌ TCP ÏÀÊÅÒÛ
	Socket::Status tcp_status = (Socket::Status)serv->getPacketManager()->process_packet(this);

	if (tcp_status == Socket::Status::Done) last_packet_t = time(0);
	if (tcp_status == Socket::Status::Disconnected) {
		disconnect_reason = L"Disconnected"; setStatus(status::disconnected);
	}
	if (tcp_status == Socket::Status::Error)
	{ disconnect_reason = L"Receive TCP failed"; setStatus(status::disconnected); }


}

std::wstring Peer::to_wstring(status s)
{
	switch (s) {
	case disconnected:	return L"Disconnected";
	case not_verifed:	return L"Not verifed";
	case verifed:		return L"Verifed";
	case logged_in:		return L"Logged in";
	default:			return L"UNK_STR";
	}
	return L"UNK_STR";
}

void Peer::disconnect()
{
	getServer()->disconnect_peer(this->getID());
}
