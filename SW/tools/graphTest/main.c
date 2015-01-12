#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")


#ifndef UNICODE
#define UNICODE
#endif

#define _WIN32_WINNT 0x600

#include <Windows.h>
#include <strsafe.h>
#include <CommCtrl.h>

#include "resource.h"
#include "wndDisplay.h"

HWND g_hwndMain;


/******************************************************************************/
INT_PTR CALLBACK DialogProc(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{	
	switch (uMsg)
	{
	
	case WM_INITDIALOG:
		InitCommonControls();
		return TRUE;

	case WM_SIZE:
	{
		HWND wnd;
		RECT rc;

		wnd = GetDlgItem(hwndDlg, IDC_DISPLAY);
		MoveWindow(wnd, 11, 11, LOWORD(lParam) - 21, HIWORD(lParam) - 59, FALSE);

		wnd = GetDlgItem(hwndDlg, IDOK);
		GetWindowRect(wnd, &rc);
		MoveWindow(wnd, LOWORD(lParam) - 167, HIWORD(lParam) - 34, rc.right - rc.left, rc.bottom - rc.top, FALSE);

		wnd = GetDlgItem(hwndDlg, IDCANCEL);
		GetWindowRect(wnd, &rc);
		MoveWindow(wnd, LOWORD(lParam) - 86, HIWORD(lParam) - 34, rc.right - rc.left, rc.bottom - rc.top, FALSE);

		InvalidateRect(hwndDlg, NULL, TRUE);

			return FALSE;
	}

	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case 0:
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				DestroyWindow(hwndDlg);
				return 1;
			}
		}
	}

	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
		
	

	default:
		return FALSE;
		break;
	}	
	return FALSE;
}

/******************************************************************************/
INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	MSG msg;
	

	ATOM aCls;

	//g_hMyInstance = hInstance;

	aCls = WndDisplayRegister();


	/* Create main Window */

	
	g_hwndMain = CreateDialogParam(
		hInstance,
		MAKEINTRESOURCE(IDD_MAIN_WND),
		NULL,
		DialogProc,
		(LPARAM)NULL
		);

	DWORD dwErr;
	dwErr = GetLastError();

	if (NULL == g_hwndMain)
	{
		

		MessageBox(NULL, TEXT("Application Initialization Failed!"),
			TEXT("Error!"), MB_OK | MB_ICONHAND);
		return -1;
	}

	
	/* Set caption of main window */
	SetWindowText(g_hwndMain, TEXT("Graphics Test"));

	/* Load and set the icon */
	/*hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	if (NULL != hIcon)
	{
		SendMessage(g_hwndMain, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		SendMessage(g_hwndMain, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		DestroyIcon(hIcon);
	}*/

	
	/* Draw Window */
	ShowWindow(g_hwndMain, nCmdShow);


	/* Enter the message loop */
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{		
		if (FALSE == IsDialogMessage(g_hwndMain, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	
	}

	ExitProcess((UINT)(msg.wParam));

	return (INT)(msg.wParam);
}

