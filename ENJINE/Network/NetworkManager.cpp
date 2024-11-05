#include "NetworkManager.h"

NetworkManager::Status NetworkManager::connect_server() {
	
	if (last_try_time == 0) { 
		last_try_time = time(0);
	}
	// �������� �� ����������� ���������� ����� �����������
	if (try_connect_count > TCP_MAX_TRY_TO_CONNECT) {
		// ���������� ������� ����������� ����� ���� ������
		set_status(connection_failed);
		last_try_time = 0;
		// �������� ����� ������� ��� �������� �����������
		try_connect_count = 0;
		return get_status();
	}

	TcpSocket* socket = new TcpSocket();
	socket->setBlocking(false);

	Socket::Status status = socket->connect(last_ip, last_port);

	if (status == Socket::Done) {
		this->set_status(connection_done);
		m_tcp = socket;
		return this->get_status();
	}


	if (time(0) - last_try_time >= TCP_MAX_SECS_TO_CONNECT) {
		last_try_time = time(0);
		try_connect_count++;
	}
	
	
	delete socket;
	return this->get_status();
}