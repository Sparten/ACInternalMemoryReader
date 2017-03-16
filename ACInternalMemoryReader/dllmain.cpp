#include <Windows.h>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "ACStruct.h"
using namespace std;

HANDLE ThreadHandle;
DWORD threadId;
HANDLE_PTR g_hModule = NULL;
PythonInterface *pyi = NULL;
BOOL WINAPI DllMain(HANDLE_PTR  hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
DllExport float WINAPI getSuspensionDamage(int32_t carId, int32_t index);
void __cdecl add_log(const char *fmt, ...);


void __cdecl add_log(const char *fmt, ...)
{

	if (!fmt || !strlen(fmt)) { return; }
	static char dir[512] = "";
	va_list va_alist;
	char logbuf[20000] = { 0 };
	va_start(va_alist, fmt);
	_vsnprintf_s(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), _TRUNCATE, fmt, va_alist);
	va_end(va_alist);
	std::ofstream ofile;
	strcpy_s(dir, _TRUNCATE, "C:\\projects\\ACInternalMemoryReader\\Release\\");
	strcat_s(dir, "log.txt");
	ofile.open(dir, std::ios::app);
	if (ofile)
	{
		ofile << logbuf << std::endl;
		ofile.close();
	}
	return;
}

DllExport float WINAPI getSuspensionDamage(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physics->suspensions[index]->getDamage();
	//add_log("getDamage %.04f", damage);
	return damage;
}

string ws2s(const std::wstring& wstr)
{
	return string(wstr.begin(), wstr.end());
}
//			swprintf(charBuf, L"ptr = %016llX  pyi->pSim = %016llX sizeof(SuspensionAvatar) = %016llX"
//, *(intptr_t*)ptr, pyi->pSim, sizeof(SuspensionAvatar));
DWORD_PTR  Thread(LPVOID LParam)
{
	intptr_t ptr = (intptr_t)GetModuleHandle(NULL) + 0x0000000001515E70;
	pyi = (PythonInterface*)*(intptr_t*)ptr;
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			add_log("%s -> %016llX", ws2s(pyi->name).c_str(), pyi);
			add_log("%s -> %016llX", ws2s(pyi->pSim->name).c_str(), pyi->pSim);
			add_log("sizeof(CarAvatar) -> %016llX", sizeof(CarAvatar));
			for each (CarAvatar* var in pyi->pSim->cars)
			{
				add_log("%.04f %.04f %.04f %.04f", var->physics->suspensions[0]->getDamage(), var->physics->suspensions[1]->getDamage(), var->physics->suspensions[2]->getDamage(), var->physics->suspensions[3]->getDamage());
			}
		}
		Sleep(200);
	}
	return 0;
}


BOOL WINAPI DllMain(HANDLE_PTR  hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hModule = hModule;
		ThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, lpReserved, NULL, (LPDWORD)&threadId);
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		break;
	}
	}
	return TRUE;
}