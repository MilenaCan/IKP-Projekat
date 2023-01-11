#pragma once
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Structs.h"
#include <stdio.h>
#include <ws2tcpip.h>
#include "List.h"
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 524
#define SAFE_DELETE_HANDLE(a) if(a){CloseHandle(a);} 
#define PUBLISHER_PORT "27016"
#define SUBSCRIBER_PORT "27017"

socketForList* publisherSockets = NULL;
socketForList* subscriberSockets = NULL;

HANDLE t1, t3;

DWORD thread1ID, thread3ID;

bool InitializeWindowsSockets() {
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{

		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

SOCKET InitializeListenSocket(const char* port) {

	SOCKET listenSocket = INVALID_SOCKET;
	// Prepare address information structures
	addrinfo* resultingAddress = NULL;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

	// Resolve the server address and port
	int iResult = getaddrinfo(NULL, port, &hints, &resultingAddress);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}


	// Create a SOCKET for connecting to server
						//		IPv4 address famly|stream socket | TCP protocol
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);

		WSACleanup();
		return INVALID_SOCKET;
	}

	// Setup the TCP listening socket - bind port number and local address 
	// to socket
	iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Since we don't need resultingAddress any more, free it
	freeaddrinfo(resultingAddress);
	//stavi u neblokirjauci rezim
	unsigned long mode = 1;
	iResult = ioctlsocket(listenSocket, FIONBIO, &mode);
	if (iResult != NO_ERROR) {
		printf("ioctlsocket failed with error: %ld\n", iResult);
		return INVALID_SOCKET;
	}
	return listenSocket;
}

DWORD WINAPI FunkcijaThread1(LPVOID param) {

	SOCKET listenSocketPublisher = *(SOCKET*)param;
	SOCKET acceptedSocketPublisher = INVALID_SOCKET;
	unsigned long mode = 1;
	int lastIndex = 0;

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	fd_set readfds;
	timeval timeVal;
	timeVal.tv_sec = 5;
	timeVal.tv_usec = 0;


	//MORA DA BUDE U PETLJI ZATO JE SAMO OBICNA WHILE PETLJA

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(listenSocketPublisher, &readfds);
		int selectResult = select(0, &readfds, NULL, NULL, &timeVal);

		if (selectResult == SOCKET_ERROR) {

			printf("select failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocketPublisher);
			WSACleanup();
			return 1;

		}
		else if (selectResult == 0) {
			//timeVal has expired without any action
			//OVO MOZEMO OBRISATI A I NE MORAMO KAKO HOCES
			printf("Nista se nije desilo...");
			continue;

		}
		else {
			sockaddr_in clientAddr;
			int clinetAddrSize = (sizeof(struct sockaddr_in));

			acceptedSocketPublisher = accept(listenSocketPublisher, (struct sockaddr*)&clientAddr, &clinetAddrSize);

			if (acceptedSocketPublisher == INVALID_SOCKET) {

				if (WSAGetLastError() == WSAECONNRESET) {
					printf("accept failed, because timeout for client request has expired.\n");
				}
				else
				{
					printf("accept failed with error: %d\n", WSAGetLastError());
				}

			}
			else {
				//client is succesfully accepted, we need to put socket into a nonblocking mode
				if (ioctlsocket(acceptedSocketPublisher, FIONBIO, &mode) != 0) {
					printf("ioctlsocket failed with error.");
					continue;
				}
				else {
					//we need to add socket into a list of accepted sockets
					Add(&publisherSockets, acceptedSocketPublisher);

					lastIndex++;
					printf("New Publisher request accpeted (%d). Client address: %s : %d\n", lastIndex, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
				}
			}

		}
	}
	// cleanup
	closesocket(listenSocketPublisher);

	WSACleanup();

	return 0;
}

DWORD WINAPI FunkcijaThread3(LPVOID param) {

	SOCKET listenSocketSubscriber = *(SOCKET*)param;
	SOCKET acceptedSocketSubscriber = INVALID_SOCKET;

	short lastIndexSub = 0;
	

	fd_set readfds;
	timeval timeVal;
	timeVal.tv_sec = 5;
	timeVal.tv_usec = 0;
	unsigned long mode = 1;

	socketForList* currentSocket = NULL;

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	//UBACILA SAM OBICNU WHILE PETLJU JER MORA DA BUDE U PETLJI

	while (true) {
		currentSocket = subscriberSockets;

		FD_ZERO(&readfds);

		FD_SET(listenSocketSubscriber, &readfds);
		while (currentSocket != NULL)
		{
			FD_SET(currentSocket->acceptedSocket, &readfds);
			currentSocket = currentSocket->next;
		}


		int selectResult = select(0, &readfds, NULL, NULL, &timeVal);

		if (selectResult == SOCKET_ERROR) {

			printf("select function failed with error: %d\n", WSAGetLastError());

		}
		else if (selectResult == 0) {
			//timeVal has expired and no action has happened
			//I OVO MOZEMO OBRISATI
			printf("Nije se desio nijedan slucaj...");
			continue;

		}
		else {
			if (FD_ISSET(listenSocketSubscriber, &readfds)) {
				sockaddr_in clientAddr;
				int clinetAddrSize = (sizeof(struct sockaddr_in));

				acceptedSocketSubscriber = accept(listenSocketSubscriber, (struct sockaddr*)&clientAddr, &clinetAddrSize);

				if (acceptedSocketSubscriber == INVALID_SOCKET) {

					if (WSAGetLastError() == WSAECONNRESET) {
						printf("accept failed, because timeout for client request has expired.\n");
					}

				}
				else {
					//client is succesfully accepted, we need to put socket in non blocking mode
					if (ioctlsocket(acceptedSocketSubscriber, FIONBIO, &mode) != 0) {
						printf("ioctlsocket failed with error.");

					}
					else {
						//add accepted socket into a list
						Add(&subscriberSockets, acceptedSocketSubscriber);

						lastIndexSub++;
						printf("New Subscriber request accpeted (%d). Client address: %s : %d\n", lastIndexSub, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
					}
				}
			}

		}
	}

	closesocket(listenSocketSubscriber);
	WSACleanup();
	return 0;
}

void CreateAllThreads(SOCKET* listenSocketPublisher, SOCKET* listenSocketSubscriber) {
	t1 = CreateThread(NULL, 0, &FunkcijaThread1, listenSocketPublisher, 0, &thread1ID);
	t3 = CreateThread(NULL, 0, &FunkcijaThread3, listenSocketSubscriber, 0, &thread3ID);
}