#pragma once
#include "framework.h"
#include <wchar.h>
#include <strsafe.h>
#include <commctrl.h>
#include "Othello_res.h"
#include "Othello_const.h"
#include "Resource.h"

#include <time.h>                   ////use in random and dalay

//-------*********------------
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#include <winuser.h>
#include <stdbool.h> //bool defenition


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
static void         appendMenus(HWND);                        //----Up Menu---
static void         windowMenus(HWND hWnd, int x, int y);     //----Window menu(buttons)
static void         resizeGridPosition(int win_width, int win_height, int x, int y);
static void         drawGrid(HDC hdc);
static void         drawString(HDC dc, const char* s, int len, int x, int y);
static void         drawCell(HDC hdc, int rowX, int colY);
static void         drawSelectedColor(HDC hdc, int x, int y);
static void         drawCounter(HDC hdc, int x, int y);
static void         drawScore(HDC hdc, int x, int y);

static void         InitEmptyBoard();
static void         InitWindowGame(HWND hWnd);
static bool         gridHitCheck(int x, int y, int* row, int* col);
static void         selectCell(HWND window, int row, int col);
static void         invalidateCell(HWND window, int row, int col);
static void         onKeyPress(HWND hWnd, WPARAM wParam);
static void         onMouseClick(HWND hWnd);
static void         makeMove(HWND hWnd, int rowX, int colY);
static void         CenterWindow(HWND);
static void         DrawPixels(HWND hWnd, HDC hdc);
