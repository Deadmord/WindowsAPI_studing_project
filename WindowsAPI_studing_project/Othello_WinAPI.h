#pragma once
#include "framework.h"

//#include <wchar.h>
//#include <strsafe.h>
//#include <string.h>
//#include <process.h>
//#include <winuser.h>
//#include <stdbool.h> //bool defenition


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
static void         AppendMenus(HWND);                        //----Up Menu---
static void         windowMenus(HWND hWnd, int x, int y);     //----Window menu(buttons)
static void         CenterWindow(HWND);
