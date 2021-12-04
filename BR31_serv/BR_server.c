#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 30
#define TURN_CHECK 'C'
#define ADDR "127.0.0.1"
#define PORT "9190"
void ErrorHandling(char* message);
void Check(char* buf);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	
	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	int ID = 1; // 클라이언트에 할당할 ID
	int count = 0; // 현재 게임 카운트
	int check = 0; // 번호를 받았는가
	int turn = 1; // 현재 턴
	int num=0; // 클라이언트에게서 받은 값

	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(PORT));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");
	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	// 사용시작
	while (1)
	{
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdNum == 0)
			continue;

		// 받은 디스크립터 확인
		for (i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock) // 새로운 소켓
				{
					adrSz = sizeof(clntAdr);
					hClntSock =
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);
					printf("connected client: %d \n", hClntSock);
				}
				else // 데이터 수신
				{
					strLen = recv(reads.fd_array[i], buf, 1, 0);
					if (buf[0]=='G') // 소켓에 id 할당
					{
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						printf("안녕");
						char message[2];
						sprintf_s(message, sizeof(message), "%d", ID);
						// 버퍼비우기
						
						send(reads.fd_array[i], message, strlen(message), 0);
						ID++;
					}
					// 번호를 받음
					else if (buf[0] <= '3' && '1' <= buf[0]) 
					{					
						num = buf[0] - '0';
						count += buf[0] - '0'; // 카운트 값 증가
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						check = 1; // 번호 받은상태로 변경
						turn ^= 3; // 턴변경
						char message[2];
						sprintf_s(message, sizeof(message), "%d", turn);
						send(reads.fd_array[i], message, strlen(message), 0);
					}
					// 클라이언트에서 턴 요구
					else if (buf[0] == TURN_CHECK) 
					{
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						if (!check) // 아직 상대에게서 번호를 받지 못한경우
						{
							char message[3];
							sprintf_s(message, sizeof(message), "%d""%d", turn, num);
							send(reads.fd_array[i], message, strlen(message), 0);
						}
						else // 상대에게서 번호를 받은 경우
						{
							check = 0; // 번호 못받은 상태로 변경
							char message[3];
							sprintf_s(message, sizeof(message), "%d""%d", turn, num);
							send(reads.fd_array[i], message, strlen(message), 0);
						}
					}
					else if (strLen == 0) // 연결끊기
					{
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %d \n", cpyReads.fd_array[i]);
					}
					else // 오류
					{
						printf("여기 오면 안됨");
						printf("%d", strcmp("GetID\n", buf));
						Check(buf);
						send(reads.fd_array[i], buf, strLen, 0);
					}
				}

			}
		}
	}
	closesocket(hServSock);
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
	while (buf[i] != '\0')
	{
		c++;
		i++;
	}
	printf("%d", c);
}