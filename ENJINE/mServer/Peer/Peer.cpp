#include "Peer.h"
#include "mServer/Server.h"

Peer::Peer(size_t idx, Server* serv, TcpSocket* tcp) {
	this->idx = idx;

	time(&first_connect_t);
	time(&last_packet_t);
	ping_sent_t = std::chrono::system_clock::now();

	m_server = serv;
	m_tcp = tcp;
	m_tcp->setBlocking(false);

	setStatus(status::not_verifed);
	// TODO: BODY SHAPE DEF
}

Peer::~Peer() {
	delete m_tcp;
}

// ��������� ���������� ���� � 1 ��� �������
void Peer::process() {
	Server* serv = getServer();

	time_t lp_timeout	= time(0) - last_packet_t;
	time_t lpp_timeout	= time(0) - last_ping_t;
	time_t fc_timeout	= time(0) - first_connect_t;

	// ������������ ��������� ����� ��� ����� ������/������
	if (getStatus() != logged_in and fc_timeout >= MAX_UNLOGED_PEER_TIME_CONNECTED) { setDisconnectReason(L"Logins time expired"); setStatus(status::disconnected); }
	// ������� �����
	if (lp_timeout > MAX_PEER_TIMEOUT)			{ setDisconnectReason(L"TIMEOUT"); setStatus(status::disconnected); }
	// ������� ������� ����
	if (ping_ms >= MAX_PEER_PING)				{ setDisconnectReason(L"HIGH PING"); setStatus(status::disconnected); }
	// ������� ����� ������������ �� ��������� �������� 
	if (unk_packets_c >= MAX_UNK_PACKETS)		{ setDisconnectReason(L"UNK Packets limit"); setStatus(status::disconnected); }
	// ����� ��������� ������� ������������
	if (failed_logins_c >= MAX_FAILED_LOGINS)	{ setDisconnectReason(L"Login failed"); setStatus(status::disconnected); }
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
	getServer()->disconnectPeer(this->getID());
}

void Peer::ping()
{
	getServer()->getPacketManager()->c_ping(this);
}
