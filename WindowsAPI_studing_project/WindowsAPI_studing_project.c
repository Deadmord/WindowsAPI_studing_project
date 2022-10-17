#include "framework.h"
#include <wchar.h>
#include <strsafe.h>
#include <commctrl.h>
#include <time.h>
#include <winuser.h>
#include "WindowsAPI_studing_project.h"
#include "Othello_res.h"
#include "Othello_Console.h"        //Function from previos verson Othello game

//-------*********------------
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

//----------Windows sizes & initial position---------
#define MAX_LOADSTRING      100
#define INIT_WIDTH_SIZE     1024     // Initial window size
#define INIT_HEIGTH_SIZE    768 
#define MIN_WIDTH_SIZE      800     // MIN window size
#define MIN_HEIGTH_SIZE     600
#define CELL_GAP            1       // Cell gap in pixels
#define WM_PC_MOVE (WM_USER + 0)    // Messege to start handle computer move
#define ID_TIMER_PCMOVE     555     // Timer for dalay of computer move
#define ID_HOTKEY           1       //Hotkey Ctrl+C

#define MENU_POSITION_X     10      //Positions of menu
#define MENU_POSITION_Y     20
#define BOARD_POSITION_X    180
#define BOARD_POSITION_Y    20



// ----------------Global Variables--------------------
HINSTANCE hInst;                                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"Othelo game - masa project";  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = L"MainWindow";      // the main window class name

HWND hUpDownRow, hEditRow;                                    //UpDownControl
HWND hUpDownCol, hEditCol;                                    //UpDownControl


struct state_t** board = NULL;                                  //declare of board (as pointer to pointer to struct).
static struct {
    int rowNumbr, colNumbr;            //Number of row and colomn
    int rowSel, colSel;                 /* Currently selected row and colomn, or -1 if none. */
    int x, y;                           /* Coordinates of the grid relative to window origin. */
    int widthSize, heigthSize;          /* Size of the grid. */
    int cellSize;                      /* Size of a grid cell, not including its border. */
} grid;

color_t player = 0;
color_t computer = 0;
color_t curentMove = 0;
int order = 0;
int mainMoveCounter = 0;
int crossCount = 0;
int score[2] = { 0 };

static HBRUSH background_brush;                         //Global brush defenition;
static HBRUSH board_brush; 
static HBRUSH highlight_brush;
static HBRUSH white_brush;
static HBRUSH black_brush;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    PanelProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
static void         appendMenus(HWND);                        //----Up Menu---
static void         windowMenus(HWND hWnd, int x, int y);     //----Window menu(buttons)
static void         resizeGridPosition(int win_width, int win_height);
static void         drawGrid(HDC hdc);
static void         drawString(HDC dc, const char* s, int len, int x, int y);
static void         drawCell(HDC hdc, int rowX, int colY);
static void         drawSelectedColor(HDC hdc, int x, int y);
static void         drawCounter(HDC hdc, int x, int y);
static void         drawScore(HDC hdc, int x, int y);
static void         InitWindowGame(HDC hdc);            //Empty!!!!
static bool         gridHitCheck(int x, int y, int* row, int* col);
static void         selectCell(HWND window, int row, int col);
static void         invalidateCell(HWND window, int row, int col);
static void         onKeyPress(HWND hWnd, WPARAM wParam);
static void         onMouseClick(HWND hWnd);
static void         makeMove(HWND hWnd, int rowX, int colY);
static void         CenterWindow(HWND);


//************Hey, entry here!***************
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);  // Switch off warnings
    UNREFERENCED_PARAMETER(lpCmdLine);      

    // Initialize global strings
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); // Я закомментил, лучше разкоментить
    //LoadStringW(hInstance, IDC_WINDOWSAPISTUDINGPROJECT, szWindowClass, MAX_LOADSTRING);

    //Использовать инициализацию когда будет кнопка старт (если статус игры начать то вызвать инициализацию)
    grid.rowNumbr = grid.colNumbr = 12; //preset size of board when game loaded.
    resizeBoard(&board, grid.rowNumbr, grid.colNumbr); //prepare new gamebord: allocate mamory

    for (int i = 0; i < grid.rowNumbr; i++) {	//reset to NULL allocated memory
        for (int j = 0; j < grid.rowNumbr; j++) {
            board[i][j] = cellInst();
        }
    }
    grid.rowSel = -1;
    //конец временной инициализации

    background_brush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); //Global brush initialisation
    board_brush = CreateSolidBrush(RGB(0x00, 0x80, 0x00));
    highlight_brush = CreateSolidBrush(RGB(0x00, 0xAA, 0x00));
    white_brush = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
    black_brush = CreateSolidBrush(RGB(0, 0, 0));

    srand(time(NULL));          //Random preset

    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))        // Perform application initialization.
    {
        return FALSE;
    }
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPISTUDINGPROJECT));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    DeleteObject(background_brush);
    DeleteObject(board_brush);
    DeleteObject(highlight_brush);
    DeleteObject(white_brush);
    DeleteObject(black_brush);

    return (int) msg.wParam;
}




// Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW ;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPISTUDINGPROJECT)); //TODO Icon!!!
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_GRAYTEXT +1);
    wcex.hbrBackground  = background_brush;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPISTUDINGPROJECT);
    wcex.lpszClassName  = szWindowClass;
    
    return RegisterClassExW(&wcex);
}

// Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, //L"MyTitle"
      CW_USEDEFAULT, 0, INIT_WIDTH_SIZE, INIT_HEIGTH_SIZE, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//  PURPOSE: Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HMENU       hMenu;  //A popup menu 
    POINT       point;

    LPNMUPDOWN lpnmud;  //UpDownControl
    UINT code;          //UpDownControl

    switch (message)
    {
    case WM_CREATE:
        appendMenus(hWnd); //Menu
        InitCommonControls();
        windowMenus(hWnd, MENU_POSITION_X , MENU_POSITION_Y);

        //Game initialisation here !!!!!!!!!!!!!------------

        RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
        break;

    case WM_SIZE:           //If window size changed
        resizeGridPosition(LOWORD(lParam), HIWORD(lParam), BOARD_POSITION_X, BOARD_POSITION_Y);
        break;

    case WM_GETMINMAXINFO:              //Windows is asking about the size constraints
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = MIN_WIDTH_SIZE;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = MIN_HEIGTH_SIZE;
        break;

    case WM_MOUSEMOVE:
    {   /* The mouse moved. */
        int rowX, colY;
        if (gridHitCheck(LOWORD(lParam), HIWORD(lParam), &rowX, &colY)) {
            selectCell(hWnd, rowX, colY);
        }
        else {
            selectCell(hWnd, -1, -1);
        }
    }
        break;

    case WM_LBUTTONDOWN:
        SetTimer(hWnd, ID_TIMER_PCMOVE, 2000, NULL);
        break;

    case WM_LBUTTONUP:
        /* Left mouse button up. */
        onMouseClick(hWnd);
        break;

    case WM_RBUTTONUP:

        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        hMenu = CreatePopupMenu();
        ClientToScreen(hWnd, &point);

        AppendMenuW(hMenu, MF_STRING, IDM_START, L"&Start");
        AppendMenuW(hMenu, MF_STRING, IDM_NEW_GAME, L"&New game");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, MF_STRING, IDM_ABOUT, L"&About");
        AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;

    case WM_KEYDOWN:
        /* Keyboard key down. */
        onKeyPress(hWnd, wParam);
        break;

    case WM_HOTKEY:
        if ((wParam) == ID_HOTKEY)
        {
            CenterWindow(hWnd);
        }
        break;

    case WM_PAINT:          //  WM_PAINT    - Paint the main window
        /* The window needs a re-paint. */
        EnableMenuItem(GetMenu(hWnd), IDM_START,
            TRUE ? MF_ENABLED : MF_GRAYED);
        EnableMenuItem(GetMenu(hWnd), IDM_NEW_GAME,
            curentMove != computer ? MF_ENABLED : MF_GRAYED);

        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        drawSelectedColor(hdc, MENU_POSITION_X, MENU_POSITION_Y);
        drawCounter(hdc, MENU_POSITION_X, MENU_POSITION_Y);
        drawScore(hdc, MENU_POSITION_X, MENU_POSITION_Y);

        drawGrid(hdc);
        EndPaint(hWnd, &ps);

        //DrawPixels(hWnd);
        //InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_COMMAND:    //  WM_COMMAND  - process the application menu
        {
            if (HIWORD(wParam) == BN_CLICKED) {

                switch (LOWORD(wParam)) 
                {
                case ID_BLACK:
                    order = 1;
                    //player = black;
                    //computer = white;
                    initStart(&board, grid.rowNumbr, grid.colNumbr, &mainMoveCounter, &player, &computer, &curentMove, order);
                    break;
                case ID_WHITE:
                    order = 2;
                    //player = white;
                    //computer = black;
                    initStart(&board, grid.rowNumbr, grid.colNumbr, &mainMoveCounter, &player, &computer, &curentMove, order);
                    break;
                }
                movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
                InvalidateRect(hWnd, NULL, TRUE);
                SetFocus(hWnd);
            }

            //int wmId = LOWORD(wParam);      // Parse the menu selections:
            switch (LOWORD(wParam))
            {

            case IDM_START:
            case IDM_NEW_GAME:
                MessageBeep(MB_ICONINFORMATION);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_FILE_QUIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        break;
    case WM_NOTIFY:

        code = ((LPNMHDR)lParam)->code;

        if (code == UDN_DELTAPOS) {

            lpnmud = (NMUPDOWN*)lParam;

            int value = lpnmud->iPos + lpnmud->iDelta;

            if (value < UD_MIN_POS) {
                value = UD_MIN_POS;
            }

            if (value > UD_MAX_POS) {
                value = UD_MAX_POS;
            }
            
            if (lpnmud->hdr.idFrom == ID_UPDOWN_ROW)
            {
                grid.rowNumbr =  value;
            }
            else if (lpnmud->hdr.idFrom == ID_UPDOWN_COL)
            {
                grid.colNumbr = value;
            }
            



            //const int asize = 4;
            //wchar_t buf[4];
            //size_t cbDest = asize * sizeof(wchar_t);
            //StringCbPrintfW(buf, cbDest, L"%d", value);
            //SetWindowTextW(hStatic, buf);
            //InvalidateRect(hWnd, NULL, false);
        }

        break;

    case WM_TIMER:
        MessageBeep(0);
        {   KillTimer(hWnd, ID_TIMER_PCMOVE);
            int rowSel, colSel;
            if (curentMove != 0 && curentMove == computer && mainMoveCounter > 2) {
                movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
                pcMove(board, grid.rowNumbr, grid.colNumbr, computer, &rowSel, &colSel);
                SendMessage((HWND)hWnd, WM_PC_MOVE, rowSel, colSel);
            }
        }
        return 0;

    case WM_PC_MOVE:
        /* White computed a move. */
        assert(curentMove == computer);
        makeMove(hWnd, wParam, lParam);
        break;

    case WM_DESTROY:        //  WM_DESTROY  - post a quit message and return
        UnregisterHotKey(hWnd, ID_HOTKEY);
        PostQuitMessage(0);
        break;
    default:

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//LRESULT CALLBACK PanelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
//{
//    switch (msg) {
//
//    case WM_LBUTTONUP:
//
//        MessageBeep(MB_OK);
//        MessageBoxW(NULL, L"First Program", L"First", MB_OK);
//        break;
//    }
//
//    return DefWindowProcW(hWnd, msg, wParam, lParam);
//}
void appendMenus(HWND hWnd)     //Menu handler
{

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_START, L"&Start");
    AppendMenuW(hMenu, MF_STRING, IDM_NEW_GAME, L"&New game");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_ABOUT, L"&About");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Menu");
    SetMenu(hWnd, hMenubar);
}

void windowMenus(HWND hWnd, int x, int y) //Menu handler
{ 
    int buttonWidth = 75;
    int buttonHeight = 25;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_UPDOWN_CLASS;
    InitCommonControlsEx(&icex);

    //-----------Сhoose a demention------------
    CreateWindowW(L"Button", L"Size of board (Row | Col)",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        x, y, 180, 60, hWnd, (HMENU)0, hInst, NULL);

    hUpDownRow = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE
        | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
        0, 0, 0, 0, hWnd, (HMENU)ID_UPDOWN_ROW, NULL, NULL);

    hEditRow = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD
        | WS_VISIBLE | ES_RIGHT, 25, y + 25, 40, 25, hWnd,
        (HMENU)ID_EDIT_ROW, NULL, NULL);

    SendMessageW(hUpDownRow, UDM_SETBUDDY, (WPARAM)hEditRow, 0);
    SendMessageW(hUpDownRow, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
    SendMessageW(hUpDownRow, UDM_SETPOS32, 0, 0);

    hUpDownCol = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE
        | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
        0, 0, 0, 0, hWnd, (HMENU)ID_UPDOWN_COL, NULL, NULL);

    hEditCol = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD
        | WS_VISIBLE | ES_RIGHT, 115, y + 25, 40, 25, hWnd,
        (HMENU)ID_EDIT_COL, NULL, NULL);

    SendMessageW(hUpDownCol, UDM_SETBUDDY, (WPARAM)hEditCol, 0);
    SendMessageW(hUpDownCol, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
    SendMessageW(hUpDownCol, UDM_SETPOS32, 0, 0);

    //-----------Сhoose a color------------
    CreateWindowW(L"Button", L"Сhoose a color",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        x, y+80, 180, 120, hWnd, (HMENU)0, hInst, NULL);
    CreateWindowW(L"Button", L"Black",
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        x + 10, y + 100, buttonWidth, buttonHeight, hWnd, (HMENU)ID_BLACK, hInst, NULL);
    CreateWindowW(L"Button", L"White",
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        x + 20 + buttonWidth, y + 100, buttonWidth, buttonHeight, hWnd, (HMENU)ID_WHITE, hInst, NULL);
    //--------------Start/New--------------
    CreateWindowW(L"Button", L"Start",
        WS_VISIBLE | WS_CHILD,
        x, y + 285, 80, 40, hWnd, (HMENU)ID_START, NULL, NULL);

    CreateWindowW(L"Button", L"New game",
        WS_VISIBLE | WS_CHILD,
        x + 90, y + 285, 80, 40, hWnd, (HMENU)ID_NEW_GAME, NULL, NULL);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/* Compute the grid's size and position in the window. */
static void resizeGridPosition(int win_width, int win_height, int x, int y)
{
    int widthCell, heigthCell;

    win_width -= x+100; //Spase for menu
    win_height -= y+150;

    widthCell = (win_width - (grid.colNumbr - 1) * CELL_GAP) / grid.colNumbr;
    heigthCell = (win_height - (grid.rowNumbr - 1) * CELL_GAP) / grid.rowNumbr;
    grid.cellSize = min(widthCell, heigthCell);

    grid.widthSize = grid.cellSize * grid.colNumbr + (grid.colNumbr - 1) * CELL_GAP;
    grid.heigthSize = grid.cellSize * grid.rowNumbr + (grid.rowNumbr - 1) * CELL_GAP;

    //grid.x = win_width / 2 - grid.widthSize / 2 + grid.cellSize;
    //grid.y = win_height / 2 - grid.heigthSize / 2 + grid.cellSize;
    grid.x = x + grid.cellSize;
    grid.y = y + grid.cellSize / 2;

}

/* Draw the grid and its contents. */
static void drawGrid(HDC hdc)
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
        x = grid.x - grid.cellSize / 2;
        y = grid.y + rowX * (grid.cellSize + CELL_GAP) + grid.cellSize / 2;
        sprintf_s(label, 3, "%d", (rowX+1));
        drawString(hdc, &label[0], 2, x, y);
    }
    for (colY = 0; colY < grid.colNumbr; colY++) {
        x = grid.x + colY * (grid.cellSize + CELL_GAP) + grid.cellSize / 2;
        y = grid.y - grid.cellSize / 2;
        label[0] = (colY + 'A');
        drawString(hdc, &label[0], 1, x, y);
    }

    /* Draw status text. */
    switch (curentMove) {
    case black:
        sprintf_s(status, 16, "Blacks's move.");
        break;
    case white:
        sprintf_s(status, 16, "White's move.");
        break;
    case gameOver:
        blackScore = 40; // mainScore(int* score, state_tdef * *board, int row, int col, color_t player, color_t computer);
        whiteScore = 42; // mainScore(int* score, state_tdef * *board, int row, int col, color_t player, color_t computer);
        if (blackScore > whiteScore) {
            sprintf_s(status, sizeof(status), "Black wins %d-%d!", blackScore, whiteScore);
        }
        else if (whiteScore > blackScore) {
            sprintf_s(status, sizeof(status), "White wins %d-%d!", whiteScore, blackScore);
        }
        else {
            sprintf_s(status, 16, "Draw!");
        }
    default:
        sprintf_s(status, sizeof(status), "Choose a demention of the board ->");
        break;
    }

    drawString(hdc, status, strlen(status), grid.x + grid.widthSize / 2, 
        grid.y + grid.heigthSize + grid.cellSize / 2);

    /* Draw cells. */
    for (rowX = 0; rowX < grid.rowNumbr; rowX++) {
        for (colY = 0; colY < grid.colNumbr; colY++) {
            drawCell(hdc, rowX, colY);
        }
    }
}

/* Draw string s of length len centered at (x,y). */
static void drawString(HDC hdc, const char* s, int len, int x, int y)
{
    SIZE size;
    int a[1024];

    int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, a, 1024);

    GetTextExtentPoint32A(hdc, s, len, &size);
    TextOut(hdc, x - size.cx / 2, y - size.cy / 2, a, len);
}

static void drawCell(HDC hdc, int rowX, int colY)
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
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, diskStatus == black ? black_brush : white_brush);
        SelectObject(hdc, diskStatus == black ? SetDCPenColor(hdc, RGB(200, 200, 200)) : SetDCPenColor(hdc, RGB(064, 064, 064)));

        Ellipse(hdc, (x + 2), (y + 2), (x + grid.cellSize - 2), (y + grid.cellSize - 2));
    }
    if (possible != empty) {
        /* Draw the disk. */
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        SelectObject(hdc, possible == for_black ? SetDCPenColor(hdc, RGB(032, 032, 032)) : SetDCPenColor(hdc, RGB(200, 200, 200)));

        Ellipse(hdc, (x + 2), (y + 2), (x + grid.cellSize - 2), (y + grid.cellSize - 2));
    }
}

static void drawSelectedColor(HDC hdc, int x, int y)
{
 const int      shiftX = 120;
 const int      shiftY = 130;
 const int      cellSize = 50;
 char           status[128];

 if (player == black) {
     sprintf_s(status, sizeof(status), "You choose black");
 }
 else if (player == white) {
     sprintf_s(status, sizeof(status), "You choose white");
 }
 else {
     sprintf_s(status, 16, "Not choosen");
 }

 drawString(hdc, status, strlen(status), x + 60,
     y + shiftY + 25);


    /* Draw the cell background. */
    SelectObject(hdc, GetStockObject(NULL_PEN));

    SelectObject(hdc, board_brush);
    Rectangle(hdc, x + shiftX, y + shiftY, x + shiftX + cellSize, y + shiftY + cellSize);

    if (player != empty) {
        /* Draw the disk. */
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, player == black ? black_brush : white_brush);
        SelectObject(hdc, player == black ? SetDCPenColor(hdc, RGB(200, 200, 200)) : SetDCPenColor(hdc, RGB(064, 064, 064)));

        //SelectObject(hdc, GetStockObject(DC_BRUSH));      //benchmark
        //SetDCBrushColor(hdc, RGB(64, 128, 248));
        //SelectObject(hdc, GetStockObject(DC_PEN));
        //SetDCPenColor(hdc, RGB(255, 128, 032));

        Ellipse(hdc, (x + shiftX + 2), (y + shiftY + 2), (x + shiftX + cellSize - 2), (y + shiftY + cellSize - 2));
    }
}

static void drawCounter(HDC hdc, int x, int y)
{
    const int      shiftX = 90;
    const int      shiftY = 225;
    char           status[128];

    sprintf_s(status, sizeof(status), "Count of moves: %d", mainMoveCounter);
    drawString(hdc, status, strlen(status), x + shiftX, y + shiftY);
}

static void drawScore(HDC hdc, int x, int y)
{
    const int      shiftX = 90;
    const int      shiftY = 250;
    int blackScore, whiteScore;
    char           status[128];

    mainScore(score, board, grid.rowNumbr, grid.colNumbr, black, white);
    blackScore = score[0];
    whiteScore = score[1];

    sprintf_s(status, sizeof(status), "Black - %d  |  %d - White", blackScore, whiteScore);
    drawString(hdc, status, strlen(status), x + shiftX, y + shiftY);
}


void InitWindowGame(HDC hdc)                            //-------!!!!-------
{


}

/* Check whether the position is over an Othello cell. */
static bool gridHitCheck(int x, int y, int* row, int* col)
{
    *row = (y - grid.y) / (grid.cellSize + CELL_GAP);
    *col = (x - grid.x) / (grid.cellSize + CELL_GAP);

    if (*row >= 0 && *row < grid.rowNumbr && *col >= 0 && *col < grid.colNumbr) {
        return true;
    }

    return false;
}

static void selectCell(HWND hWnd, int row, int col)
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
static void invalidateCell(HWND hWnd, int row, int col)
{
    RECT rect;

    rect.left = grid.x + col * (grid.cellSize + CELL_GAP);
    rect.top = grid.y + row * (grid.cellSize + CELL_GAP);
    rect.right = rect.left + grid.cellSize;
    rect.bottom = rect.top + grid.cellSize;

    InvalidateRect(hWnd, &rect, false);
    //InvalidateRect(hWnd, NULL, false);
}

static void onKeyPress(HWND hWnd, WPARAM wParam)
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

static void onMouseClick(HWND hWnd)
{
    if (curentMove == gameOver) {
        InitWindowGame(hWnd);
        //new_game();                                                //------------!!!!------------
        InvalidateRect(hWnd, NULL, TRUE);
        return;
    }

    if (curentMove == player && grid.rowSel >= 0) 
    {
        makeMove(hWnd, grid.rowSel, grid.colSel);
    }
}

/* Make a move for the current player and transition the game state. */
static void makeMove(HWND hWnd, int rowX, int colY)
{
    int rowSel, colSel;
    assert(curentMove == player || curentMove == computer);

    if (curentMove == player) {
        if (!(board[rowX][colY].possible == player)) {
            /* Illegal move; ignored. */
            return;
        }

        writeCell(board, grid.rowNumbr, grid.colNumbr, rowX, colY, curentMove);
        crossCount = 0;
        mainMoveCounter++;
        curentMove = computer;
    }
    else if (curentMove == computer) {
        writeCell(board, grid.rowNumbr, grid.colNumbr, rowX, colY, curentMove);
        crossCount = 0;
        mainMoveCounter++;
        curentMove = player;
    }

    movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);

    if (crossCount >= 2) {
        curentMove = gameOver;
    }
    else if (curentMove == computer && !movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove)) 
    {
        crossCount++;
        curentMove = player;
        movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
    }
    else if (curentMove == player && !movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove)) 
    {
        crossCount++;
        curentMove = computer;
        movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove);
    }

    InvalidateRect(hWnd, NULL, TRUE);
}

void CenterWindow(HWND hWnd) 
{
    RECT rc = { 0 };

    GetWindowRect(hWnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hWnd, HWND_TOP, (screen_w - win_w) / 2,
        (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

void DrawPixels(HWND hWnd) {

    PAINTSTRUCT ps;
    RECT r;

    GetClientRect(hWnd, &r);

    if (r.bottom == 0) {

        return;
    }

    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < 2; i++) {
        //Sleep(5);
        int x = rand() % r.right;
        int y = rand() % r.bottom;
        //SetPixel(hdc, x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        Ellipse(hdc, x, y, x+100, y+100);
    }

    EndPaint(hWnd, &ps);
}

//int x = GetSystemMetrics(SM_CXSCREEN);
//int y = GetSystemMetrics(SM_CYSCREEN);
//wprintf(L"The screen size is: %dx%d\n", x, y);
