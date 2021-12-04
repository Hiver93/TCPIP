//#include <stdio.h>
//#include <stdlib.h>
//#include <winsock2.h>
//#include <WS2tcpip.h>
//
//#define BUF_SIZE 1024
//#define port "9199"
//void ErrorHandling(const char* message);
//
//int main(int argc, char* argv[])
//{
//	WSADATA wsaData;
//	SOCKET hSocket;
//	char message[BUF_SIZE];
//	int strLen;
//	SOCKADDR_IN servAdr;
//	
//	bool isMyTurn = 0;
//	int turn = 0;
//
//	//if (argc != 3)
//	//{
//	//	printf("Usage : %s <IP> <port>\n", argv[0]);
//	//	exit(1);
//	//}
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//
//	hSocket = socket(PF_INET, SOCK_STREAM, 0);
//	if (hSocket == INVALID_SOCKET)
//		ErrorHandling("socket() error");
//
//	memset(&servAdr, 0, sizeof(servAdr));
//	servAdr.sin_family = AF_INET;
//	inet_pton(AF_INET, "127.0.0.1", &servAdr.sin_addr);
//	servAdr.sin_port = htons(atoi(port));
//
//	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
//		ErrorHandling("connect() error!");
//	else
//		puts("Connected...........");
//
//	send(hSocket, "Get client turn", strlen("Get client turn"), 0);
//	strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
//	//turn = message[0] - '0';
//	if (message[0] == '1')
//		isMyTurn = 1;
//	else
//		isMyTurn = 0;
//	printf("%d", isMyTurn);
//	while (1)
//	{
//		if (isMyTurn)
//		{
//			fputs("Input message(Q to quit): ", stdout);
//			fgets(message, BUF_SIZE, stdin);
//
//			if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
//				break;
//
//			send(hSocket, message, strlen(message), 0);
//		}
//		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
//		message[strLen] = 0;
//		printf("Message from server: %s \n", message);
//		isMyTurn = !isMyTurn;
//	}
//	closesocket(hSocket);
//	WSACleanup();
//	return 0;
//}
//
//void ErrorHandling(const char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}