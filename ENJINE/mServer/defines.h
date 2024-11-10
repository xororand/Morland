#pragma once



#define SERVER_DEF_PORT 55055
#define SERVER_PROCESS_THREADS_COUNT 4
#define MAX_TPS 20.0 // 1��� / 60
#define MAX_PLAYER_PING		1000 // MS
#define MAX_PLAYER_TIMEOUT	10 // ������

#define P_HEAD	0xAC
#define P_END	0xDC

/*
* INT16
* �������� 2'16 = 65.536 �������
*/
#define C_PING 0