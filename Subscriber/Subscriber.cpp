
#include "../Common/Subscriber.h"

int __cdecl main(int argc, char** argv) {
	
	
		int iResult;

		if (InitializeWindowsSockets() == false)
		{
			// we won't log anything since it will be logged
			// by InitializeWindowsSockets() function
			printf("Error with socket init\n");
		}

		if (Connect() == false) {
			printf("Connection failed\n");
			return 1;
		}

		char topic[] = "";
		char signal;
		char type;
		int num;
		printf("Format of the topic is signal.type.num.\n");
		printf("For signal you can choose STATUS or ANALOG.\n\t If you choose STATUS it must be FUSE or BREAKER.\n\tIf you choose ANALOG it must be SEC_A or SEC_V.\n");
		printf("NUM is the number of device.\n");
		printf("Please enter your topic: \n");
		scanf("%s", &topic);
		

	
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


