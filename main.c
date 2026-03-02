#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#define WM_SYSMENU  0x0313

LRESULT CALLBACK GlassWndProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            WTA_OPTIONS opts;
            MARGINS margins;

            opts.dwFlags = WTNCA_NODRAWCAPTION | WTNCA_NODRAWICON;
            opts.dwMask = WTNCA_NODRAWCAPTION | WTNCA_NODRAWICON;
            SetWindowThemeAttribute(hwnd, WTA_NONCLIENT, &opts, sizeof(WTA_OPTIONS));

            margins.cxLeftWidth = -1;
            margins.cxRightWidth = -1;
            margins.cyTopHeight = -1;
            margins.cyBottomHeight = -1;
            DwmExtendFrameIntoClientArea(hwnd, &margins);
            return 0;
        }
        case WM_NCHITTEST:
        {
            LRESULT lrHit = DefWindowProcW(hwnd, uMsg, wParam, lParam);
            if (lrHit == HTCLIENT)
                lrHit = HTCAPTION;
            return lrHit;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

void main(void)
{
    WNDCLASSW wc;
    HWND hwnd;
    MSG msg;
    STARTUPINFOW si;
    int nCmdShow;

    wc.lpfnWndProc = GlassWndProc;
    wc.hInstance = HINST_THISCOMPONENT;
    wc.hIcon = LoadIconW(HINST_THISCOMPONENT, MAKEINTRESOURCEW(1));
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = L"GlassWnd";
    RegisterClassW(&wc);

    hwnd = CreateWindowExW(
        0,
        L"GlassWnd",
        L"GlassWnd",
        WS_OVERLAPPEDWINDOW,
        ((GetSystemMetrics(SM_CXSCREEN) / 2) - 250),
        ((GetSystemMetrics(SM_CYSCREEN) / 2) - 250),
        500, 500,
        NULL, NULL, HINST_THISCOMPONENT, NULL);

    GetStartupInfoW(&si);
    if (si.dwFlags & STARTF_USESHOWWINDOW)
        nCmdShow = si.wShowWindow;
    else
        nCmdShow = SW_SHOWNORMAL;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    ExitProcess(0);
}