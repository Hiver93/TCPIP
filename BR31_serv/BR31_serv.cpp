//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <winsock2.h>
//#define BUF_SIZE 1024
//#define MAX_CLNT 256
//
//void ErrorHandling(const char* message);
//const char* GetCount(int input, int count);
//
//int main()
//{
//	int turn = 1;
//	int turnCount = 0;
//	const char *port = "9199";
//	char printN[6];
//	int count = 0;
//
//	WSADATA wsaData;
//	SOCKET clntSocks[MAX_CLNT];
//	int clntCnt = 0;
//	SOCKET hServSock, hClntSock;
//	SOCKADDR_IN servAdr, clntAdr;
//	TIMEVAL timeout;
//	fd_set reads, cpyReads;
//
//	int adrSz;
//	int strLen, fdNum, i;
//	char buf[BUF_SIZE];
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//
//	hServSock = socket(PF_INET, SOCK_STREAM, 0);
//	memset(&servAdr, 0, sizeof(servAdr));
//	servAdr.sin_family = AF_INET;
//	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
//	servAdr.sin_port = htons(atoi(port));
//
//	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
//		ErrorHandling("bind() error!");
//	if (listen(hServSock, 5) == SOCKET_ERROR)
//		ErrorHandling("liten() error!");
//
//	FD_ZERO(&reads);
//	FD_SET(hServSock, &reads);
//
//	while (1)
//	{
//		cpyReads = reads;
//		timeout.tv_sec = 5;
//		timeout.tv_usec = 5000;
//
//		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
//			break;
//		if (fdNum == 0)
//			continue;
//
//		for (i = 0; i < reads.fd_count; i++)
//		{
//			if (FD_ISSET(reads.fd_array[i], &cpyReads))
//			{
//				if (reads.fd_array[i] == hServSock)
//				{
//					adrSz = sizeof(clntAdr);
//					hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
//					FD_SET(hClntSock, &reads);
//					clntSocks[clntCnt++] = hClntSock;
//					printf("connected client: %d \n", hClntSock);
//					if (clntCnt == 1)
//					{
//						send(clntSocks[clntCnt], "1", 1, 0);
//					}
//					else if (clntCnt == 2)
//					{
//						send(clntSocks[clntCnt], "0", 1, 0);
//					}
//				}
//				else
//				{
//					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
//					if (strLen == 0)
//					{
//						FD_CLR(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
//						closesocket(cpyReads.fd_array[i]);
//						printf("closed client: %d \n", cpyReads.fd_array[i]);
//					}
//					else
//					{
//						if (buf[0] == 'G')
//						{
//							printf("get");
//							char cTurn[2] = {' ','\0'};
//							cTurn[0] = 1 + '0';
//							send(clntSocks[i], cTurn, 1, 0);		
//							turn ^= 3;
//						}
//						else if('0'<buf[0]&&buf[0]<='9')
//						{
//							count += buf[0] - '0';
//							printf("%s", GetCount(buf[0] - '0', count));
//							for (int i = 0; i < clntCnt; i++)
//							{
//								send(clntSocks[i], GetCount(buf[0] - '0', count), 29, 0);
//							}
//							turn ^= 3;
//						}
//					}
//				}
//			}
//		}
//	}
//	closesocket(hServSock);
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
//
//// 두자리 수 생각해야 함
//const char* GetCount(int input, int count)
//{
//	int init = count - input;
//	char output[30] = "           ";
//	int over = 0;
//	for (int i = 0; i < input; i++)
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