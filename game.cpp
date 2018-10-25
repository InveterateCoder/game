#undef UNICODE
#undef _UNICODE
#include<Windows.h>
#include<windowsx.h>
#include<stdlib.h>
#include<time.h>
#include"resource.h"
#pragma comment(lib,"winmm.lib")
HWND hWndM, hWndD;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcD(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI WinThread(LPVOID lpParam)
{
	MSG msg;
	WNDCLASS wc;
	RECT rect;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProcD;
	wc.lpszClassName = "Game7ClassD";
	wc.lpszMenuName = NULL;
	wc.style = 0;
	if (!RegisterClass(&wc))
	{
		PostMessage(hWndM, WM_CLOSE, 0, 0);
		return 0;
	}
	GetClientRect(hWndM, &rect);
	hWndD = CreateWindowEx(WS_EX_STATICEDGE, "Game7ClassD", NULL, WS_CHILD | WS_VISIBLE,
		20, 50, rect.right - 40, 200, hWndM, NULL, GetModuleHandle(NULL), NULL);
	if (!hWndD)
	{
		PostMessage(hWndM, WM_CLOSE, 0, 0);
		return 0;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hThread;
	MSG msg;
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "Game7Class";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = 0;
	if (!RegisterClass(&wc))
		return 0;
	hWndM = CreateWindow("Game7Class", "Hakob's 7", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, 900, 485, NULL, NULL, hInstance, NULL);
	if (!hWndM)
		return 0;
	ShowWindow(hWndM, nCmdShow);
	hThread = CreateThread(NULL, 0, WinThread, NULL, 0, NULL);
	if (!hThread)
		PostMessage(hWndM, WM_CLOSE, 0, 0);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	WaitForSingleObject(hThread, INFINITE);
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, hCompDC;
	PAINTSTRUCT ps;
	static HMENU hMenu;
	static unsigned char game;
	static HBITMAP hBm49, hBm45;
	static RECT rect;
	static HFONT hFont;
	HFONT hFontTmp;
	LOGFONT lf;
	COLORREF col;
	static HWND hButton;
	switch (uMsg)
	{
	case WM_CREATE:
		hMenu = GetSubMenu(GetMenu(hWnd), 0);
		CheckMenuRadioItem(hMenu, 0, 1, 0, MF_BYPOSITION);
		game = 49;
		hBm49 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
		hBm45 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
		SetRect(&rect, 60, 275, 190, 360);
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = 78;
		wsprintf(lf.lfFaceName, "Comic Sans MS");
		hFont = CreateFontIndirect(&lf);
		hButton = CreateWindow("BUTTON", "Play", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			760, 293, 50, 50, hWnd, (HMENU)ID_PLAY, (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE), NULL);
		break;
	case WM_SETFOCUS:
		SetFocus(hButton);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_GAME_49:
			if (game == 45)
			{
				game = 49;
				CheckMenuRadioItem(hMenu, 0, 1, 0, MF_BYPOSITION);
				InvalidateRect(hWnd, &rect, FALSE);
			}
			break;
		case ID_GAME_45:
			if (game == 49)
			{
				game = 45;
				CheckMenuRadioItem(hMenu, 0, 1, 1, MF_BYPOSITION);
				InvalidateRect(hWnd, &rect, FALSE);
			}
			break;
		case ID_ABOUT:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			break;
		case ID_PLAY:
			PostMessage(hWndD, WM_USER, (WPARAM)game, 0);
			PlaySound("MY_SOUN2", GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		default:;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		RoundRect(hDC, 40, 265, 845, 370, 50, 50);
		hFontTmp = SelectObject(hDC, hFont);
		col = SetTextColor(hDC, RGB(128, 128, 128));
		TextOut(hDC, 305, 280, "Good Luck!", 10);
		SetTextColor(hDC, col);
		SelectObject(hDC, hFontTmp);
		switch (game)
		{
		case 49:
			hCompDC = CreateCompatibleDC(hDC);
			SelectObject(hCompDC, hBm49);
			BitBlt(hDC, 60, 275, 130, 85, hCompDC, 0, 0, SRCCOPY);
			DeleteDC(hCompDC);
			break;
		case 45:
			hCompDC = CreateCompatibleDC(hDC);
			SelectObject(hCompDC, hBm45);
			BitBlt(hDC, 60, 275, 130, 85, hCompDC, 0, 0, SRCCOPY);
			DeleteDC(hCompDC);
			break;
		default:
			break;
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hFont);
		DeleteObject(hBm45);
		DeleteObject(hBm49);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProcD(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	static HFONT hFont;
	HFONT hFontTmp;
	LOGFONT lf;
	DWORD x, y;
	static char arr[7][3];
	DWORD dwarr[7];
	static RECT rcNumbers[7];
	static BOOL pushed;
	static UCHAR game;
	switch (uMsg)
	{
	case WM_CREATE:
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = 39;
		hFont = CreateFontIndirect(&lf);
		srand((DWORD)time(NULL));
		x = 31;
		y = x + 90;
		for (UCHAR c = 0; c < 7; c++, x = y + 25, y = x + 90)
		{
			SetRect(&rcNumbers[c], x, 55, y, 145);
		}
		break;
	case WM_USER:
		pushed = TRUE;
		if ((UCHAR)wParam == 49)
			game = 7;
		else
			game = 6;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		hFontTmp = SelectObject(hDC, hFont);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		switch (game)
		{
		case 7:
			if (pushed)
			{
				for (UCHAR c = 0; c < 7; c++)
				{
					x = rand() % 49 + 1;
					for (char z = c - 1; z >= 0; z--)
					{
						if (x == dwarr[z])
						{
							x = rand() % 49 + 1;
							z = c;
						}
					}
					dwarr[c] = x;
					_itoa_s(x, arr[c], 3, 10);
					DrawText(hDC, arr[c], -1, &rcNumbers[c], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				pushed = FALSE;
			}
			else
			{
				for (UCHAR c = 0; c < 7; c++)
				{
					DrawText(hDC, arr[c], -1, &rcNumbers[c], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
			}
			break;
		case 6:
			if (pushed)
			{
				for (UCHAR c = 0; c < 6; c++)
				{
					x = rand() % 45 + 1;
					for (char z = c - 1; z >= 0; z--)
					{
						if (x == dwarr[z])
						{
							x = rand() % 45 + 1;
							z = c;
						}
					}
					dwarr[c] = x;
					_itoa_s(x, arr[c], 3, 10);
					DrawText(hDC, arr[c], -1, &rcNumbers[c], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				DrawText(hDC, ".", -1, &rcNumbers[6], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				pushed = FALSE;
			}
			else
			{
				for (UCHAR c = 0; c < 6; c++)
				{
					DrawText(hDC, arr[c], -1, &rcNumbers[c], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				DrawText(hDC, ".", -1, &rcNumbers[6], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			break;
		default:;
		}
		SelectObject(hDC, hFontTmp);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd;
	LOGFONT lf;
	static HFONT hFont1, hFont2;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hWnd = GetDlgItem(hDlg, ID_H);
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = 28;
		lstrcpy((LPSTR)&lf.lfFaceName, "Verdana");
		hFont1 = CreateFontIndirect(&lf);
		SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont1, TRUE);
		hWnd = GetDlgItem(hDlg, ID_Y);
		lf.lfHeight = 24;
		lstrcpy((LPSTR)&lf.lfFaceName, "Comic Sans MS");
		hFont2 = CreateFontIndirect(&lf);
		SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont2, TRUE);
		PlaySound("MY_SOUND", GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == GetDlgItem(hDlg, ID_H))
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (DWORD)GetSysColorBrush(COLOR_3DFACE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			PlaySound(NULL, GetModuleHandle(NULL), SND_ASYNC | SND_LOOP);
			DeleteObject(hFont1);
			DeleteObject(hFont2);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
//end