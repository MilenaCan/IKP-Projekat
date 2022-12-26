#pragma once
#include <WinSock2.h>
typedef struct socketForList {
	SOCKET acceptedSocket;
	struct socketForList* next;
}SOCKET_FOR_LIST;




