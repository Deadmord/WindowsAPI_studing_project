#include <assert.h>
#include "Game_service.h"
#include "Othello_const.h"
#include "Initialize.h"

void InitWindowGame(HWND hWnd)                            //-------!!!!-------
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    if (grid.rowMenu >= UD_MIN_POS && grid.colMenu >= UD_MIN_POS && grid.rowMenu <= UD_MAX_POS && grid.colMenu <= UD_MAX_POS)
    {
        grid.rowNumbr = grid.rowMenu;
        grid.colNumbr = grid.colMenu; //preset size of board when game loaded.
    }
    else
    {
        grid.rowNumbr = grid.colNumbr = 8;
    }
    GetWindowRect(hWnd, &rect);
    LONG width = rect.right - rect.left;
    LONG height = rect.bottom - rect.top - 60;

    resizeGridPosition(width, height, BOARD_POSITION_X, BOARD_POSITION_Y);
    initStart(&board, grid.rowNumbr, grid.colNumbr, &mainMoveCounter, &player, &computer, &curentMove, order);

    movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
    InvalidateRect(hWnd, NULL, TRUE);

    gameStatus = started;
    SetTimer(hWnd, ID_TIMER_PCMOVE, 1000, NULL);
}

/* Check whether the position is over an Othello cell. */
bool gridHitCheck(int x, int y, int* row, int* col)
{
    *row = (y - grid.y) / (grid.cellSize + CELL_GAP);
    *col = (x - grid.x) / (grid.cellSize + CELL_GAP);

    if (*row >= 0 && *row < grid.rowNumbr && *col >= 0 && *col < grid.colNumbr) {
        return true;
    }

    return false;
}

void selectCell(HWND hWnd, int row, int col)
{
    int old_row = grid.rowSel;
    int old_col = grid.colSel;

    if (row == old_row && col == old_col) {
        /* This cell is already selected. */
        return;
    }

    grid.rowSel = row;
    grid.colSel = col;

    if (old_row >= 0) {
        /* Re-draw the previously selected cell. */
        invalidateCell(hWnd, old_row, old_col);
    }

    if (row >= 0) {
        /* Need to draw the newly selected cell. */
        invalidateCell(hWnd, row, col);
    }
}

/* Invalidate an Othello cell, causing it to be repainted. */
void invalidateCell(HWND hWnd, int row, int col)
{
    RECT rect;

    rect.left = grid.x + col * (grid.cellSize + CELL_GAP);
    rect.top = grid.y + row * (grid.cellSize + CELL_GAP);
    rect.right = rect.left + grid.cellSize;
    rect.bottom = rect.top + grid.cellSize;

    InvalidateRect(hWnd, &rect, false);
    //InvalidateRect(hWnd, NULL, false);
}

void onKeyPress(HWND hWnd, WPARAM wParam)
{
    int row, col;

    row = grid.rowSel;
    col = grid.colSel;

    switch (wParam) {
    default:
        return;
    case VK_SPACE:
    case VK_RETURN:
        onMouseClick(hWnd);
        return;

    case VK_ESCAPE:
    {
        int ret = MessageBoxW(hWnd, L"Are you sure to quit?",
            L"Message", MB_OKCANCEL);

        if (ret == IDOK) {

            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }
    return;

    case VK_RIGHT: col++; break;
    case VK_LEFT:  col--; break;
    case VK_DOWN:  row++; break;
    case VK_UP:    row--; break;
    }

    selectCell(hWnd, max(0, min(row, grid.rowNumbr)), max(0, min(col, grid.colNumbr)));
}

void onMouseClick(HWND hWnd)
{
    if (curentMove == gameOver) {
        InitWindowGame(hWnd);
        //new_game();                                                //------------!!!!------------
        InvalidateRect(hWnd, NULL, TRUE);
        return;
    }

    if (gameStatus == started && curentMove == player && grid.rowSel >= 0)
    {
        makeMove(hWnd, grid.rowSel, grid.colSel);
    }
}

/* Make a move for the current player and transition the game state. */
void makeMove(HWND hWnd, int rowX, int colY)
{
    int numberMovePossible;
    assert(curentMove == player || curentMove == computer);

    movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);

    if (curentMove == player) {
        if (!(board[rowX][colY].possible == player)) {
            /* Illegal move; ignored. */
            return;
        }

        writeCell(board, grid.rowNumbr, grid.colNumbr, rowX, colY, curentMove);
        mainMoveCounter++;
        curentMove = computer;
        SetTimer(hWnd, ID_TIMER_PCMOVE, 1000, NULL);
    }
    else if (curentMove == computer) {
        writeCell(board, grid.rowNumbr, grid.colNumbr, rowX, colY, curentMove);
        mainMoveCounter++;
        curentMove = player;
    }

    crossCount = 0;
    numberMovePossible = movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);

    if (curentMove == computer && !numberMovePossible)
    {
        crossCount++;
        curentMove = player;
        numberMovePossible = movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
    }
    if (curentMove == player && !numberMovePossible)
    {
        crossCount++;
        curentMove = computer;
        numberMovePossible = movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
        SetTimer(hWnd, ID_TIMER_PCMOVE, 1000, NULL);
    }
    if (crossCount >= 2) {
        curentMove = gameOver;
    }
    mainScore(score, board, grid.rowNumbr, grid.colNumbr, black, white);
    if (mainMoveCounter > 0 && (!score[0] || !score[1])) curentMove = gameOver;
    if (mainMoveCounter >= (grid.rowNumbr * grid.colNumbr)) curentMove = gameOver;

    InvalidateRect(hWnd, NULL, TRUE);
}