#include <string.h>

#include "Initialize.h"
#include "Othello_const.h"


void InitializeGlobalVar()
{
    wcscpy_s(szTitle, MAX_LOADSTRING, L"Othelo game - masa project");
    wcscpy_s(szWindowClass, MAX_LOADSTRING, L"MainWindow");
    gameStatus = stoped;
    player = 0;
    computer = 0;
    curentMove = 0;
    order = 0;
    mainMoveCounter = 0;
    crossCount = 0;
    score[0] = score[1] = 0;
    grid.rowNumbr = grid.colNumbr = UD_SET_POS; //preset size of board when game loaded.
    srand((int)time(NULL));          //Random preset
}

void DeactivatePaintTools()
{
    DeleteObject(background_brush);
    DeleteObject(board_brush);
    DeleteObject(highlight_brush);
    DeleteObject(white_brush);
    DeleteObject(black_brush);
    DeleteObject(white_pen);
    DeleteObject(black_pen);
}

void InitializePaintTools()
{
    background_brush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); //Global brush initialisation
    board_brush = CreateSolidBrush(BOARD_COLOR);
    highlight_brush = CreateSolidBrush(HIGHLIGHT_COLOR);

    white_brush = CreateSolidBrush(WHITE);
    black_brush = CreateSolidBrush(BLACK);
    white_pen = CreatePen(PS_SOLID, 1, SOFT_WHITE);
    black_pen = CreatePen(PS_SOLID, 1, SOFT_BLACK);
}
