#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define MAXNUM 8
SOCKET client_fds[MAXNUM];

// stdcall���̴߳�����
DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);

int main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		cout << "WSAStartup Error:" << WSAGetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "WSA�����ɹ���" << endl;
	}

	// ������ʽ�׽���
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "socket error:" << WSAGetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "��ʽ�׽��ִ����ɹ���" << endl;
	}

	// �󶨶˿ں�ip
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int len = sizeof(sockaddr_in);
	if (bind(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "bind Error:" << WSAGetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "IP��˿ڰ󶨳ɹ���" << endl;
	}

	// ����
	listen(s, 10);
	cout << "�������������ɹ���" << endl;
	// ���߳�ѭ�����տͻ��˵�����
	while (true)
	{
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		// ���ܳɹ�������clientͨѶ��Socket
		SOCKET c = accept(s, (SOCKADDR*)&addrClient, &len);
		if (c != INVALID_SOCKET)
		{
			int index = -1;
			for (int i = 0; i < MAXNUM; i++)
			{
				if (client_fds[i] == 0)
				{
					index = i;
					client_fds[i] = c;
					break;
				}
			}
			// �����̣߳����Ҵ�����clientͨѶ���׽���
			HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)c, 0, NULL);
			CloseHandle(hThread); // �رն��̵߳�����
		}

	}

	// �رռ����׽���
	closesocket(s);

	// ����winsock2�Ļ���
	WSACleanup();

	return 0;
}

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter)
{
	// ��ͻ���ͨѶ�����ͻ��߽�������
	SOCKET c = (SOCKET)lpThreadParameter;

	cout << "��ӭ" << c << "���������ң�" << endl;

	// ��������
	char buf[100] = { 0 };
	sprintf(buf, "��ӭ %d ���������ң�", c);
	send(c, buf, 100, 0);

	// ѭ�����տͻ�������
	int ret = 0;
	do
	{
		char buf2[100] = { 0 };
		send(c, buf2, 100, 0);
		ret = recv(c, buf2, 100, 0);
		// ������������ַ�����ͷΪ��to��ʱ����˽��ģʽ
		if (buf2[0] == 't' && buf2[1] == 'o')
		{
			string s = to_string(c);
			char* temp = const_cast<char*>(s.c_str());
			char temp2[] = "˵��";
			char temp3[] = "[˽��]";
			char buf3[100] = "";
			strcpy(buf3, temp3);
			strcat(buf3, temp);
			strcat(buf3, temp2);
			// ����Ҫ˽�ĵ��û��˿�
			int id = 0;
			int index = 2;
			for (index = 2;(buf2[index] <= '9' && buf2[index] >= '0');index++)
			{
				id *= 10;
				id += (buf2[index] - '0');
			}
			index++;
			char buf4[100] = { 0 };
			for (int i = index;i < 100;i++)
			{
				buf4[i - index] = buf2[i];
			}
			strcat(buf3, buf4);
			for (int i = 0;i < MAXNUM;i++)
			{
				if (client_fds[i] != 0 && client_fds[i] == id)
				{
					send(client_fds[i], buf3, 100, 0);
				}
			}
		}
		// �о������������ҵ��û�
		else if (buf2[0] == 'u' && buf2[1] == 's' && buf2[2] == 'e' && buf2[3] == 'r' && buf2[4] == 's')
		{
			char temp[] = "�����û��У�\n";
			char temp1[] = "\n";
			char temp3[] = "    ";
			char buf3[100] = "";
			strcpy(buf3, temp);
			for (int i = 0;i < MAXNUM;i++)
			{
				if (client_fds[i] != 0)
				{
					string s = to_string(client_fds[i]);
					char* temp2 = const_cast<char*>(s.c_str());
					strcat(buf3, temp3);
					strcat(buf3, temp2);
					strcat(buf3, temp1);
				}
			}
			send(c, buf3, 100, 0);
		}
		// ����
		else
		{
			string s = to_string(c);
			char* temp = const_cast<char*>(s.c_str());
			char temp2[] = "˵��";
			char buf3[100] = "";
			strcpy(buf3, temp);
			strcat(buf3, temp2);
			strcat(buf3, buf2);
			for (int i = 0;i < MAXNUM;i++)
			{
				if (client_fds[i] != 0 && client_fds[i] != c)
				{
					send(client_fds[i], buf3, 100, 0);
				}
			}
		}
		
	} while (ret != SOCKET_ERROR && ret != 0);

	char buf2[100] = { 0 };
	send(c, buf2, 100, 0);
	string s = to_string(c);
	char* temp = const_cast<char*>(s.c_str());
	char temp2[] = "�뿪�������ң�";
	char buf3[100] = "";
	strcpy(buf3, temp);
	strcat(buf3, temp2);
	cout << buf3 << endl;
	for (int i = 0;i < MAXNUM;i++)
	{
		if (client_fds[i] != 0 && client_fds[i] != c)
		{
			send(client_fds[i], buf3, 100, 0);
		}
		if (client_fds[i] != 0 && client_fds[i] == c)
		{
			client_fds[i] = 0;
		}
	}

	return 0;
}