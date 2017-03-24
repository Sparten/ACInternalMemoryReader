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
int32_t isInitialized = 0;

string ws2s(const std::wstring& wstr)
{
	return string(wstr.begin(), wstr.end());
}

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

DllExport int32_t WINAPI Init()
{
	if (isInitialized == 0)
	{
		intptr_t patternAddress = (intptr_t)FindPattern((intptr_t)GetModuleHandle(NULL), 0x0000000002000000,
			(CHAR *)"\x48\x89\x1D\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x4C\x89\x73\x58",
			(CHAR *)"xxx????xxx????xx????xxxx") + 3;
	
		if (patternAddress == NULL)
		{
			return isInitialized;
		}
		try
		{
			pyi = (PythonInterface*)*(intptr_t*)(patternAddress + LODWORD(*(intptr_t*)patternAddress + 4));
			if (pyi != nullptr)
			{
				if (pyi->name.compare(L"PYTHON_INTERFACE") == 0)
				{
					if (pyi->pSim->name.compare(L"ACS_SIM") == 0)
					{
						if (pyi->pSim->cars[0]->name.compare(L"CAR_AVATAR") == 0)
						{
							isInitialized = 1;
							return isInitialized;
						}
						
					}
				}
			}
		}
		catch (exception e)
		{
			isInitialized = 0;
			return isInitialized;
		}
	}
	return isInitialized;
}

DllExport float WINAPI GetSuspensionDamage(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physics->suspensions[index]->getDamage();
	return damage;
}

DllExport float WINAPI GetEngineLifeLeft(int32_t carId)
{
	float damage = pyi->pSim->cars[carId]->physicsState.engineLifeLeft;
	return damage;
}

DllExport float WINAPI GetTyreInflation(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physicsState.tyreInflation[index];
	return damage;
}

DllExport float WINAPI GetTyreVirtualKM(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physicsState.tyreVirtualKM[index];
	return damage;
}

DllExport float WINAPI GetTyreFlatSpot(int32_t carId, int32_t index)
{
	float damage = pyi->pSim->cars[carId]->physicsState.tyreFlatSpot[index];
	return damage;
}

BOOL WINAPI DllMain(HANDLE_PTR  hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
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