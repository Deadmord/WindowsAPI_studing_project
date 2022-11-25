#pragma once
#include "framework.h"
#include "Othello_const.h"
#include "Othello_Console.h"            //Function from previos verson Othello game

// ----------------Global Variables--------------------
HINSTANCE hInst;                        // current instance
WCHAR szTitle[MAX_LOADSTRING];          // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];    // the main window class name

HWND hUpDownRow, hEditRow;              //UpDownControl
HWND hUpDownCol, hEditCol;              //UpDownControl


struct state_t** board;                 //declare of board (as pointer to pointer to struct).
struct {
    int rowNumbr, colNumbr;             //Number of row and colomn
    int rowMenu, colMenu;               //Number of row and colomn
    int rowSel, colSel;                 // Currently selected row and colomn, or -1 if none.
    int x, y;                           // Coordinates of the grid relative to window origin.
    int widthSize, heigthSize;          // Size of the grid.
    int cellSize;                       // Size of a grid cell, not including its border.
} grid;
status_t gameStatus;
color_t player;
color_t computer;
color_t curentMove;
int order;
int mainMoveCounter;
int crossCount;
int score[2];

HBRUSH   background_brush;              //Global brush defenition;
HBRUSH   board_brush;
HBRUSH   highlight_brush;
HBRUSH   white_brush;
HBRUSH   black_brush;
HPEN     white_pen;
HPEN     black_pen;

void InitializeGlobalVar();
void InitializePaintTools();
void DeactivatePaintTools();
void InitEmptyBoard();

