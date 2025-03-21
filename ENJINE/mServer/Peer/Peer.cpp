#include "Peer.h"
#include "mServer/Server.h"

Peer::Peer(sf::Uint16 idx, Server* serv, TcpSocket* tcp) {
	this->m_pid = idx;

	time(&first_connect_t);
	time(&last_packet_t);
	time(&last_ping_t);
	time(&last_not_ping_packet_t);
	ping_sent_t = std::chrono::system_clock::now();

	m_server = serv;
	m_tcp = tcp;
	m_tcp->setBlocking(false);

	setStatus(status::not_verifed);

	// �������� ��������������� ���� ��� SERVER_PING_PEER_DELAY ����� ����� ������������
	ping();
}

Peer::~Peer() {
	delete m_tcp; m_tcp = nullptr;
	delete m_playerobj; m_playerobj = nullptr;
}

// ��������� ���������� ���� � 1 ��� �������
void Peer::process() {
	Server* serv = getServer();

	time_t lp_timeout	= time(0) - last_packet_t;
	time_t lpp_timeout	= time(0) - last_ping_t;
	time_t fc_timeout	= time(0) - first_connect_t;
	time_t lnpp_timeout	= time(0) - last_not_ping_packet_t;

	// ������������ ��������� ����� ��� ����� ������/������
	if (getStatus() != logged_in and fc_timeout >= MAX_UNLOGED_PEER_TIME_CONNECTED) { setDisconnectReason(L"Logins time expired"); setStatus(status::disconnected); }
	// ������� �����
	if (lp_timeout > MAX_PEER_TIMEOUT)					{ setDisconnectReason(L"TIMEOUT");			setStatus(status::disconnected); }
	// ������� ������� ����
	if (ping_ms >= MAX_PEER_PING)						{ setDisconnectReason(L"HIGH PING");		setStatus(status::disconnected); }
	// ������� ����� ������������ �� ��������� �������� 
	if (unk_packets_c >= MAX_UNK_PACKETS)				{ setDisconnectReason(L"UNK Packets limit"); setStatus(status::disconnected); }
	// ����� ��������� ������� ������������
	if (failed_logins_c >= MAX_FAILED_LOGINS)			{ setDisconnectReason(L"Failed logins limit");		setStatus(status::disconnected); }
	// ���� ������ ����� ��������� ������ PING ������ - ��� TIMEOUT
	if (lnpp_timeout >= MAX_ONLY_PING_PACKETS_TIMEOUT && getStatus() == logged_in)	{ setDisconnectReason(L"AFK"); setStatus(status::disconnected); }
	// ��������� �� ������� �������
	if (this->getStatus() == Peer::status::disconnected) { disconnect(); return; }

	// ������� ������� ������ n ������
	if (lpp_timeout >= SERVER_PING_PEER_DELAY) ping();

	// ��������� TCP ������
	Socket::Status tcp_status = (Socket::Status)( serv->getPacketManager()->process_packet(this) );

	if (tcp_status == Socket::Status::Done) last_packet_t = time(0);
	if (tcp_status == Socket::Status::Disconnected) {
		setDisconnectReason(L"Disconnected");
		setStatus(status::disconnected);
	}
	if (tcp_status == Socket::Status::Error)		{ 
		setDisconnectReason(L"Receive TCP failed");
		setStatus(status::disconnected); 
	}

	// �������������� ����
	for (auto peer : sync_peers) { 
		S_PlayerObj* pobj = peer->getPlayerObj();
		if (pobj == nullptr) continue;
		getServer()->syncPeerByPeer(this->getID(), peer->getID()); 
	}
}

void Peer::onPeerAdd2Sync(Peer* peer) {
	if (peer == nullptr) return;
	if (peer == this) return;

	sync_peers.push_back(peer);

	getServer()->getLogger()->info( std::format(L"[sync] {} += {}", getUsername(), peer->getUsername()).c_str() );
}

void Peer::onPeerRemove2Sync(Peer* peer) {
	if (peer == nullptr) return;
	if (peer == this) return;

	auto it = std::find(sync_peers.begin(), sync_peers.end(), peer);

	if (it == sync_peers.end()) return;

	sync_peers.erase(it);

	getServer()->getLogger()->info( std::format(L"[sync] {} -= {}", getUsername(), peer->getUsername()).c_str() );
}

std::wstring Peer::to_wstring(status s)
{
	switch (s) {
	case disconnected:	return L"Disconnected";
	case not_verifed:	return L"Not verifed";
	case verifed:		return L"Verifed";
	case logged_in:		return L"Logged in";
	case sync_ready:	return L"SYNC";
	default:			return L"UNK_STR";
	}
	return L"UNK_STR";
}

void Peer::disconnect()
{
	getServer()->disconnectPeer(this->getID());
}
void Peer::ping()
{
	getServer()->getPacketManager()->c_ping(this);
}
