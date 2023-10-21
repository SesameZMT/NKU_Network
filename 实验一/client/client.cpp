#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

using  namespace std;

// stdcall���̴߳�����
DWORD WINAPI ThreadGET(LPVOID lpThreadParameter);

int  main()
{
	//����winsock2�Ļ���
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		cout << "WSAStartup  error��" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "WSA�����ɹ���" << endl;
	}

	// ������ʽ�׽���
	SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "socket  error��" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "��ʽ�׽��ִ����ɹ���" << endl;
	}
	cout << "�����뷿��IP��ַ��";
	char addr_ip[50];
	cin >> addr_ip;
	// ���ӷ�����
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr(addr_ip);

	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "connect  error��" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "���������ӳɹ���" << endl;
	}

	bool flag = true;

	while (flag)
	{
		// ���շ���˵���Ϣ
		HANDLE hThreadGET = CreateThread(NULL, 0, ThreadGET, (LPVOID)s, 0, NULL);
		// ��ʱ������˷���Ϣ
		int  ret = 0;
		do
		{
			char buf[100] = { 0 };
			scanf("%[^\n]", &buf);
			getchar();
			// ��������Ϊ��quit��ʱ�˳�
			if (buf[0] == 'q' && buf[1] == 'u' && buf[2] == 'i' && buf[3] == 't' && strlen(buf) == 4)
			{
				cout << "�˳��ɹ���";
				flag = false;
				CloseHandle(hThreadGET);
				break;
			}
			ret = send(s, buf, 100, 0);
		} while (ret != SOCKET_ERROR && ret != 0);
	}
	// �رռ����׽���
	closesocket(s);

	// ����winsock2�Ļ���
	WSACleanup();


	return 0;
}

// ���ݽ����߳�
DWORD WINAPI ThreadGET(LPVOID lpThreadParameter)
{
	// ���������ͨѶ����������
	SOCKET c = (SOCKET)lpThreadParameter;

	// ѭ�����շ�����������
	int ret = 0;
	do
	{
		char buf[100] = { 0 };
		ret = recv(c, buf, 100, 0);
		cout << buf << endl;

	} while (ret != SOCKET_ERROR && ret != 0);

	return 0;
}