// Socket_Recv_Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
VOID eMsg(char* msg);
int _tmain(int argc, _TCHAR* argv[])
{
	//HINSTANCE hlib = LoadLibrary(L"afflux.dll");
	//if (!hlib)
	//	error_msg("error:000:loadlibrary_error\n");
	//init wsadata
	WSADATA wsd;
	int code;
	code = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (code == SOCKET_ERROR)
		eMsg("error:001:init_error\n");
	if (HIBYTE(wsd.wVersion) != 2 || LOBYTE(wsd.wVersion) != 2)
		eMsg("error:002:version_error\n");

	//create socket
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sSocket == SOCKET_ERROR)
		eMsg("error:003:create_socket_error\n");
	//bind
	sockaddr_in Saddr;
	Saddr.sin_port = htons(0x1234);
	Saddr.sin_family = AF_INET;
	Saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	code = bind(sSocket, (sockaddr*)&Saddr, sizeof(sockaddr));
	if (code == SOCKET_ERROR)
		eMsg("error:004:bind_socket_error\n");
	code = listen(sSocket, SOMAXCONN);
	if (code == SOCKET_ERROR)
		eMsg("error:005:listen_socket_error\n");
	printf("server>>\n");
	while (true)
	{
		SOCKET Csocket;
		sockaddr_in Caddr;
		int CaddrSize = sizeof(Caddr);
		Csocket = accept(sSocket, (sockaddr*)&Caddr, &CaddrSize);
		if (Csocket == INVALID_SOCKET)
			eMsg("error:006:accept_client_error\n");
		char buff[100] = "";
		code = recv(Csocket, buff, 100, 0);
		if (code == SOCKET_ERROR)
			eMsg("error:005:recv_server_error\n");
		printf("recv>>%s\n", buff);
	}
	return 0;
}

VOID eMsg(char* msg)
{
	printf(msg);
	system("pause");
	exit(0);
}

