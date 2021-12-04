//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <process.h>
//#include <windows.h>
//#define BUF_SIZE 100
//#define NAME_SIZE 20
//#define ADDR "127.0.0.1"
//#define PORT "9190"
//
//
//unsigned WINAPI SendMsg(void* arg);
//unsigned WINAPI RecvMsg(void* arg);
//void ErrorHandling(char* msg);
//
//char name[NAME_SIZE] = "[DEFAULT]";
//char msg[BUF_SIZE];
//
//int main()
//{
//	WSADATA wsaData;
//	SOCKET hSock;
//	SOCKADDR_IN servAdr;
//	HANDLE hSndThread, hRcvThread;
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//	//scanf_s(&name);
//	hSock = socket(PF_INET, SOCK_STREAM, 0);
//
//	memset(&servAdr, 0, sizeof(servAdr));
//	servAdr.sin_family = AF_INET;
//	servAdr.sin_addr.s_addr = htonl(ADDR);
//	servAdr.sin_port = htons(atoi(PORT));
//
//	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
//		ErrorHandling("connect() error!");
//
//	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
//	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);
//
//	WaitForSingleObject(hSndThread, INFINITE);
//	WaitForSingleObject(hRcvThread, INFINITE);
//	closesocket(hSock);
//	WSACleanup();
//	return 0;
//}
//
//unsigned WINAPI SendMsg(void* arg)
//{
//	SOCKET hSock = *((SOCKET*)arg);
//	char nameMsg[NAME_SIZE + BUF_SIZE];
//	while (1)
//	{
//		fgets(msg, BUF_SIZE, stdin);
//		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
//		{
//			closesocket(hSock);
//			exit(0);
//		}
//		send(hSock, nameMsg, strlen(nameMsg), 0);
//	}
//	return 0;
//}
//
//unsigned WINAPI RecvMsg(void* arg)
//{
//	int hSock = *((SOCKET*)arg);
//	char nameMsg[NAME_SIZE + BUF_SIZE];
//	int strLen;
//	while (1)
//	{
//		strLen = recv(hSock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
//		if (strLen == -1)
//			return -1;
//		nameMsg[strLen] = 0;
//		fputs(nameMsg, stdout);
//	}
//	return 0;
//}
//
//void ErrorHandling(char* msg)
//{
//	fputs(msg, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}