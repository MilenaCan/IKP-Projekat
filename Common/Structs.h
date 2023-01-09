#pragma once
#include <WinSock2.h>

typedef struct topic {
	char signal[7];
	char type[7];
	int num;
}TOPIC;

typedef struct data {
	topic topic;
	char message[500];
}DATA;

typedef struct queue {
	struct node* front;
	struct node* back;
} QUEUE;

typedef struct node {
	struct data data;
	struct node* next;
} NODE;





typedef struct socketForList {
	SOCKET acceptedSocket;
	struct socketForList* next;
}SOCKET_FOR_LIST;





