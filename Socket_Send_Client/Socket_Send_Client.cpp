// Socket_Send_Client.cpp : 定义控制台应用程序的入口点。
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
	printf("Client>>\n");
	while (true)
	{
		//create socket
		SOCKET Csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Csocket == INVALID_SOCKET)
			eMsg("error:003:create_socket_error\n");
		//link
		sockaddr_in Saddr;
		Saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		Saddr.sin_family = AF_INET;
		Saddr.sin_port = htons(0x1234);
		int SaddrSize = sizeof(Saddr);
		code = connect(Csocket, (sockaddr*)&Saddr, SaddrSize);
		if (Csocket == INVALID_SOCKET)
			eMsg("error:004:link_server_error\n");
		char buff[100] = "";
		printf("input>>");
		gets_s(buff, 50);
		code = send(Csocket, buff, 50, 0);
		if (code == SOCKET_ERROR)
			eMsg("error:006:send_data_error\n");
		closesocket(Csocket);
	}
	return 0;
}
VOID eMsg(char* msg)
{
	printf(msg);
	system("pause");
	exit(0);
}