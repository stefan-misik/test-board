

#include "wndDisplay.h"

static HANDLE g_hHeap;

/******************************************************************************/
static BOOL UpdateBitmap(
	HWND hWnd
	)
{
	RECT client;
	LPWNDDISPLAY lpWd;
	SIZE dim;
	HDC hdcMem, hdc;
	HGDIOBJ original_pen, original_brush;
	CHAR * data;
	INT uRow, uCol;
	INT uDispW, uDispH;
	INT uOffX, uOffY;
	INT uPixelW, uPixelH;
	INT uDrawAreaW, uDrawAreaH;
	INT uGridW, uGridH;

	lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hWnd, 0);
	if (NULL == lpWd)
	{
		return FALSE;
	}

	/* Prepare bitmap */
	GetClientRect(hWnd, &client);
	hdc = GetWindowDC(hWnd);

	hdcMem = CreateCompatibleDC(hdc);

	if (NULL != lpWd->hBitmap)
	{
		GetBitmapDimensionEx(lpWd->hBitmap, &dim);
		if (dim.cx < client.right || dim.cy < client.bottom)
		{
			DeleteObject(lpWd->hBitmap);
			lpWd->hBitmap = CreateCompatibleBitmap(hdc, client.right, client.bottom);
			SetBitmapDimensionEx(lpWd->hBitmap, client.right, client.bottom, NULL);
		}
	}
	else
	{
		lpWd->hBitmap = CreateCompatibleBitmap(hdc, client.right, client.bottom);
		SetBitmapDimensionEx(lpWd->hBitmap, client.right, client.bottom, NULL);
	}

	DeleteDC(hdc);



	/* Draw to Bitmap */
	SelectObject(hdcMem, lpWd->hBitmap);

	original_brush = SelectObject(hdcMem, lpWd->hbrBrushes[DISPLAY_BACKGROUND]);
	original_pen = SelectObject(hdcMem, GetStockObject(BLACK_PEN));

	/* Clear Background */
	Rectangle(hdcMem, 0, 0, client.right, client.bottom);

	

	/* Calculate sizes */
	uDrawAreaW = client.right - 10;
	uDrawAreaH = client.bottom - 10;

	uDispW = lpWd->display.size.ux;
	uDispH = lpWd->display.size.uy;

	uGridW = uDrawAreaW / uDispW;
	uGridH = uDrawAreaH / uDispH;

	uPixelW = ((UINT)(uGridW * 0.8) + 1);
	uPixelH = ((UINT)(uGridH * 0.8) + 1);

	uOffX = (((uDrawAreaW % uDispW) / 2) + 4 );
	uOffY = (((uDrawAreaH % uDispH) / 2) + 4 );


	/* Remove Pen */
	SelectObject(hdcMem, GetStockObject(NULL_PEN));


	/* Draw inactive */
	SelectObject(hdcMem, lpWd->hbrBrushes[DISPLAY_INACTIVE]);

	for (uRow = 0; uRow < uDispH; uRow++)
	{
		UINT uPy = uOffY + (uGridH * uRow);
		for (uCol = 0; uCol < uDispW; uCol++)
		{
			UINT uPx = uOffX + (uGridW * uCol);
			Rectangle(hdcMem, uPx - 1, uPy - 1, uPx + uPixelW, uPy + uPixelH);
		}
	}	

	/* Draw active */
	SelectObject(hdcMem, lpWd->hbrBrushes[DISPLAY_ACTIVE]);

	data = lpWd->display.data;

	for (uRow = 0; uRow < uDispH; uRow++)
	{
		UINT uPy = uOffY + (uGridH * uRow);
		for (uCol = 0; uCol < uDispW; uCol++)
		{
			if (((data[((uRow >> 3) * uDispW) + uCol] >> (uRow & 7) & 1)) != 0)
			{
				UINT uPx = uOffX + (uGridW * uCol);
				Rectangle(hdcMem, uPx-1, uPy-1, uPx + uPixelW, uPy + uPixelH);
			}
		}
	}

	SelectObject(hdcMem, original_pen);
	SelectObject(hdcMem, original_brush);

	DeleteDC(hdcMem);

	return TRUE;
}

/******************************************************************************/
static LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{

	switch (uMsg)
	{
		
	case WM_CREATE:
	{
		LPWNDDISPLAY lpWd;
		UINT i;
		

		lpWd = HeapAlloc(g_hHeap, 0, sizeof(WNDDISPLAY));

		SetWindowLongPtr(hwnd, 0, (LONG_PTR)lpWd);
		lpWd->display.data = NULL;
		lpWd->hBitmap = NULL;

		
		

		for (i = 0; i < 3; i++)
		{
			lpWd->hbrBrushes[i] = NULL;
		}

		WndDisplaySetSize(hwnd, 84, 48);

		for (i = 0; i < 504; i++)
		{
			((char *)lpWd->display.data)[i] = (i & 255);
		}

		WndDisplaySetColor(hwnd, DISPLAY_BACKGROUND, RGB(232, 252, 163));
		WndDisplaySetColor(hwnd, DISPLAY_INACTIVE, RGB(198, 229, 132));
		WndDisplaySetColor(hwnd, DISPLAY_ACTIVE, RGB(30, 34, 2));

		UpdateBitmap(hwnd);

		break;
	}

	case WM_DESTROY:
	{
		LPWNDDISPLAY lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hwnd, 0);
		if (NULL != lpWd)
		{
			UINT i;

			if (NULL != lpWd->display.data)
			{
				HeapFree(g_hHeap, 0, lpWd->display.data);
			}

			
			if (NULL != lpWd->hBitmap)
			{
				DeleteObject(lpWd->hBitmap);
			}

			for (i = 0; i < 3; i++)
			{
				if (NULL != lpWd->hbrBrushes[i])
				{
					DeleteObject(lpWd->hbrBrushes[i]);
					lpWd->hbrBrushes[i] = NULL;
				}
			}			

			HeapFree(g_hHeap, 0, lpWd);
		}
		break;
	}

	case WM_ERASEBKGND:
		
		return 1;

	case WM_SIZE:
		UpdateBitmap(hwnd);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc, hdcMem;
		LPWNDDISPLAY lpWd;
			

		lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hwnd, 0);
		if (NULL == lpWd)
		{
			break;
		}

		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, lpWd->hBitmap);

		BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top,
			ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
			hdcMem, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

		DeleteDC(hdcMem);
		
		EndPaint(hwnd, &ps);
		return 0;
	}
		

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}		

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




/******************************************************************************/
ATOM WndDisplayRegister(
	void
	)
{
	WNDCLASSEX cls;
	HCURSOR hCur;
	ATOM ret;

	hCur = LoadCursor(NULL, IDC_CROSS);

	cls.cbSize = sizeof(WNDCLASSEX);
	cls.style = CS_DBLCLKS | CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	cls.lpfnWndProc = WindowProc;
	cls.cbClsExtra = 0;
	cls.cbWndExtra = sizeof(LPWNDDISPLAY);
	cls.hInstance = GetModuleHandle(NULL);
	cls.hIcon = NULL;
	cls.hCursor = hCur;
	cls.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	cls.lpszMenuName = NULL;
	cls.lpszClassName = TEXT("WNDDISPLAYCLASS");
	cls.hIconSm = NULL;

	
	ret = RegisterClassEx(&cls);
	DestroyCursor(hCur);

	g_hHeap = GetProcessHeap();

	return ret;
}

/******************************************************************************/
BOOL WndDisplayGetSize(
	HWND hWnd,
	LPDISPLAYSIZE lpSize
	)
{
	LPWNDDISPLAY lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hWnd, 0);
	
	if (NULL == lpWd)
	{
		return FALSE;
	}

	*lpSize = lpWd->display.size;
	return TRUE;
}

/******************************************************************************/
BOOL WndDisplaySetSize(
	HWND hWnd,
	UINT ux,
	UINT uy
	)
{
	LPWNDDISPLAY lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hWnd, 0);

	if (NULL == lpWd)
	{
		return FALSE;
	}

	if (NULL != lpWd->display.data)
	{
		HeapFree(g_hHeap, 0, lpWd->display.data);
	}

	lpWd->display.size.ux = ux;
	lpWd->display.size.uy = uy;

	lpWd->display.data = HeapAlloc(g_hHeap, 0, (ux * uy) >> 3);

	return TRUE;
}

/******************************************************************************/
LPDISPLAY WndDisplayGetData(
	HWND hWnd
	)
{
	LPWNDDISPLAY lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hWnd, 0);

	if (NULL == lpWd)
	{
		return NULL;
	}

	return &(lpWd->display);
}

/******************************************************************************/
BOOL WndDisplayRedraw(
	HWND hWnd
	)
{
	if (FALSE == UpdateBitmap(hWnd))
	{
		return FALSE;
	}

	return InvalidateRect(hWnd, NULL, FALSE);
}

/******************************************************************************/
BOOL WndDisplaySetColor(
	HWND hWnd,
	DISPLAYCOLS iColor,
	COLORREF color	
	)
{
	LPWNDDISPLAY lpWd = (LPWNDDISPLAY)GetWindowLongPtr(hWnd, 0);

	if (NULL == lpWd)
	{
		return FALSE;
	}

	if (NULL != lpWd->hbrBrushes[iColor])
	{
		DeleteObject(lpWd->hbrBrushes[iColor]);
	}

	lpWd->hbrBrushes[iColor] = CreateSolidBrush(color);	

	return TRUE;
}