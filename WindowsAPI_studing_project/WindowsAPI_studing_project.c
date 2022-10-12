/*
#define CELL_GAP 1       // Cell gap in pixels.
#define MIN_SIZE 300     // Minimum window size. 
#define INIT_SIZE 450    // Initial window size. 
#define WM_WHITE_MOVE (WM_USER + 0)

static const char APP_NAME[] = "othello"; // Not found
static HBRUSH background_brush, board_brush, highlight_brush,
white_brush, black_brush;

static othello_t board;
static enum { BLACKS_MOVE, WHITES_MOVE, GAME_OVER } state;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance,
    LPSTR cmd_line, int cmd_show)
{
    WNDCLASS window_class;  //Not found
    HWND window;            //Not found
    HACCEL accelerators;
    MSG message;
*/
//#pragma comment(lib, "Ws2_32.lib")

#include "framework.h"
#include <time.h>
#include <winuser.h>
#include "WindowsAPI_studing_project.h"

#define MAX_LOADSTRING 100
#define ID_HOTKEY 1     //Hotkey Ctrl+C

#define IDM_FILE_NEW 1  //Menu
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

HWND hwndSta1;
HWND hwndSta2;
HWND MsgeAll;

RECT        rectMove = {0};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"MyWindowTitle";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = L"MyWindow";            // the main window class name



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PanelProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                CenterWindow(HWND);
void                RegisterRedPanelClass(void);
void                RegisterBluePanelClass(void);
void                CreateLabels(HWND);
void                FlashWindowLocalFunc(HWND);
void                AddMenus(HWND); //----Menu---
void                MoveRect(RECT* rect); 

void DrawPixels(HWND hWnd);
void DrawLinesLocal(HWND hWnd);
void DoDrawing(HWND hwnd);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);  // ???
    UNREFERENCED_PARAMETER(lpCmdLine);      // ???

    // TODO: Place code here.
    rectMove.left = 100;
    rectMove.top = 300;
    // Initialize global strings
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); // Я закомментил, лучше разкоментить
    //LoadStringW(hInstance, IDC_WINDOWSAPISTUDINGPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance); // ???

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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

    srand(time(NULL));
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW ;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPISTUDINGPROJECT));
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_GRAYTEXT +1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPISTUDINGPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wchar_t     buf[10];
    size_t      BUF_LEN = sizeof(buf);
    RECT        rect;
    LPPOINT     pPnt;
    pPnt = malloc(sizeof(*pPnt));

    HMENU       hMenu;  //A popup menu 
    POINT       point;

    //SetWindowTextW(MsgeAll, message);
    

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_FILE_NEW:
            case IDM_FILE_OPEN:
                MessageBeep(MB_ICONINFORMATION);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_FILE_QUIT:
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            //// TODO: Add any drawing code that uses hdc here...
            //EndPaint(hWnd, &ps);

            //DrawPixels(hWnd);


            DrawLinesLocal(hWnd, &rectMove);
            InvalidateRect(hWnd, NULL, FALSE);
            //DoDrawing(hWnd);
            
            
        }
        break;

    case WM_CREATE:
        AddMenus(hWnd); //Menu

        InitCommonControls();

        RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
        CreateLabels(hWnd);

        RegisterRedPanelClass();
        CreateWindowExW(0L, L"RedPanelClass", NULL,
            WS_CHILD | WS_VISIBLE,
            20, 20, 80, 80,
            hWnd, (HMENU)1, NULL, NULL);

        RegisterBluePanelClass();
        CreateWindowExW(0L, L"BluePanelClass", NULL,
            WS_CHILD | WS_VISIBLE,
            120, 20, 80, 80,
            hWnd, (HMENU)2, NULL, NULL);

        break;

    case WM_HOTKEY:
        if ((wParam) == ID_HOTKEY) 
        {
            CenterWindow(hWnd);
        }
   
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {

            int ret = MessageBoxW(hWnd, L"Are you sure to quit?",
                L"Message", MB_OKCANCEL);

            if (ret == IDOK) {

                SendMessage(hWnd, WM_CLOSE, 0, 0);
            }
        }

        break;

    case WM_MOVE:
    
        GetWindowRect(hWnd, &rect);
        GetCursorPos(pPnt);

        //StringCbPrintfW(buf, BUF_LEN, L"%ld", rect.left);
        //SetWindowTextW(hwndSta1, buf);

        //StringCbPrintfW(buf, BUF_LEN, L"%ld", rect.top);
        //SetWindowTextW(hwndSta2, buf);
    
        break;
    
    case WM_LBUTTONDOWN:

        GetCursorPos(pPnt);
        ScreenToClient(hWnd, pPnt);

        StringCbPrintfW(buf, BUF_LEN, L"%ld", pPnt->x);
        SetWindowTextW(hwndSta1, buf);

        StringCbPrintfW(buf, BUF_LEN, L"%ld", pPnt->y);
        SetWindowTextW(hwndSta2, buf);

        StringCbPrintfW(buf, BUF_LEN, L"%ld", wParam);
        SetWindowTextW(MsgeAll, buf);

        break;

    case WM_RBUTTONUP:

        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        hMenu = CreatePopupMenu();
        ClientToScreen(hWnd, &point);

        AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
        AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, MF_STRING, IDM_ABOUT, L"&About");
        AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;

    case WM_DESTROY:
        UnregisterHotKey(hWnd, ID_HOTKEY);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK PanelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {

    case WM_LBUTTONUP:

        MessageBeep(MB_OK);
        MessageBoxW(NULL, L"First Program", L"First", MB_OK);
        break;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
void AddMenus(HWND hWnd) { //Menu handler

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_ABOUT, L"&About");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&View");
    SetMenu(hWnd, hMenubar);
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

void RegisterRedPanelClass(void) 
{

    HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));

    WNDCLASSW rwc = { 0 };

    rwc.lpszClassName = L"RedPanelClass";
    rwc.hbrBackground = hbrush;
    rwc.lpfnWndProc = PanelProc;
    rwc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&rwc);
}

void RegisterBluePanelClass(void) 
{

    HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

    WNDCLASSW rwc = { 0 };

    rwc.lpszClassName = L"BluePanelClass";
    rwc.hbrBackground = hbrush;
    rwc.lpfnWndProc = PanelProc;
    rwc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&rwc);
}

void CreateLabels(HWND hWnd) 
{

    CreateWindowW(L"static", L"x: ",
        WS_CHILD | WS_VISIBLE,
        10, 110, 25, 25,
        hWnd, (HMENU)1, NULL, NULL);

    hwndSta1 = CreateWindowW(L"static", L"150",
        WS_CHILD | WS_VISIBLE,
        40, 110, 55, 25,
        hWnd, (HMENU)2, NULL, NULL);

    CreateWindowW(L"static", L"y: ",
        WS_CHILD | WS_VISIBLE,
        10, 130, 25, 25,
        hWnd, (HMENU)3, NULL, NULL);

    hwndSta2 = CreateWindowW(L"static", L"150",
        WS_CHILD | WS_VISIBLE,
        40, 130, 55, 25,
        hWnd, (HMENU)4, NULL, NULL);

    MsgeAll = CreateWindowW(L"static", L"150",
        WS_CHILD | WS_VISIBLE,
        240, 110, 55, 25,
        hWnd, (HMENU)5, NULL, NULL);
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
void DrawLinesLocal(HWND hWnd, RECT* rect)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    MoveToEx(hdc, 50, 50, NULL);
    LineTo(hdc, 250, 50);

    HPEN hWhitePen = GetStockObject(WHITE_PEN);
    HPEN hOldPen = SelectObject(hdc, hWhitePen);

    MoveToEx(hdc, 50, 100, NULL);
    LineTo(hdc, 250, 100);

    SelectObject(hdc, hOldPen);

    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(016, 128, 032));
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB(255, 255, 255));

     MoveRect(rect);

    //Rectangle(hdc, rect->left, rect->top, rect->right, rect->bottom);

  
    SetDCBrushColor(hdc, RGB(225, 0, 225));
    Ellipse(hdc, rect->left, rect->top, rect->right, rect->bottom);

    char str[100];
    sprintf_s(str, sizeof(str), "Sorry, Ilia...");
    TextOutA(hdc, 200, 250, str, strlen(str) + 1);

    Sleep(1);
    EndPaint(hWnd, &ps);
}

void DoDrawing(HWND hwnd) {

    LOGBRUSH brush;
    COLORREF col = RGB(0, 0, 0);
    DWORD pen_style = PS_SOLID | PS_JOIN_MITER | PS_GEOMETRIC;

    brush.lbStyle = BS_SOLID;
    brush.lbColor = col;
    brush.lbHatch = 0;

    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    HPEN hPen1 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
    HPEN holdPen = SelectObject(hdc, hPen1);

    POINT points[5] = { { 30, 230 }, { 130, 230 }, { 130, 300 },
        { 30, 300 }, { 30, 230} };
    Polygon(hdc, points, 5);
    
    pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;
    HPEN hPen2 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);

    SelectObject(hdc, hPen2);
    DeleteObject(hPen1);

    POINT points2[5] = { { 160, 230 }, { 260, 230 }, { 260, 300 },
        { 160, 300 }, {160, 230 } };
    MoveToEx(hdc, 130, 30, NULL);
    Polygon(hdc, points2, 5);

    pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_ROUND;
    HPEN hPen3 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);

    SelectObject(hdc, hPen3);
    DeleteObject(hPen2);

    POINT points3[5] = { { 290, 230 }, { 390, 230 }, { 390, 300 },
        { 290, 300 }, {290, 230 } };
    MoveToEx(hdc, 260, 30, NULL);
    Polygon(hdc, points3, 5);

    SelectObject(hdc, holdPen);
    DeleteObject(hPen3);

    EndPaint(hwnd, &ps);
}       

void MoveRect(RECT* rect)
{
    rect->left += 1;
    rect->top += 3;
    if (rect->left > 1210 || rect->left < 0) rect->left = 0;
    if (rect->top > 500 || rect->top < 0) rect->top = 0;
    rect->right = rect->left + 100;
    rect->bottom = rect->top + 100;
}