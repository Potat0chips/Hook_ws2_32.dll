#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <stdio.h>

#define SIZE 6
BYTE oldBytes[SIZE] = { 0 };
BYTE JMP[SIZE] = { 0 };
DWORD oldProtect, myProtect = PAGE_EXECUTE_READWRITE;
typedef int (WINAPI *pRecv)(UINT, PSTR, int, int);
int WINAPI MyRecv(UINT s, PSTR buf, int len, int flags);
pRecv pOrigAddress = NULL;
void BeginRedirect(LPVOID);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		pOrigAddress = (pRecv)GetProcAddress(GetModuleHandle(L"WS2_32.dll"), "recv");
		if (pOrigAddress != NULL)
			BeginRedirect(MyRecv);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


int WINAPI MyRecv(UINT s, PSTR buf, int len, int flags)
{
	VirtualProtect((LPVOID)pOrigAddress, SIZE, myProtect, &oldProtect);
	memcpy(pOrigAddress, oldBytes, SIZE);
	int retValue = recv(s, buf, len, flags);
	memcpy(pOrigAddress, JMP, SIZE);
	VirtualProtect((LPVOID)pOrigAddress, SIZE, oldProtect, &myProtect);
	OutputDebugStringA("do Something!!");
	printf("MyRecv>>%s\n",buf);
	return retValue;
}

void BeginRedirect(LPVOID newFunction)
{
	BYTE tempJMP[SIZE] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 };
	memcpy(JMP, tempJMP, SIZE);
	DWORD JMPSize = ((DWORD)newFunction - (DWORD)pOrigAddress - 5);
	VirtualProtect((LPVOID)pOrigAddress, SIZE, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(oldBytes, pOrigAddress, SIZE);
	memcpy(&JMP[1], &JMPSize, 4);
	memcpy(pOrigAddress, JMP, SIZE);
	VirtualProtect((LPVOID)pOrigAddress, SIZE, oldProtect, &myProtect);
}
