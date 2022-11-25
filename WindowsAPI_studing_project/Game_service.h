#pragma once
#include <stdbool.h> //bool defenition
#include "framework.h"

void InitWindowGame(HWND hWnd);
bool gridHitCheck(int x, int y, int* row, int* col);
void selectCell(HWND window, int row, int col);
void invalidateCell(HWND window, int row, int col);
void onKeyPress(HWND hWnd, WPARAM wParam);
void onMouseClick(HWND hWnd);
void makeMove(HWND hWnd, int rowX, int colY);