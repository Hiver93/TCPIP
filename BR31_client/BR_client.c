#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>

#define BUF_SIZE 1024
#define TURN_CHECK C
#define ADDR "127.0.0.1"
#define PORT "9190"
void ErrorHandling(char* message);
void PrintCount(int count, int num);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAdr;

	int ID = 0;
	char myID[2];
	int count = 0;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	inet_pton(AF_INET, ADDR, &servAdr.sin_addr);
	servAdr.sin_port = htons(atoi(PORT));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected...........");

	while (1)
	{
		// ID 할당
		if (ID == 0)
		{
			printf(" ID 줘\n");
			Sleep(1000);
			send(hSocket, "G", 1, 0);
			strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			message[strLen] = 0;
			
			ID = message[0] - '0';
			printf("당신의 ID : %d\n",ID);
			send(hSocket, "C", strlen("C"), 0);
		}
		// 할당 받은 후
		else
		{		
			while (1) // 내턴이 아닐경우 계속 체크
			{
				Sleep(1000);
				send(hSocket, "C", strlen("C"), 0);
				//printf("체크중");
				strLen = recv(hSocket, message, 2, 0);
				message[strLen] = 0;
				if (message[0] == '0' + ID) // 내턴이라면 (내 아이디의 턴이라면)
				{
					PrintCount(count, message[1] - '0');
					count += message[1] - '0';
					long tmp_long = 0;
					char tmp_char;
					if (ioctlsocket(hSocket, FIONREAD, &tmp_long) != SOCKET_ERROR)
					{
						for (int i = 0; i < tmp_long; i++)
						{
							recv(hSocket, &tmp_char, sizeof(char), 0);
						}
					}
					fputs("Input message(Q to quit): ", stdout);
					fgets(message, BUF_SIZE, stdin);

					if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
					{
						closesocket(hSocket);
						WSACleanup();
						return 0;
					}
					PrintCount(count, message[0] - '0');
					count += (message[0]-'0');
					send(hSocket, message, strlen(message), 0);
					strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
					if (ioctlsocket(hSocket, FIONREAD, &tmp_long) != SOCKET_ERROR)
					{
						for (int i = 0; i < tmp_long; i++)
						{
							recv(hSocket, &tmp_char, sizeof(char), 0);
						}
					}
				}
				long tmp_long = 0;
				char tmp_char;
				if (ioctlsocket(hSocket, FIONREAD, &tmp_long) != SOCKET_ERROR)
				{
					for (int i = 0; i < tmp_long; i++)
					{
						recv(hSocket, &tmp_char, sizeof(char), 0);
					}
				}

			}
			send(hSocket, myID, strlen(myID), 0);
			strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			message[strLen] = 0;
			if (message[0] == '1')
			{
				fputs("Input message(Q to quit): ", stdout);
				fgets(message, BUF_SIZE, stdin);

				if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
					break;

				send(hSocket, message, strlen(message), 0);
				strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
				message[strLen] = 0;
				printf("Message from server: %s", message);
			}

			//fputs("Input message(Q to quit): ", stdout);
			//fgets(message, BUF_SIZE, stdin);

			//if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			//	break;

			//send(hSocket, message, strlen(message), 0);
			//strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			//message[strLen] = 0;
			//printf("Message from server: %s", message);
		}
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Check(char* buf)
{
	int i = 0;
	int c = 0;
	while (buf[i] == '\0')
	{
		c++;
		i++;
	}
}

void PrintCount(int count, int num)
{
	printf("\n");
	for (int i = 1; i <= num; i++)
	{
		printf("%d ",count+i);
	}
}