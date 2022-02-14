/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// Helper.h :  Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(__HELPER_H_)
#define __HELPER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlWinApp.h"

/////////////////////////////////////////////////////////////////////////////
// Helper functions

void DrawTheBackground(CWnd *pWnd, CDC *pDC, CPalette *pPalette, CBitmap *pBitmap, BOOL bTile = FALSE, LPPOINT pTopTeft = NULL);
BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &palette);
unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
void DefaultPaletteEntries(LOGPALETTE  *pPal);
COLORREF AFX_EXT_API BlendColors(COLORREF colorInput, COLORREF colorUnchanged, COLORREF colorOldPure, COLORREF colorNewPure);

LRESULT AFX_EXT_API PostMessageTimeout(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, 
						   UINT fuFlags, UINT uTimeout, LPDWORD lpdwResult, 
						   BOOL bRetry = FALSE);
OSRUNNING GetOSRunning();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(__HELPER_H_)
