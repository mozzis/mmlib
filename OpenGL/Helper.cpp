/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// Helper.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Helper.h"
#include "MemDC.h"

CRect oldRect(0,0,0,0);

void DrawTheBackground(CWnd *pWnd, CDC *pDC, CPalette *pPalette, CBitmap *pBitmap, BOOL bTile, LPPOINT pTopTeft)
{
	if (pDC->IsPrinting())
		return;

	CRect rect;
	CPalette *pOldPalette = NULL;

	// Select and realize the palette
	if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE && pPalette->m_hObject != NULL)
	{
		pOldPalette = pDC->SelectPalette(pPalette, FALSE);
		pDC->RealizePalette();
	}
	pWnd->GetClientRect(&rect);
	pDC->DPtoLP(rect);

	CMemDC DC(pDC, rect);
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(pDC))
		return;

	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(pBitmap);
	if (bTile)
	{
		for (int i = ((int)floor((double)rect.left/bm.bmWidth))*bm.bmWidth; i <= rect.right; i+=bm.bmWidth)
			for (int j=((int)floor((double)rect.top/bm.bmHeight))*bm.bmHeight; j <= rect.bottom; j+=bm.bmHeight)
				DC->BitBlt(i, j, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	}
	else
	{
		CBrush brush;
		brush.CreateSolidBrush(GetSysColor(COLOR_APPWORKSPACE));
		CBrush* pOldBrush = (CBrush*)DC->SelectObject(&brush);
		CRect crect;
		int left, top;
		if (pTopTeft != NULL)
		{
			left = pTopTeft->x;
			top = pTopTeft->y;
		}
		else
		{
			left = (rect.Width()-bm.bmWidth)/2;
			top = (rect.Height()-bm.bmHeight)/2;
		}
		oldRect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		pDC->GetClipBox(&crect);
		DC->PatBlt(crect.left, crect.top, crect.Width(), crect.Height(), PATCOPY);
		DC->SelectObject(pOldBrush);
		DC->BitBlt(oldRect.left, oldRect.top, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	}
	dcImage.SelectObject(pOldBitmap); 
	pDC->SelectPalette(pOldPalette, FALSE);
	pDC->RealizePalette(); 
}

BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &palette)
{
	LPCTSTR lpszResourceName = (LPCTSTR)nIDResource;
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
		lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION);
	if (hBmp == NULL)
		return FALSE;

	bitmap.Attach(hBmp);

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	bitmap.GetObject(sizeof(ds), &ds);
	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL); // Desktop DC
	if (nColors > 256) 
		palette.CreateHalftonePalette(&dc);
	else
	{
		// Create the palette
		RGBQUAD *pRGB = new RGBQUAD[nColors];

		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&bitmap);
		::GetDIBColorTable(memDC, 0, nColors, pRGB);
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD)nColors;
		for (int i = 0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}
		palette.CreatePalette(pLP);
		delete[] pLP;
		delete[] pRGB;
	}

	return TRUE;
}

static unsigned char threeto8[8] = {
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

static unsigned char twoto8[4] = {
	0, 0x55, 0xaa, 0xff
};

static unsigned char oneto8[2] = {
	0, 255
};

unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char) (i >> shift);

	switch (nbits)
	{
		case 1:
			val &= 0x1;
			return oneto8[val];
		case 2:
			val &= 0x3;
			return twoto8[val];
		case 3:
			val &= 0x7;
			return threeto8[val];
		default:
			return 0;
	}
}

static int defaultOverride[13] = {
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = {
	{ 0,   0,   0,   0 },
	{ 0x80,0,   0,   0 },
	{ 0,   0x80,0,   0 },
	{ 0x80,0x80,0,   0 },
	{ 0,   0,   0x80,0 },
	{ 0x80,0,   0x80,0 },
	{ 0,   0x80,0x80,0 },
	{ 0xC0,0xC0,0xC0,0 },

	{ 192, 220, 192, 0 },
	{ 166, 202, 240, 0 },
	{ 255, 251, 240, 0 },
	{ 160, 160, 164, 0 },

	{ 0x80,0x80,0x80,0 },
	{ 0xFF,0,   0,   0 },
	{ 0,   0xFF,0,   0 },
	{ 0xFF,0xFF,0,   0 },
	{ 0,   0,   0xFF,0 },
	{ 0xFF,0,   0xFF,0 },
	{ 0,   0xFF,0xFF,0 },
	{ 0xFF,0xFF,0xFF,0 }
};

void DefaultPaletteEntries(LOGPALETTE  *pPal)
{
	ASSERT(pPal);

	for (int i = 1 ; i <= 12 ; i++)
		pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
}

COLORREF BlendColors(COLORREF colorInput, COLORREF colorUnchanged, COLORREF colorOldPure, COLORREF colorNewPure)
{
	int redInput, greenInput, blueInput, 
		redUnchanged, greenUnchanged, blueUnchanged, 
		redOldPure, greenOldPure, blueOldPure, 
		redNewPure, greenNewPure, blueNewPure, 
		redOutput, greenOutput, blueOutput;
	double redLambda, greenLambda, blueLambda;

	redInput = GetRValue(colorInput);
	greenInput = GetGValue(colorInput);
	blueInput = GetBValue(colorInput);

	redUnchanged = GetRValue(colorUnchanged);
	greenUnchanged = GetGValue(colorUnchanged);
	blueUnchanged = GetBValue(colorUnchanged);

	redNewPure = GetRValue(colorNewPure);
	greenNewPure = GetGValue(colorNewPure);
	blueNewPure = GetBValue(colorNewPure);

	redOldPure = GetRValue(colorOldPure);
	greenOldPure = GetGValue(colorOldPure);
	blueOldPure = GetBValue(colorOldPure);

	if (redOldPure != redUnchanged)
	{
		redLambda = double(redInput-redUnchanged)/double(redOldPure-redUnchanged);
		redOutput = (int)round(redUnchanged + redLambda*(redNewPure-redUnchanged));
		if (redOutput < 0)
			redOutput = 0;
		else if (redOutput > 255)
			redOutput = 255;
	}
	else
	{
		redOutput = redOldPure;
	}

	if (greenOldPure != greenUnchanged)
	{
		greenLambda = double(greenInput-greenUnchanged)/double(greenOldPure-greenUnchanged);
		greenOutput = (int)round(greenUnchanged + greenLambda*(greenNewPure-greenUnchanged));
		if (greenOutput < 0)
			greenOutput = 0;
		else if (greenOutput > 255)
			greenOutput = 255;
	}
	else
	{
		greenOutput = greenOldPure;
	}

	if (blueOldPure != blueUnchanged)
	{
		blueLambda = double(blueInput-blueUnchanged)/double(blueOldPure-blueUnchanged);
		blueOutput = (int)round(blueUnchanged + blueLambda*(blueNewPure-blueUnchanged));
		if (blueOutput < 0)
			blueOutput = 0;
		else if (blueOutput > 255)
			blueOutput = 255;
	}
	else
	{
		blueOutput = blueOldPure;
	}

	return RGB(redOutput,greenOutput,blueOutput);
}

LRESULT PostMessageTimeout(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, 
						   UINT fuFlags, UINT uTimeout, LPDWORD lpdwResult, 
						   BOOL bRetry)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	LRESULT ret;

	if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)) && pApp->m_nOsRunning >= OSWinNT)
	{
		ret = SendMessageTimeout(hWnd, Msg, wParam, lParam, fuFlags, uTimeout, lpdwResult);
		if (bRetry && ret == 0 && GetLastError() == ERROR_TIMEOUT)
		{
			// retry it
			ret = PostMessage(hWnd, Msg, wParam, lParam);
		}
	}
	else
	{
		// It seems that Win95/Win98 returns GetLastError() == ERROR_NO_ERROR after timeout.
		// so we ignore SendMessageTimeout in this case.
		ret = PostMessage(hWnd, Msg, wParam, lParam);
	}
	return ret;
}

OSRUNNING GetOSRunning()
{
    OSVERSIONINFO OSInfo;
    ZeroMemory(&OSInfo, sizeof(OSVERSIONINFO));
    OSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&OSInfo);

    OSRUNNING retOS = OSUnknown;
	if (OSInfo.dwPlatformId == VER_PLATFORM_WIN32s)
		retOS = OSWin3;
    else if ((OSInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (OSInfo.dwMajorVersion >= 4))
        retOS = ((OSInfo.dwMajorVersion > 4 || (OSInfo.dwMajorVersion == 4 && OSInfo.dwMinorVersion > 0)) ? OSWin98 : OSWin95);
    else if (OSInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
        retOS = ((OSInfo.dwMajorVersion <= 4) ? OSWinNT : OSWin2K);

    return retOS;
}
