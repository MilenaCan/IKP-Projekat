
#include "../Common/Subscriber.h"
#include <ctype.h>

int __cdecl main(int argc, char** argv) {
	
	
		int iResult;
		int broj;
		

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
		else {
			while (true) {
				bool resultForsignal = true;
				bool resultForType1 = true;
				bool resultForType2 = true;
				bool resultForNum = true;
				char topic[100];
				char topicToLower[100];
				int parts_count = 0;
				char signal[100];
				char num[100];

				HeaderForEnteringTopic();
				fgets(topic, sizeof(topic), stdin);
				strcpy(topicToLower, TopicToLower(topic));
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
						strcpy(signal, parts[0]);


					}

					if (strcmp(signal, "status") == 0 && strcmp(parts[1], "fuse") != 0 && strcmp(parts[1], "breaker") != 0) {

						resultForType1 = false;
						break;
					}
					else if (strcmp(signal, "analog") == 0 && strcmp(parts[1], "sec_a") != 0 && strcmp(parts[1], "sec_v") != 0) {

						resultForType2 = false;
						break;
					}

					strcpy(num, parts[2]);
					int length = strlen(num);
					printf("%s", num);
					printf("length : %d", length);
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
					printf("For NUM you must enter a NUMBER");
				}
				else {
					Subscribe((void *)topicToLower);
					
					scanf_s("%d", &num);
				}
				
				
			}
			
		}
	
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}


