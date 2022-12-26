
#include "../Common/Subscriber.h"

int __cdecl main(int argc, char** argv) {
	// variable used to store function return value
	int iResult;


	char c = ' ';

	while (c != 'a') {
		if (InitializeWindowsSockets() == false)
		{
			// we won't log anything since it will be logged
			// by InitializeWindowsSockets() function
			printf("greska je u inicijalizaciji soketa\n");//provera da li je greska bila ovde
		}

		printf("proslo inicijalizaciju soketa\n");//nije xD
		if (Connect() == false) {
			printf("nije proslo konektovanje\n");//provera jel problem negde u connectu
			return 1;
		}
		printf("konektovan je\n");// nije
		scanf_s("%c", &c);//ovo je tu samo da mi se ne bi odmah gasila app za sada
	}

	// cleanup
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


