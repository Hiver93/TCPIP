//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <windows.h>
//#include <process.h>
//
//#define BUF_SIZE 100
//#define MAX_CLNT 256
//#define PORT "9190"
//
//unsigned WINAPI HandleClnt(void* arg);
//void SendMsg(char* msg, int len);
//void ErrorHandling(char* msg);
//char* GetCount(char* input);
//
//int count = 0;
//int clntCnt = 0;
//SOCKET clntSocks[MAX_CLNT];
//HANDLE hMutex;
//
//int main()
//{
//	WSADATA wsaData;
//	SOCKET hServSock, hClntSock;
//	SOCKADDR_IN servAdr, clntAdr;
//	int clntAdrSz;
//	HANDLE hThread;
//	
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//
//	hMutex = CreateMutex(NULL, FALSE, NULL);
//	hServSock = socket(PF_INET, SOCK_STREAM, 0);
//	
//	memset(&servAdr, 0, sizeof(servAdr));
//	servAdr.sin_family = AF_INET;
//	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
//	servAdr.sin_port = htons(atoi(PORT));
//
//	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
//		ErrorHandling("bind() error");
//	if (listen(hServSock, 5) == SOCKET_ERROR)
//		ErrorHandling("listen() error");
//
//	while (1)
//	{
//		clntAdrSz = sizeof(clntAdr);
//		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
//
//		WaitForSingleObject(hMutex, INFINITE);
//		clntSocks[clntCnt++] = hClntSock;
//		ReleaseMutex(hMutex);
//		hThread =
//			(HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
//		printf("Connected client IP:%s \n", inet_ntoa(clntAdr.sin_addr));
//	}
//	closesocket(hServSock);
//	WSACleanup();
//	return 0;
//}
//
//unsigned WINAPI HandleClnt(void* arg)
//{
//	SOCKET hClntSock = *((SOCKET*)arg);
//	int strLen = 0, i;
//	char msg[BUF_SIZE];
//
//	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
//	{
//		GetCount(msg);
//		SendMsg(msg, strlen(msg));
//	}
//
//	WaitForSingleObject(hMutex, INFINITE);
//	for (i = 0; i < clntCnt; i++)
//	{
//		if (hClntSock == clntSocks[i])
//		{
//			while (i++ < clntCnt - 1)
//				clntSocks[i] = clntSocks[i + 1];
//			break;
//		}
//	}
//	clntCnt--;
//	ReleaseMutex(hMutex);
//	closesocket(hClntSock);
//	return 0;
//}
//
//void SendMsg(char* msg, int len)
//{
//	int i;
//	WaitForSingleObject(hMutex, INFINITE);
//	for (i = 0; i < clntCnt; i++)
//		send(clntSocks[i], msg, len, 0);
//	ReleaseMutex(hMutex);
//}
//void ErrorHandling(char* msg)
//{
//	fputs(msg, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
//
//char* GetCount(char* input)
//{
//	int init = count - (input[0]-'0');
//	char output[30] = "           ";
//	int over = 0;
//	for (int i = 0; i < input[0] - '0'; i++)
//	{
//		if (9 < init + 1 + i)
//		{
//			output[i * 2 + over] = (init + 1 + i) / 10 + '0';
//			output[i * 2 + over + 1] = (init + 1 + i) - ((init + 1 + i) / 10)*10 + '0';
//			over++;
//		}
//		else
//			output[i * 2] = (init + 1 + i) + '0';
//	}
//	output[28] = '\n';
//
//	return output;
//}
//
