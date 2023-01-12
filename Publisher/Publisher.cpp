

#include <iostream>
#include "../Common/Publisher.h"

int __cdecl main(int argc, char** argv) {
	char c = ' ';

	while (c != 'a') {
		if (InitializeWindowsSockets() == false)
		{
			// we won't log anything since it will be logged
			// by InitializeWindowsSockets() function
			printf("Eroor with init socket\n");
		}

		if (Connect() == false) {
			printf("Connection failed\n");
			return 1;
		}
		char topic[100]="";
		char message[250]="";
		char topicToLower[100]="";
		bool resultForsignal = true;
		bool resultForType1 = true;
		bool resultForType2 = true;
		bool resultForNum = true;
		int parts_count = 0;
		char signal[100]="";
		char num[100]="";


		printf("Enter a topic: ");
		gets_s(topic, 100);
		strcpy_s(topicToLower, TopicToLower(topic));
		topicToLower[strcspn(topicToLower, "\n")] = 0;

		char** parts = separate_string(topicToLower, '.', &parts_count);
		if (parts_count != 3) {
			printf("You have to enter exactly 3 parts of topic\n");
			HeaderForEnteringTopic();
			fgets(topic, sizeof(topic), stdin);
		}
		for (int i = 0; i < parts_count; i++) {
			if (strcmp(parts[0], "status") != 0 && strcmp(parts[0], "analog") != 0) {

				resultForsignal = false;
				break;
			}
			else {
				strcpy_s(signal, parts[0]);


			}

			if (strcmp(signal, "status") == 0 && strcmp(parts[1], "fuse") != 0 && strcmp(parts[1], "breaker") != 0 && strcmp(parts[1], "*") != 0) {

				resultForType1 = false;
				break;
			}
			else if (strcmp(signal, "analog") == 0 && strcmp(parts[1], "sec_a") != 0 && strcmp(parts[1], "sec_v") != 0 && strcmp(parts[1], "*") != 0) {

				resultForType2 = false;
				break;
			}

			strcpy_s(num, parts[2]);
			int length = strlen(num);
			if (length == 1) {
				if (!isdigit(num[0]))
				{
					resultForNum = false;
				}
			}
			else {
				for (i = 0; i < length; i++)
					if (!isdigit(num[i]))
					{
						resultForNum = false;

					}

			}

		}
		if (resultForsignal == false) {
			printf("SIGNAL must be STATUS or ANALOG\n");
		}
		else if (resultForType1 == false) {
			printf("For signal STATUS type must be FUSE or BREAKER\n");
		}
		else if (resultForType2 == false) {
			printf("For signal ANALOG type must be SEC_A or SEC_V\n");
		}
		else if (resultForNum == false) {
			printf("For NUM you must enter a NUMBER\n");
		}
		else {

			printf("Enter a message: \n");
			gets_s(message, 250);
			Publish(topicToLower, message);
		}
		scanf_s("%c", &c);
	}


	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


