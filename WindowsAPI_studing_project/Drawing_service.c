#include <stdio.h>
#include <wchar.h>
#include "Drawing_service.h"
#include "Initialize.h"


/* Compute the grid's size and position in the window. */
void resizeGridPosition(int win_width, int win_height, int x, int y)
{
    int widthCell, heigthCell;

    win_width -= x + 50; //Spase for menu
    win_height -= y + 100;

    int spaseForLableX = 50;
    int spaseForLableY = 50;

    widthCell = (win_width - (grid.colNumbr - 1) * CELL_GAP) / grid.colNumbr;
    heigthCell = (win_height - (grid.rowNumbr - 1) * CELL_GAP) / grid.rowNumbr;
    grid.cellSize = min(widthCell, heigthCell);

    grid.widthSize = grid.cellSize * grid.colNumbr + (grid.colNumbr - 1) * CELL_GAP;
    grid.heigthSize = grid.cellSize * grid.rowNumbr + (grid.rowNumbr - 1) * CELL_GAP;

    //grid.x = win_width / 2 - grid.widthSize / 2 + grid.cellSize;
    //grid.y = win_height / 2 - grid.heigthSize / 2 + grid.cellSize;
    //grid.x = x + grid.cellSize;
    //grid.y = y + grid.cellSize / 2;
    grid.x = x + spaseForLableX;
    grid.y = y + spaseForLableY;
}

/* Draw the grid and its contents. */
void drawGrid(HDC hdc)
{
    int rowX, colY, x, y;
    int blackScore, whiteScore;
    char status[128];
    char label[3];

    /* Draw a background square around the 8x8 centre cells. */
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, black_brush);
    Rectangle(hdc,
        grid.x,
        grid.y,
        grid.x + grid.widthSize,
        grid.y + grid.heigthSize);

    /* Draw labels. */
    for (rowX = 0; rowX < grid.rowNumbr; rowX++) {
        x = grid.x - 25;
        y = grid.y + rowX * (grid.cellSize + CELL_GAP) + grid.cellSize / 2;
        sprintf_s(label, 3, "%d", (rowX + 1));
        drawString(hdc, &label[0], 2, x, y);
    }
    for (colY = 0; colY < grid.colNumbr; colY++) {
        x = grid.x + colY * (grid.cellSize + CELL_GAP) + grid.cellSize / 2;
        y = grid.y - 25;
        label[0] = (colY + 'A');
        drawString(hdc, &label[0], 1, x, y);
    }

    /* Draw status text. */
    switch (curentMove) {
    case black:
        sprintf_s(status, 16, "Blacks's turn.");
        break;
    case white:
        sprintf_s(status, 16, "White's turn.");
        break;
    case gameOver:
        blackScore = score[0];
        whiteScore = score[1];
        if (blackScore > whiteScore) {
            sprintf_s(status, sizeof(status), "Black wins %d-%d!", blackScore, whiteScore);
        }
        else if (whiteScore > blackScore) {
            sprintf_s(status, sizeof(status), "White wins %d-%d!", whiteScore, blackScore);
        }
        else {
            sprintf_s(status, 16, "Draw!");
        }
        break;
    default:
        sprintf_s(status, sizeof(status), "Choose a dimension, color and press Start");
        break;
    }

    drawString(hdc, status, (int)strlen(status), grid.x + grid.widthSize / 2,
        grid.y + grid.heigthSize + 25);

    /* Draw cells. */
    for (rowX = 0; rowX < grid.rowNumbr; rowX++) {
        for (colY = 0; colY < grid.colNumbr; colY++) {
            drawCell(hdc, rowX, colY);
        }
    }
}

/* Draw string s of length len centered at (x,y). */
void drawString(HDC hdc, const char* s, int len, int x, int y)
{
    SIZE size;
    int a[1024];

    int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, (LPWSTR)a, 1024);

    GetTextExtentPoint32A(hdc, s, len, &size);
    TextOut(hdc, x - size.cx / 2, y - size.cy / 2, (LPWSTR)a, len);
}

void drawCell(HDC hdc, int rowX, int colY)
{
    int x, y;
    bool highlight;
    color_t diskStatus;
    possible_t possible;
    diskStatus = board[rowX][colY].color;
    possible = board[rowX][colY].possible;

    x = grid.x + colY * (grid.cellSize + CELL_GAP);
    y = grid.y + rowX * (grid.cellSize + CELL_GAP);

    highlight = (rowX == grid.rowSel && colY == grid.colSel &&
        curentMove == player);

    /* Draw the cell background. */
    SelectObject(hdc, GetStockObject(NULL_PEN));

    SelectObject(hdc, highlight ? highlight_brush : board_brush);
    Rectangle(hdc, x, y, x + grid.cellSize, y + grid.cellSize);

    if (diskStatus != empty) {
        /* Draw the disk. */
        SelectObject(hdc, diskStatus == black ? black_brush : white_brush);     //select a brash
        SelectObject(hdc, GetStockObject(DC_PEN));                              //1st approach: get dc_pen
        //SetDCPenColor(hdc, diskStatus == black ? SOFT_WHITE : SOFT_BLACK);    //1st approach: change collor of dc_pen
        SelectObject(hdc, diskStatus == black ? white_pen : black_pen);         //2nd approach: select a pen

        //SelectObject(hdc, GetStockObject(DC_BRUSH));                          //benchmark
        //SetDCBrushColor(hdc, RGB(64, 128, 248));
        //SelectObject(hdc, GetStockObject(DC_PEN));
        //SetDCPenColor(hdc, RGB(255, 128, 032));

        Ellipse(hdc, (x + 2), (y + 2), (x + grid.cellSize - 2), (y + grid.cellSize - 2));
    }
    if (possible != empty) {
        /* Draw the disk. */
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        SelectObject(hdc, GetStockObject(DC_PEN));                               //1st approach: get dc_pen
        //SetDCPenColor(hdc, possible == for_black ? SOFT_BLACK : SOFT_WHITE);   //1st approach: change collor of dc_pen
        SelectObject(hdc, possible == for_black ? black_pen : white_pen);        //2nd approach: select a pen

        Ellipse(hdc, (x + 2), (y + 2), (x + grid.cellSize - 2), (y + grid.cellSize - 2));
    }
}

void drawSelectedColor(HDC hdc, int x, int y)
{
    const int      shiftX = 120;
    const int      shiftY = 130;
    const int      cellSize = 50;
    char           status[128];

    if (player == black) {
        sprintf_s(status, sizeof(status), "You chose black");
    }
    else if (player == white) {
        sprintf_s(status, sizeof(status), "You chose white");
    }
    else {
        sprintf_s(status, 16, "Not chosen");
    }

    drawString(hdc, status, (int)strlen(status), x + 60,
        y + shiftY + 25);


    /* Draw the cell background. */
    SelectObject(hdc, GetStockObject(NULL_PEN));

    SelectObject(hdc, board_brush);
    Rectangle(hdc, x + shiftX, y + shiftY, x + shiftX + cellSize, y + shiftY + cellSize);

    if (player != empty) {
        /* Draw the disk. */
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, player == black ? black_brush : white_brush);
        SelectObject(hdc, player == black ? white_pen : black_pen);

        Ellipse(hdc, (x + shiftX + 2), (y + shiftY + 2), (x + shiftX + cellSize - 2), (y + shiftY + cellSize - 2));
    }
}

void drawCounter(HDC hdc, int x, int y)
{
    const int      shiftX = 90;
    const int      shiftY = 225;
    char           status[128];

    sprintf_s(status, sizeof(status), "Count of moves: %d", mainMoveCounter);
    drawString(hdc, status, (int)strlen(status), x + shiftX, y + shiftY);
}

void drawScore(HDC hdc, int x, int y)
{
    const int      shiftX = 90;
    const int      shiftY = 250;
    int blackScore, whiteScore;
    char           status[128];

    mainScore(score, board, grid.rowNumbr, grid.colNumbr, black, white);
    blackScore = score[0];
    whiteScore = score[1];

    sprintf_s(status, sizeof(status), "Black - %d  |  %d - White", blackScore, whiteScore);
    drawString(hdc, status, (int)strlen(status), x + shiftX, y + shiftY);
}

void DrawPixels(HWND hWnd, HDC hdc) {

    //PAINTSTRUCT ps;
    RECT r;
    GetClientRect(hWnd, &r);

    if (r.bottom == 0) {
        return;
    }

    //HDC hdc = BeginPaint(hWnd, &ps);
    Sleep(3000);
    for (int i = 0; i < 250; i++) {
        Sleep(5);
        int x = rand() % r.right;
        int y = rand() % r.bottom;
        //SetPixel(hdc, x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        Ellipse(hdc, x, y, x + i, y + i);
    }

    InitEmptyBoard(hWnd);
    InvalidateRect(hWnd, NULL, TRUE);
    //EndPaint(hWnd, &ps);
}