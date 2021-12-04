#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 30
#define NEED_PLAYER 'P'
#define TURN_CHECK 'C'
#define ADDR "127.0.0.1"
#define PORT "9190"
void ErrorHandling(char* message);
void Check(char* buf);
int IsGameOver(int count);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	SOCKET socks[100];
	int sockCnt = 0;
	int sockNum = 0;
	
	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	int ID = 1; // Ŭ���̾�Ʈ�� �Ҵ��� ID
	int count = 0; // ���� ���� ī��Ʈ
	int check = 0; // ��ȣ�� �޾Ҵ°�
	int turn = 1; // ���� ��
	int num=0; // Ŭ���̾�Ʈ���Լ� ���� ��
	int GameOver = 0;

	
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

	// ������
	while (1)
	{
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdNum == 0)
			continue;

		// ���� ��ũ���� Ȯ��
		for (i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == hServSock) // ���ο� ����
				{
					adrSz = sizeof(clntAdr);
					hClntSock =
						accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					socks[sockNum] = hClntSock;
					sockNum++;
					sockCnt++;
					FD_SET(hClntSock, &reads);
					printf("connected client: %d \n", hClntSock);
				}
				else // ������ ����
				{
					strLen = recv(reads.fd_array[i], buf, 1, 0);
					if (buf[0]=='G') // ���Ͽ� id �Ҵ�
					{
						printf("�Ҵ� �� %d\n", ID);
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						printf("�ȳ�\n");
						char message[3];
						sprintf_s(message, sizeof(message), "%d%d", ID,0);
						printf("%d\n", ID);
						printf("�Ҵ� �߰� %c\n", message[0]);
						ID++;
						send(reads.fd_array[i], message, 2, 0);
						printf("�Ҵ� �� %d\n", ID);
					}

					// �÷��̾� �� Ȯ��
					else if (buf[0] == NEED_PLAYER)
					{
						char message[2] = "N";
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						if (sockCnt == 2)
						{
							message[0] = 'E';
						}
						send(reads.fd_array[i], message, 1, 0);
					}

					// ��ȣ�� ����
					else if (buf[0] <= '9' && '1' <= buf[0]) 
					{					
						num = buf[0] - '0';
						count += buf[0] - '0'; // ī��Ʈ �� ����
						long tmp_long = 0;
						char tmp_char;
						if (ioctlsocket(hServSock, FIONREAD, &tmp_long) != SOCKET_ERROR)
						{
							for (int i = 0; i < tmp_long; i++)
							{
								recv(hServSock, &tmp_char, sizeof(char), 0);
							}
						}
						check = 1; // ��ȣ �������·� ����
						turn ^= 3; // �Ϻ���
						char message[2];
						sprintf_s(message, sizeof(message), "%d", turn);
						send(reads.fd_array[i], message, 1, 0);
					}

					// Ŭ���̾�Ʈ���� �� �䱸
					else if (buf[0] == TURN_CHECK) 
					{
						 check = 0; // ��ȣ ������ ���·� ����
						 char message[3];
						 sprintf_s(message, sizeof(message), "%d%d", turn, num);
						 
						 send(reads.fd_array[i], message, 2, 0);
						 if (IsGameOver(count))
						 {
						 	 count = 0;
						 	 num = 0;
						 }
					}

					// ��� ���� ����
					else if (buf[0]=='Q') // �������
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
						
						// Ŭ���̾�Ʈ ���°� ó������ ���ư��� ����
						sockCnt--;
						count = 0;
						ID--;
						num = 0;
						for(int i = 0; i<sockNum;i++)
						    send(socks[i], "R", 1, 0);

						printf("closed client: %d \n", cpyReads.fd_array[i]);
					}
					else // ����
					{
					    char message[3] = "  ";
						//printf("%s", buf);
						printf("���� ���� �ȵ�");
						//message[0] = turn + '0';
						//message[1] = num + '0';
						//printf("%c %c", message[0], message[1]);
						//send(reads.fd_array[i], message, 2, 0);
						//printf("%d", strcmp("GetID\n", buf));
						Check(buf);
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
int IsGameOver(int count)
{
	if (31 < count)
		return 1;
	return 0;
}