#include "framework.h"
#include "Rectilinear Polygon Partitioning.h"
#include "Grid.h"
#include "print.h"

#define MAX_LOADSTRING 100
#define CONTAINS(rect, x, y) (x >= rect.left) && (x <= rect.right) && (y >= rect.top) && (y <= rect.bottom)
#define DOTSELECTED selectedDot != nullptr
#define REDRAWRECT(rectAddress) RedrawWindow(mainWindow, rectAddress, NULL, RDW_INVALIDATE)

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND mainWindow;
Grid dotGrid;
Dot* selectedDot;
bool trackingMouse;
HWND resetButton;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void OnPaint(HDC&);
void TrackMouse();
void selectDot(Dot&, bool redraw = true);
void deselectDot(bool redraw = true);
void reselectDot(Dot&, bool redraw = true);

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RECTILINEARPOLYGONPARTITIONING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RECTILINEARPOLYGONPARTITIONING));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RECTILINEARPOLYGONPARTITIONING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RECTILINEARPOLYGONPARTITIONING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance;

   mainWindow = CreateWindowW(
       szWindowClass,
       szTitle,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT,
       0,
       CW_USEDEFAULT,
       0,
       nullptr,
       nullptr,
       hInstance,
       nullptr
   );

   if (!mainWindow) {
      return FALSE;
   }

   ShowWindow(mainWindow, nCmdShow);
   UpdateWindow(mainWindow);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            dotGrid = Grid(hWnd, 7, 5);
            selectedDot = nullptr;

            RECT gridRect = dotGrid.getRect();
            resetButton = CreateWindow(
                L"BUTTON",
                L"Reset",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                ((gridRect.right - gridRect.left) / 2) - 50 + gridRect.left,
                10,
                100,
                50,
                hWnd,
                NULL,
                hInst,
                NULL
            );
            TrackMouse();
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case BN_CLICKED:
                if (lParam == (int)resetButton) {
                    selectedDot = nullptr;
                    RECT oldGridRect = dotGrid.getRect();
                    dotGrid = Grid(mainWindow, 7, 5);
                    RECT newGridRect = dotGrid.getRect();
                    RECT redrawRect;
                    redrawRect.left = min(oldGridRect.left, newGridRect.left);
                    redrawRect.top = min(oldGridRect.top, newGridRect.top);
                    redrawRect.right = max(oldGridRect.right, newGridRect.right);
                    redrawRect.bottom = max(oldGridRect.bottom, newGridRect.bottom);
                    REDRAWRECT(&redrawRect);
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
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
            OnPaint(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            const int mouseX = GET_X_LPARAM(lParam);
            const int mouseY = GET_Y_LPARAM(lParam);
            for (std::vector<Dot>& dotCol : dotGrid.dots) {
                for (Dot& dot : dotCol) {
                    RECT dotRect = dot.getRect();
                    if (
                        CONTAINS(dotRect, mouseX, mouseY) &&
                        !dot.selected &&
                        dot.forward == nullptr
                    ) {
                        selectDot(dot);
                    }
                }
            }
        }
        break;
    case WM_LBUTTONUP:
        deselectDot();
        break;
    case WM_MOUSEMOVE:
        if (DOTSELECTED) {
            const int mouseX = GET_X_LPARAM(lParam);
            const int mouseY = GET_Y_LPARAM(lParam);
            for (std::vector<Dot>& dotCol : dotGrid.dots) {
                for (Dot& dot : dotCol) {
                    RECT dotRect = dot.getRect();
                    if (
                        CONTAINS(dotRect, mouseX, mouseY) &&
                        dot.backward == nullptr
                    ) {
                        const unsigned int xDiff = abs((int)(selectedDot->getGridX() - dot.getGridX()));
                        const unsigned int yDiff = abs((int)(selectedDot->getGridY() - dot.getGridY()));
                        if (
                            (xDiff == 1 && yDiff == 0) ||
                            (xDiff == 0 && yDiff == 1)
                        ) {
                            selectedDot->forward = &dot;
                            dot.backward = selectedDot;
                            selectedDot->selected = false;
                            if (dot.forward == nullptr) {
                                dot.selected = true;
                            }
                            RECT redrawRect = selectedDot->getRect();
                            redrawRect.left = min(redrawRect.left, dotRect.left);
                            redrawRect.top = min(redrawRect.top, dotRect.top);
                            redrawRect.right = max(redrawRect.right, dotRect.right);
                            redrawRect.bottom = max(redrawRect.bottom, dotRect.bottom);
                            REDRAWRECT(&redrawRect);
                            selectedDot = dot.selected ? &dot : nullptr;
                        }
                    }
                }
            }
        }

        if (!trackingMouse) {
            TrackMouse();
        }
        break;
    case WM_MOUSELEAVE:
        deselectDot();
        trackingMouse = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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

void OnPaint(HDC& hdc) {
    dotGrid.OnDraw(hdc);
}

void TrackMouse() {
    tagTRACKMOUSEEVENT mouseLeave = tagTRACKMOUSEEVENT();
    mouseLeave.cbSize = sizeof(tagTRACKMOUSEEVENT);
    mouseLeave.dwFlags = TME_LEAVE;
    mouseLeave.hwndTrack = mainWindow;
    mouseLeave.dwHoverTime = HOVER_DEFAULT;
    TrackMouseEvent(&mouseLeave);
    trackingMouse = true;
}

void selectDot(Dot& dot, bool redraw) {
    dot.selected = true;
    selectedDot = &dot;
    if (redraw) {
        RECT dotRect = dot.getRect();
        REDRAWRECT(&dotRect);
    }
}

void deselectDot(bool redraw) {
    if (DOTSELECTED) {
        selectedDot->selected = false;
        if (redraw) {
            RECT selectedDotRect = selectedDot->getRect();
            REDRAWRECT(&selectedDotRect);
        }
        selectedDot = nullptr;
    }
}

void reselectDot(Dot& dot, bool redraw) {
    deselectDot(redraw);
    selectDot(dot, redraw);
}