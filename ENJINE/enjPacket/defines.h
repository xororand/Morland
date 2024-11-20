#pragma once
#include "enjPacket/enjPacket.h"

#define SERVER_DEF_PORT						55055
#define SERVER_PROCESS_THREADS_COUNT		4
#define SERVER_PING_PEER_DELAY				2		// ���
#define MAX_TPS								60.0	// 1��� / 60
#define MAX_PEER_PING						5000	// MS
#define MAX_PEER_TIMEOUT					10		// ���
#define MAX_UNLOGED_PEER_TIME_CONNECTED		300		// ���
#define MAX_UNK_PACKETS						20		// ��

#define P_HEAD	0xAC
#define P_END	0xDC

/*
* INT16
* �������� 2'16 = 65.536 �������
*/
#define C_PING  0
#define C_LOGIN 1