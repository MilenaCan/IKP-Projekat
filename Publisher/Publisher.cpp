

#include <iostream>
#include "../Common/Publisher.h"

int __cdecl main(int argc, char** argv) {
	// variable used to store function return value
	int iResult;
	char c = ' ';

	while (c != 'a') {
		if (InitializeWindowsSockets() == false)
		{
			// we won't log anything since it will be logged
			// by InitializeWindowsSockets() function
			printf("greska je u inicijalizaciji soketa\n");//napisano samo da mi prikaze jel ovde bila greska, mozemo nekad u buducnosti obrisati
		}

		printf("proslo inicijalizaciju soketa\n");
		if (Connect() == false) {
			printf("Connection failed\n");//isto kao i gornji komentar
			return 1;
		}
		printf("konektovan je\n");
		scanf_s("%c", &c);//a ovo je samo da mi ne gasi odmah app da vidim jel ispisuje sve ovo
	}
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


