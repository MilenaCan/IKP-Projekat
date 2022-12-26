// PubSubEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include "../Common/PubSubEngine.h"
#include <iostream>


int main(void)
{
	// Socket used for listening for new clients 
	SOCKET listenSocketPublisher = INVALID_SOCKET;
	SOCKET listenSocketSubscriber = INVALID_SOCKET;
	int iResult, IResultSubscriber;



	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	listenSocketPublisher = InitializeListenSocket(PUBLISHER_PORT);
	if (listenSocketPublisher == SOCKET_ERROR || listenSocketPublisher == INVALID_SOCKET) return 1;
	listenSocketSubscriber = InitializeListenSocket(SUBSCRIBER_PORT);
	if (listenSocketSubscriber == SOCKET_ERROR || listenSocketSubscriber == INVALID_SOCKET) return 1;

	// Set listenSocket in listening mode
	iResult = listen(listenSocketPublisher, SOMAXCONN);
	IResultSubscriber = listen(listenSocketSubscriber, SOMAXCONN);

	if (iResult == SOCKET_ERROR || IResultSubscriber == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocketPublisher);
		closesocket(listenSocketSubscriber);
		WSACleanup();
		return 1;
	}

	printf("Server initialized, waiting for clients.\n");

	CreateAllThreads(&listenSocketPublisher, &listenSocketSubscriber);

	if (t1) {
		WaitForSingleObject(t1, INFINITE);
	}
	if (t3) {
		WaitForSingleObject(t3, INFINITE);
	}

	CloseAllSocketsFromList(publisherSockets);
	CloseAllSocketsFromList(subscriberSockets);

	deleteList(&publisherSockets); // oslobodi memoriju koju je zauzela lista
	deleteList(&subscriberSockets);

	closesocket(listenSocketPublisher);
	closesocket(listenSocketSubscriber);
	listenSocketPublisher = INVALID_SOCKET;
	listenSocketSubscriber = INVALID_SOCKET;
	WSACleanup();

	return 0;

}


