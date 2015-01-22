

#ifndef WND_DISPLAY
#define WND_DISPLAY

#include <Windows.h>

typedef enum tagDISPLAYCOLS
{
	DISPLAY_BACKGROUND = 0,
	DISPLAY_INACTIVE,
	DISPLAY_ACTIVE
} DISPLAYCOLS, *LPDISPLAYCOLS;

typedef struct tagDISPLAYSIZE
{
	UINT ux;
	UINT uy;
}DISPLAYSIZE, *LPDISPLAYSIZE;

typedef struct tagDisplay
{
	LPVOID data;
	DISPLAYSIZE size;
} DISPLAY, *LPDISPLAY;

typedef struct tagWNDDISPLAY
{
	DISPLAY display;
	HBRUSH hbrBrushes[3];	
	HBITMAP hBitmap;	
} WNDDISPLAY, *LPWNDDISPLAY;

ATOM WndDisplayRegister(
	void
	);

BOOL WndDisplayGetSize(
	HWND hWnd,
	LPDISPLAYSIZE lpSize
	);

BOOL WndDisplaySetSize(
	HWND hWnd,
	UINT ux,
	UINT uy
	);

LPDISPLAY WndDisplayGetData(
	HWND hWnd
	);

BOOL WndDisplayRedraw(
	HWND hWnd
	);

BOOL WndDisplaySetColor(
	HWND hWnd,
	DISPLAYCOLS iColor,
	COLORREF color
	);





#endif	/* #ifndef WND_DISPLAY */