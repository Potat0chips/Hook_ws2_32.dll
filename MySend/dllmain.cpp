#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <stdio.h>


#define SIZE 6
BYTE oldBytes[SIZE] = { 0 };
BYTE JMP[SIZE] = { 0 };
DWORD oldProtect, myProtect = PAGE_EXECUTE_READWRITE;
typedef int (WINAPI *pSend)(UINT, PSTR, int, int);
int WINAPI MySend(UINT s, PSTR buf, int len, int flags);
pSend pOrigAddress = NULL;
void BeginRedirect(LPVOID);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		pOrigAddress = (pSend)GetProcAddress(GetModuleHandle(L"WS2_32.dll"), "send");
		if (pOrigAddress != NULL)
			BeginRedirect(MySend);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
int WINAPI MySend(UINT s, PSTR buf, int len, int flags)
{
	VirtualProtect((LPVOID)pOrigAddress, SIZE, myProtect, &oldProtect);
	memcpy(pOrigAddress, oldBytes, SIZE);
	printf("MySend::%s\n", buf);
	int retValue = send(s, buf, len, flags);
	memcpy(pOrigAddress, JMP, SIZE);
	VirtualProtect((LPVOID)pOrigAddress, SIZE, oldProtect, &myProtect);
	OutputDebugStringA("do Something!!");
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
