#pragma once
#include "enjPacket/enjPacket.h"

#define SERVER_DEF_PORT						55055
#define SERVER_PROCESS_THREADS_COUNT		std::thread::hardware_concurrency()
#define SERVER_PING_PEER_DELAY				2		// ���
#define MAX_TPS								60.0	// TPS
#define MAX_PEER_PING						5000	// MS
#define MAX_PEER_TIMEOUT					10		// ���
#define MAX_UNLOGED_PEER_TIME_CONNECTED		300		// ���
#define MAX_ONLY_PING_PACKETS_TIMEOUT		1800	// ��� ��� AFK
#define MAX_UNK_PACKETS						20		// ��
#define MAX_FAILED_LOGINS					10		// ��

#define P_HEAD		0xAC
#define P_END		0xDC
#define P_SUCCESS	0x01
#define P_FAIL		0x00

/*
* INT16
* �������� 2'16 = 65.536 �������
*/
#define C_PING				0
#define C_REGISTER_USER		1 // [wstr]username, [wstr]password
#define C_LOGIN_USER		2 // [wstr]username, [wstr]password
#define C_SYNC_READY		3