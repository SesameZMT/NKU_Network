#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

using  namespace std;

// stdcall的线程处理函数
DWORD WINAPI ThreadGET(LPVOID lpThreadParameter);

int  main()
{
	//加载winsock2的环境
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		cout << "WSAStartup  error：" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "WSA启动成功！" << endl;
	}

	// 创建流式套接字
	SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "socket  error：" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "流式套接字创建成功！" << endl;
	}
	cout << "请输入房间IP地址：";
	char addr_ip[50];
	cin >> addr_ip;
	// 链接服务器
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr(addr_ip);

	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "connect  error：" << GetLastError() << endl;
		return 0;
	}
	else
	{
		cout << "服务器连接成功！" << endl;
	}

	bool flag = true;

	while (flag)
	{
		// 接收服务端的消息
		HANDLE hThreadGET = CreateThread(NULL, 0, ThreadGET, (LPVOID)s, 0, NULL);
		// 随时给服务端发消息
		int  ret = 0;
		do
		{
			char buf[100] = { 0 };
			scanf("%[^\n]", &buf);
			getchar();
			// 发现输入为“quit”时退出
			if (buf[0] == 'q' && buf[1] == 'u' && buf[2] == 'i' && buf[3] == 't' && strlen(buf) == 4)
			{
				cout << "退出成功！";
				flag = false;
				CloseHandle(hThreadGET);
				break;
			}
			ret = send(s, buf, 100, 0);
		} while (ret != SOCKET_ERROR && ret != 0);
	}
	// 关闭监听套接字
	closesocket(s);

	// 清理winsock2的环境
	WSACleanup();


	return 0;
}

// 数据接收线程
DWORD WINAPI ThreadGET(LPVOID lpThreadParameter)
{
	// 与服务器端通讯，接受数据
	SOCKET c = (SOCKET)lpThreadParameter;

	// 循环接收服务器端数据
	int ret = 0;
	do
	{
		char buf[100] = { 0 };
		ret = recv(c, buf, 100, 0);
		cout << buf << endl;

	} while (ret != SOCKET_ERROR && ret != 0);

	return 0;
}