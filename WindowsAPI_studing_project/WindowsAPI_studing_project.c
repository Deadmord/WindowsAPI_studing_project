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
#include "WindowsAPI_studing_project.h"

#define MAX_LOADSTRING 100
#define ID_HOTKEY 1     //Hotkey Ctrl+C

#define IDM_FILE_NEW 1  //Menu
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

HWND hwndSta1;
HWND hwndSta2;


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

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);  // ???
    UNREFERENCED_PARAMETER(lpCmdLine);      // ???

    // TODO: Place code here.
    
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPISTUDINGPROJECT));
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
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

    HMENU       hMenu;  //A popup menu 
    POINT       point;

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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

        StringCbPrintfW(buf, BUF_LEN, L"%ld", rect.left);
        SetWindowTextW(hwndSta1, buf);

        StringCbPrintfW(buf, BUF_LEN, L"%ld", rect.top);
        SetWindowTextW(hwndSta2, buf);
    
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
}
