#include "Othello_WinAPI.h"
#include <stdio.h>
#include <time.h>			//use in random and dalay
#include <assert.h>			//use in WndProc
#include "Othello_res.h"	//control elements IDs
#include "Othello_const.h"
#include "Resource.h"
#include <commctrl.h>		//Include common control elements defenition

#include "Initialize.h"
#include "Drawing_service.h"
#include "Game_service.h"
#include "Othello_Console.h"        //Function from previos verson Othello game

//************Hey, entry here!***************
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);      // Switch off warnings
    UNREFERENCED_PARAMETER(lpCmdLine);      

    InitializeGlobalVar();
    InitEmptyBoard();
    InitializePaintTools();

    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))    // Perform application initialization.
    {
        return FALSE;
    }
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPISTUDINGPROJECT));
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))        // Main message loop:
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    DeactivatePaintTools();

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
    //wcex.hbrBackground  = background_brush;
    wcex.hbrBackground = CreatePatternBrush((HBITMAP)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));

    
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
    HDC hdc;            //device context
    PAINTSTRUCT ps;
    HMENU       hMenu;  //A popup menu 
    POINT       point;

    LPNMUPDOWN lpnmud;  //UpDownControl
    UINT code;          //UpDownControl

    switch (message)
    {
    case WM_CREATE:
        AppendMenus(hWnd); //Menu
        InitCommonControls();
        windowMenus(hWnd, MENU_POSITION_X , MENU_POSITION_Y);

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
            TRUE ? MF_ENABLED : MF_GRAYED);

        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT); //make TRANSPARENT background

        drawSelectedColor(hdc, MENU_POSITION_X, MENU_POSITION_Y);
        drawCounter(hdc, MENU_POSITION_X, MENU_POSITION_Y);
        drawScore(hdc, MENU_POSITION_X, MENU_POSITION_Y);

        drawGrid(hdc);
        if (curentMove == gameOver) {
            DrawPixels(hWnd, hdc);
        }
        EndPaint(hWnd, &ps);

        //
        //InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_COMMAND:    //  WM_COMMAND  - process the application menu
        {
            if (HIWORD(wParam) == BN_CLICKED) {

                switch (LOWORD(wParam)) 
                {
                case ID_BLACK:
                    order = 1;
                    player = black;
                    computer = white;
                    break;
                case ID_WHITE:
                    order = 2;
                    player = white;
                    computer = black;
                    break;
                }
                InvalidateRect(hWnd, NULL, TRUE);
                SetFocus(hWnd);
            }

            //int wmId = LOWORD(wParam);      // Parse the menu selections:
            switch (LOWORD(wParam))
            {

            case IDM_START: 
            case IDM_NEW_GAME:
                InitWindowGame(hWnd);
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
                grid.rowMenu =  value;
            }
            else if (lpnmud->hdr.idFrom == ID_UPDOWN_COL)
            {
                grid.colMenu = value;
            }
        }
        break;

    case WM_TIMER:
        //MessageBeep(0); for testing
        {   KillTimer(hWnd, ID_TIMER_PCMOVE);
            int rowSel, colSel;
            if (curentMove != 0 && curentMove == computer && mainMoveCounter > 2) {
                if (movePossibilities(board, grid.rowNumbr, grid.colNumbr, curentMove))
                {
                    pcMove(board, grid.rowNumbr, grid.colNumbr, computer, &rowSel, &colSel);
                    SendMessage((HWND)hWnd, WM_PC_MOVE, rowSel, colSel);
                }
                else {
                    crossCount++;
                    curentMove = player;
                }
                if (crossCount >= 2) {
                    curentMove = gameOver;
                }
            }
        }
        return 0;

    case WM_PC_MOVE:
        /* White computed a move. */
        assert(curentMove == computer);
        makeMove(hWnd, (int)wParam, (int)lParam);
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

void AppendMenus(HWND hWnd)     //Menu handler
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

    //-----------Ñhoose a demention------------
    CreateWindowW(L"Button", L"Size of board (Row | Col)",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | TRANSPARENT,
        x, y, 180, 60, hWnd, (HMENU)0, hInst, NULL);

    hUpDownRow = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE
        | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
        0, 0, 0, 0, hWnd, (HMENU)ID_UPDOWN_ROW, NULL, NULL);

    hEditRow = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD
        | WS_VISIBLE | ES_RIGHT, 25, y + 25, 40, 25, hWnd,
        (HMENU)ID_EDIT_ROW, NULL, NULL);

    SendMessageW(hUpDownRow, UDM_SETBUDDY, (WPARAM)hEditRow, 0);
    SendMessageW(hUpDownRow, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
    SendMessageW(hUpDownRow, UDM_SETPOS32, 0, UD_SET_POS);

    hUpDownCol = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE
        | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
        0, 0, 0, 0, hWnd, (HMENU)ID_UPDOWN_COL, NULL, NULL);

    hEditCol = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD
        | WS_VISIBLE | ES_RIGHT, 115, y + 25, 40, 25, hWnd,
        (HMENU)ID_EDIT_COL, NULL, NULL);

    SendMessageW(hUpDownCol, UDM_SETBUDDY, (WPARAM)hEditCol, 0);
    SendMessageW(hUpDownCol, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
    SendMessageW(hUpDownCol, UDM_SETPOS32, 0, UD_SET_POS);

    //-----------Ñhoose a color------------
    CreateWindowW(L"Button", L"Ñhoose a color",
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
        x, y + 285, 80, 40, hWnd, (HMENU)IDM_START, NULL, NULL);

    CreateWindowW(L"Button", L"New game",
        WS_VISIBLE | WS_CHILD,
        x + 90, y + 285, 80, 40, hWnd, (HMENU)IDM_NEW_GAME, NULL, NULL);
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

//int x = GetSystemMetrics(SM_CXSCREEN);
//int y = GetSystemMetrics(SM_CYSCREEN);
//wprintf(L"The screen size is: %dx%d\n", x, y);
