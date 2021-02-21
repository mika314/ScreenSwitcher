// SceeneSwitcher.cpp : Defines the entry point for the application.
//

#include "SceeneSwitcher.h"
#include "Windows.h"
#include "framework.h"
#include <sstream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                     // current instance
WCHAR szTitle[MAX_LOADSTRING];       // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_SCEENESWITCHER, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCEENESWITCHER));

  MSG msg;

  int screen = 0;

  // Main message loop:
  while (true)
  {
    auto res = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
    POINT p;
    GetCursorPos(&p);
    std::wostringstream st;
    st << "Mouse position:" << p.x << ":" << p.y << "\n";

    OutputDebugString(st.str().c_str());
    if (p.x <= 1920 && screen == 1)
    {
      OutputDebugString(L"Switching to screen 0\n");
      screen = 0;

      const auto msg =
        R"({ "jsonrpc": "2.0", "id": 1, "method": "makeSceneActive", "params": { "resource": "ScenesService", "args": ["scene_f67169fb-706c-40c7-be6e-4e839eb94c6c"] }})";

      auto hFile = CreateFile(L"\\\\.\\pipe\\slobs",
                              GENERIC_READ | GENERIC_WRITE,
                              0,                     // dwShareMode
                              nullptr,               // lpSecurityAttributes
                              OPEN_EXISTING,         // dwCreationDisposition
                              FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
                              nullptr                // hTemplateFile
      );

      if (hFile == INVALID_HANDLE_VALUE)
      {
        std::wostringstream st;
        st << "pipe open error last error: " << GetLastError() << "\n";
        OutputDebugString(st.str().c_str());
      }

      char out[80960];
      DWORD l;
      WriteFile(hFile, msg, static_cast<DWORD>(strlen(msg)), &l, nullptr);
      ReadFile(hFile, out, static_cast<DWORD>(sizeof(out)), &l, nullptr);
      //      std::wostringstream st;
      //      st << L"res: " << res << " last error: " << GetLastError() << L" out size:" << l << L"
      //      out: "
      //         << std::string{out, out + l} << L"\n";
      //      OutputDebugString(st.str().c_str());
    }

    if (p.x > 1920 && screen == 0)
    {
      OutputDebugString(L"Switching to screen 1\n");
      screen = 1;
      const auto msg =
        R"({ "jsonrpc": "2.0", "id": 1, "method": "makeSceneActive", "params": { "resource": "ScenesService", "args": ["scene_8d4cb72b-fbe1-4463-813a-2a8714f129e7"] }})";

      auto hFile = CreateFile(L"\\\\.\\pipe\\slobs",
                              GENERIC_READ | GENERIC_WRITE,
                              0,                     // dwShareMode
                              nullptr,               // lpSecurityAttributes
                              OPEN_EXISTING,         // dwCreationDisposition
                              FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
                              nullptr                // hTemplateFile
      );

      if (hFile == INVALID_HANDLE_VALUE)
      {
        std::wostringstream st;
        st << "pipe open error last error: " << GetLastError() << "\n";
        OutputDebugString(st.str().c_str());
      }

      char out[80960];
      DWORD l;
      WriteFile(hFile, msg, static_cast<DWORD>(strlen(msg)), &l, nullptr);
      ReadFile(hFile, out, static_cast<DWORD>(sizeof(out)), &l, nullptr);
      //      std::wostringstream st;
      //      st << "res: " << res << " last error: " << GetLastError() << " out size:" << l
      //         << " out: " << std::string(out, out + l) << "\n";
      //      OutputDebugString(st.str().c_str());
    }

    if (!res)
    {
      Sleep(200);
      continue;
    }

    if (msg.message == WM_QUIT)
      break;
    if (msg.message == WM_DESTROY)
      break;

    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
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

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCEENESWITCHER));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SCEENESWITCHER);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

  HWND hWnd = CreateWindowW(szWindowClass,
                            szTitle,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            0,
                            CW_USEDEFAULT,
                            0,
                            nullptr,
                            nullptr,
                            hInstance,
                            nullptr);

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
  switch (message)
  {
  case WM_COMMAND: {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
    case IDM_EXIT: DestroyWindow(hWnd); break;
    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code that uses hdc here...
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY: PostQuitMessage(0); break;
  default: return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG: return (INT_PTR)TRUE;

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
