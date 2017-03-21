#include <Windows.h>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "ACStruct.h"

using namespace std;

HANDLE ThreadHandle;
DWORD threadId;
PythonInterface *pyi = NULL;

BOOL WINAPI DllMain(HANDLE_PTR  hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
DllExport float WINAPI getSuspensionDamage(int32_t carId, int32_t index);
DllExport float WINAPI getEngineLifeLeft(int32_t carId);
DllExport float WINAPI getTyreInflation(int32_t carId, int32_t index);
DllExport float WINAPI getAeroDamage(int32_t carId);

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
	strcpy_s(dir, _TRUNCATE, "C:\\");
	strcat_s(dir, "ACI_log.txt");
	ofile.open(dir, std::ios::app);
	if (ofile)
	{
		ofile << logbuf << std::endl;
		ofile.close();
	}
	return;
}

intptr_t FindPattern(intptr_t start_offset, intptr_t size, char *pattern, char *mask)
{
	DWORD byteMatchedIndex = 0;
	int searchLen = strlen(mask) - 1;

	for (intptr_t scanAddress = start_offset; scanAddress < start_offset + size; scanAddress++) 
	{
		//Do scan
		if (*(BYTE*)scanAddress == (BYTE)pattern[byteMatchedIndex] || mask[byteMatchedIndex] == '?')
		{
			if (mask[byteMatchedIndex + 1] == '\0') 
			{
				return (scanAddress - searchLen);
			}
			byteMatchedIndex++;
		}
		else 
		{
			byteMatchedIndex = 0;
		}
	}
	return NULL;
}

DllExport float WINAPI getSuspensionDamage(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physics->suspensions[index]->getDamage();
	//add_log("getDamage %.04f", damage);
	return damage;
}

//seems looks like this have some issues, i will investigate on it later.
DllExport float WINAPI getEngineLifeLeft(int32_t carId)
{
	float damage = pyi->pSim->cars[carId]->physicsState.engineLifeLeft;

	return damage;
}

DllExport float WINAPI getTyreInflation(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physicsState.tyreInflation[index];
	
	return damage;
}

//seems looks like this have some issues, i will investigate on it later.
DllExport float WINAPI getAeroDamage(int32_t carId)
{
	float wingDamage = 0.0f;
	//add_log("%i", pyi->pSim->cars[carId]->physics->aeroMap.wings.size());
	for each (Wing wing in pyi->pSim->cars[carId]->physics->aeroMap.wings)
	{
		if (wing.hasDamage)
		{
			for each (float damage in wing.damageCD)
			{
				wingDamage += damage;
			}
			for each (float damage in wing.damageCL)
			{
				wingDamage += damage;
			}
		}
	}
	return wingDamage;
}

string ws2s(const std::wstring& wstr)
{
	return string(wstr.begin(), wstr.end());
}
//swprintf(charBuf, L"ptr = %016llX  pyi->pSim = %016llX sizeof(SuspensionAvatar) = %016llX"

DWORD_PTR Thread(LPVOID LParam)
{
	//intptr_t ptr = (intptr_t)GetModuleHandle(NULL) + 0x0000000001515E70;

	intptr_t patternAddress = (intptr_t)FindPattern((intptr_t)GetModuleHandle(NULL), 0x0000000002000000,
		(CHAR *)"\x48\x89\x1D\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x4C\x89\x73\x58",
		(CHAR *)"xxx????xxx????xx????xxxx")+3;
	
	//add_log("patternAddress -> %016llX", patternAddress);
	pyi = (PythonInterface*)*(intptr_t*)(patternAddress + LODWORD(*(intptr_t*)patternAddress + 4));	
	add_log("pyi -> %016llX", pyi);

	add_log("AeroMap -> %016llX", sizeof(AeroMap));
	add_log("sizeof(Wing) -> %016llX", sizeof(Wing));
	add_log("sizeof(CarPhysicsState) -> %016llX", sizeof(CarPhysicsState));
	add_log("sizeof(std::vector<Wing >) -> %016llX", sizeof(std::vector<Wing >));

	add_log("offsetof(Car::suspensions -> %016llX", offsetof(Car, suspensions));
	add_log("offsetof(Car::aeroMap -> %016llX", offsetof(Car, aeroMap));
	add_log("offsetof(AeroMap::wings -> %016llX", offsetof(AeroMap, wings));
	add_log("offsetof(CarPhysicsState::engineLifeLeft -> %016llX", offsetof(CarPhysicsState, engineLifeLeft));
	

	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			for each (Wing wing in pyi->pSim->cars[0]->physics->aeroMap.wings)
			{
				add_log(ws2s(wing.name).c_str());
			}
			//add_log("%s -> %016llX", ws2s(pyi->name).c_str(), pyi);
			//add_log("%s -> %016llX", ws2s(pyi->pSim->name).c_str(), pyi->pSim);
			//add_log("sizeof(CarAvatar) -> %016llX", sizeof(CarAvatar));
			//for each (CarAvatar* var in pyi->pSim->cars)
			//{
			//	add_log("%.04f %.04f %.04f %.04f", var->physics->suspensions[0]->getDamage(), var->physics->suspensions[1]->getDamage(), var->physics->suspensions[2]->getDamage(), var->physics->suspensions[3]->getDamage());
			//}
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
			ThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, lpReserved, NULL, (LPDWORD)&threadId);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}