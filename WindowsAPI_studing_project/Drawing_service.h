#pragma once
#include "framework.h"

void resizeGridPosition(int win_width, int win_height, int x, int y);
void drawGrid(HDC hdc);
void drawString(HDC dc, const char* s, int len, int x, int y);
void drawCell(HDC hdc, int rowX, int colY);
void drawSelectedColor(HDC hdc, int x, int y);
void drawCounter(HDC hdc, int x, int y);
void drawScore(HDC hdc, int x, int y);
void DrawPixels(HWND hWnd, HDC hdc);
