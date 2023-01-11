
#include "../Common/Subscriber.h"

int __cdecl main(int argc, char** argv) {
	
	

	char c = ' ';

	while (c != 'a') {
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
		printf("Subscriber connected..\n");
		scanf_s("%c", &c);
	}

	
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


