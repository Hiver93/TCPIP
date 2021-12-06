#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>

#define BUF_SIZE 1024
#define TURN_CHECK C
#define ADDR "127.0.0.1"
#define PORT "9190"
#define ENOUGH 'E'
#define NEED_PLAYER "P"
void ErrorHandling(char* message);
void PrintCount(int count, int num);
int IsGameOver(int count);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAdr;

	char addr[20] = "192.168.25.59";
	char port[6]= "9190";

	int ID = 0;
	char myID[2];
	int count = 0;

	/*printf("Input IP address : ");	
	scanf_s("%s",addr,sizeof(addr));
	printf("Input PORT : ");
	scanf_s("%s",port,sizeof(port));*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	inet_pton(AF_INET, addr, &servAdr.sin_addr);
	servAdr.sin_port = htons(atoi(port));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected...........");

	while (1)
	{
		// ID 할당
		if (ID == 0)
		{
			printf("ID를 발급받습니다.\n");
			send(hSocket, "G", 1, 0);
			strLen = recv(hSocket, message, 2, 0);
			message[2] = 0;
			ID = message[0] - '0';			
			printf("당신의 플레이어 넘버 : %d\n",ID);
			send(hSocket, NEED_PLAYER, 1, 0);
		}
		// 할당 받은 후
		else
		{	
			// 플레이어 수 확인
			while (1)
			{
				strLen = recv(hSocket, message, 1, 0);
				if (message[0] == ENOUGH)
				{
					send(hSocket, "C", 1, 0);
					printf("2명이 되었습니다.\n(Game start)  \n");
					break;
				}				
				send(hSocket, "P", 1, 0);
			}

			// 게임시작
			while (1) // 내턴이 아닐경우 계속 체크
			{
				send(hSocket, "C", strlen("C"), 0);
				//printf("체크중");
				strLen = recv(hSocket, message, 2, 0);
				message[strLen] = 0;
				if (message[0] == 'R')
				{
					printf("상대가 나갔습니다.\n 이제 당신이 1번 플레이어 입니다.\n");
					count = 0;
					ID = 1;
					break;
				}
				else if (message[0] == '0' + ID) // 내턴이라면 (내 아이디의 턴이라면)
				{
					//printf("\n%d,  %ch\n", count, message[1]);
					if(count != 0)
					    printf("상대의 외침 : ");
					PrintCount(count, message[1] - '0');
					printf("\n");
					count += message[1] - '0';

					// 게임오버 체크
					if (IsGameOver(count))
					{
						printf("당신은 살아 남았습니다.\n 게임 다시 시작\n");
						count = 0;
						continue;
					}

					// 버퍼초기화
					long tmp_long = 0;
					char tmp_char;					
					if (ioctlsocket(hSocket, FIONREAD, &tmp_long) != SOCKET_ERROR)
					{
						for (int i = 0; i < tmp_long; i++)
						{
							recv(hSocket, &tmp_char, sizeof(char), 0);
						}
					}
					fputs("당신의 턴입니다. 1~3의 수를 입력하세요.(Q to quit): ", stdout);
					fgets(message, BUF_SIZE, stdin);
					printf("\n");

					// 나가기
					if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
					{
						send(hSocket, "Q", 1, 0);
						closesocket(hSocket);
						WSACleanup();
						return 0;
					}
					printf("당신의 외침 : ");
					PrintCount(count, message[0] - '0');
					printf("\n");
					count += (message[0]-'0');
					
					send(hSocket, message, 1, 0);
					
					// 버퍼 초기화
					tmp_long = 0;
					if (ioctlsocket(hSocket, FIONREAD, &tmp_long) != SOCKET_ERROR)
					{
						for (int i = 0; i < tmp_long; i++)
						{
							recv(hSocket, &tmp_char, sizeof(char), 0);
						}
					}

					// 게임 오버 체크
					if (IsGameOver(count))
					{
						printf("당신은 죽었다.\n 게임 다시 시작\n");
						count = 0;
						continue;
					}
					
				}
				// 버퍼초기화
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
			//send(hSocket, myID, strlen(myID), 0);
			//strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			//message[strLen] = 0;
			//if (message[0] == '1')
			//{
			//	fputs("Input message(Q to quit): ", stdout);
			//	fgets(message, BUF_SIZE, stdin);

			//	if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			//		break;

			//	send(hSocket, message, strlen(message), 0);
			//	strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			//	message[strLen] = 0;
			//	printf("Message from server: %s", message);
			//}

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
	for (int i = 1; i <= num; i++)
	{
		printf("%d ",count+i);
	}
}

int IsGameOver(int count)
{
	if (31 < count)
		return 1;
	return 0;
}