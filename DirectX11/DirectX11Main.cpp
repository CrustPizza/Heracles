/********************************
*								*
*	DirectX11 Main.cpp			*
*								*
*	Created : 2022/06/14		*
*	Updated : 2022/06/14		*
*********************************/

/* Framework */
#include "DirectX11Framework.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(_T("DirectX11\t\tDLL_PROCESS_ATTACH\n"));
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(_T("DirectX11\t\tDLL_THREAD_ATTACH\n"));
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(_T("DirectX11\t\tDLL_PROCESS_DETACH\n"));
		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(_T("DirectX11\t\tDLL_THREAD_DETACH\n"));
		break;
	}

	return TRUE;
}