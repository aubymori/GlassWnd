#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <windowsx.h>
#include "resource.h"

const WCHAR c_szClassName[] = L"GlassWnd";

void UpdateGlass(HWND hWnd)
{
	MARGINS mrg = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(hWnd, &mrg);
	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK GlassWndProc(
	HWND   hWnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			WTA_OPTIONS opts = {
				WTNCA_NODRAWCAPTION | WTNCA_NODRAWICON,
				WTNCA_NODRAWCAPTION | WTNCA_NODRAWICON
			};
			SetWindowThemeAttribute(hWnd, WTA_NONCLIENT, &opts, sizeof(opts));
			MARGINS mrg = { -1, -1, -1, -1 };
			DwmExtendFrameIntoClientArea(hWnd, &mrg);
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			RECT rc = { 0 };
			GetClientRect(hWnd, &rc);
			FillRect(hDC, &rc, GetStockObject(BLACK_BRUSH));
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
}

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow
)
{
	WNDCLASSW wc = { 0 };
	wc.lpfnWndProc = GlassWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconW(hInstance, IDI_GLASSWND);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = (COLOR_3DFACE + 1);
	wc.lpszClassName = c_szClassName;
	RegisterClassW(&wc);

	HWND hWnd = CreateWindowExW(
		NULL,
		c_szClassName,
		L"GlassWnd",
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_XVIRTUALSCREEN) + ((GetSystemMetrics(SM_CXSCREEN) / 2) - 250),
		GetSystemMetrics(SM_YVIRTUALSCREEN) + ((GetSystemMetrics(SM_CYSCREEN) / 2) - 250),
		500,
		500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);

	MSG msg = { 0 };
	while (GetMessageW(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;
}