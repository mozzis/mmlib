/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: September 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlDocument.cpp: implementation of class CGlDocument
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "GlWinApp.h"
#include "GlDocument.h"
#include "GlView.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlDocument

IMPLEMENT_DYNAMIC(CGlDocument, CDocument)

BEGIN_MESSAGE_MAP(CGlDocument, CDocument)
	//{{AFX_MSG_MAP(CGlDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlDocument construction/destruction

CGlDocument::CGlDocument()
{
}

CGlDocument::~CGlDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGlDocument Message handlers

int CGlDocument::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CWnd* pMainFrame = AfxGetMainWnd();
	return pMainFrame->MessageBox(lpszText, lpszCaption, nType);
}

void CGlDocument::AddStarToTitle()
{
	CString mTitle = GetTitle();
	if (mTitle.GetLength() > 0)
		if (mTitle.Right(1) != CString("*"))
		{
			mTitle += "*";
			SetTitle(mTitle);
		}
}

void CGlDocument::RemoveStarFromTitle()
{
	CString mTitle = GetTitle();
	if (mTitle.GetLength() > 0)
		if (mTitle.Right(1) == CString("*"))
		{
			mTitle = mTitle.Left(mTitle.GetLength()-1);
			SetTitle(mTitle);
		}
}

/////////////////////////////////////////////////////////////////////////////
// Dokument öffnen/schließen
/////////////////////////////////////////////////////////////////////////////
BOOL CGlDocument::SaveModified()
{
	BOOL ret;
	if (IsModified() == TRUE)
		RemoveStarFromTitle();

	ret = CDocument::SaveModified();
	if (IsModified() == TRUE)
		AddStarToTitle();

	return ret;
}

BOOL CGlDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	BOOL ret;
	if (IsModified() == TRUE)
		RemoveStarFromTitle();

	ret = CDocument::DoSave(lpszPathName, bReplace);
	if (IsModified() == TRUE)
		AddStarToTitle();

	return ret;
}

void CGlDocument::SetModifiedFlag(BOOL bModified)
{
	if (bModified == TRUE && IsModified() == FALSE)
		AddStarToTitle();
	else if (bModified == FALSE && IsModified() == TRUE)
		RemoveStarFromTitle();

	CDocument::SetModifiedFlag(bModified);
}

void CGlDocument::OnPrint(CDC* pDC, CPrintInfo* pInfo, CGlView* pCallingView) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	SIZE	res = {0,0};     // Device resolution
	RECT	rect = {0,0,0,0};		// Page rectangle
	HBRUSH	brush;			// Background brush for page
	POINT	PageSize;		  // Size of printed image
	POINT	ImageOffset;	// Offset from edges for image
	int		nXRes, nYRes;
	HDC		hDC, hMemDC, hTmpDC;
	HGLRC	hRC, hMemRC;

	BITMAPINFO	bitmapInfo;
	HBITMAP		hDib;
	LPVOID		pBitmapBits;
	double		fac = 1.;		// try <printer device> DPI

	// Set our device context mapping mode.
	// Each logical unit is converted to 1 device pixel. Positive x is to
	// the right; positive y is down.
	pDC->SetMapMode(MM_TEXT);

	// Get the size of the page (in printer device points)
	rect.top	= 0;
	rect.left   = 0;

	if (pInfo->m_bPreview)
	{
		PRINTDLG PrtDlg;	// Printer dialog

		if (!pApp->GetPrinterDeviceDefaults(&PrtDlg))
		{
			TRACE("No default printer.\n");
			// Just create a display device context.
			// Note that the DIB image will NOT be 
			// positioned properly, but it will at least
			// be displayed.

			if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC))) 
			{
				CPreviewDC *pPrevDC = (CPreviewDC *)pDC;
				//m_hAttribDC hold the print/fax DC so...
				rect.right =  GetDeviceCaps(pPrevDC->m_hAttribDC, HORZRES);
				rect.bottom = GetDeviceCaps(pPrevDC->m_hAttribDC, VERTRES);
				res.cx = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSX);
				res.cy = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSY);
			}
		}
		else
		{
			// Don't display the dialog.  We create the CPrintDialog so that
			// we can create a printer device context.
			CPrintDialog dlg(FALSE);

			dlg.m_pd.hDevMode  = PrtDlg.hDevMode;
			dlg.m_pd.hDevNames = PrtDlg.hDevNames;

			HDC hdc = dlg.CreatePrinterDC();

			// Get the size of the default printer page
			rect.right  = GetDeviceCaps(hdc, HORZRES);
			rect.bottom = GetDeviceCaps(hdc, VERTRES);

			// Get the resolution of the printer device
			res.cx = GetDeviceCaps(hdc, LOGPIXELSX);
			res.cy = GetDeviceCaps(hdc, LOGPIXELSY);

			// Free the device context
			DeleteDC(hdc);
		}
	}
	else
	{
		// Get the size of the actual printed page
		rect.right  = GetDeviceCaps(pDC->GetSafeHdc(), HORZRES);
		rect.bottom = GetDeviceCaps(pDC->GetSafeHdc(), VERTRES);

		// Get the resolution of the printer device
		res.cx = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSX);
		res.cy = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
	}
	nXRes = rect.right-rect.left;
	nYRes = rect.bottom-rect.top;

	// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
	while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024) 
	{
		nXRes /= 2;
		nYRes /= 2;
		fac /= 2.;
	}

	// arrange views
	CView *pView;
	POSITION pos;
	double printerAspect = (nYRes == 0) ? (double)nXRes : (double)nXRes/(double)nYRes;
	double averageAspect = 0.;

	// determine number of views to be printed
	int nNumberOfViewsToPrint = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			nNumberOfViewsToPrint++;
			averageAspect += ((CGlView*)pView)->m_aspect;

			// check if pView is in a hidden row/column of an extended splitter view
			CWnd* pParent = pView->GetParent();
			if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
			{
				int nRow, nCol;

				int nID = pView->GetDlgCtrlID();
				ASSERT(nID >= AFX_IDW_PANE_FIRST && nID <= AFX_IDW_PANE_LAST);

				nRow = (nID - AFX_IDW_PANE_FIRST) / 16;
				nCol = (nID - AFX_IDW_PANE_FIRST) % 16;
				if (nRow >= ((CSplitterWndEx*)pParent)->GetRowCount() || 
					nCol >= ((CSplitterWndEx*)pParent)->GetColumnCount())		// row/column is hidden
				{
					nNumberOfViewsToPrint--;
					averageAspect -= ((CGlView*)pView)->m_aspect;
				}
			}
		}
	}
	ASSERT(nNumberOfViewsToPrint > 0);
	averageAspect /= (double)nNumberOfViewsToPrint;
	ASSERT(averageAspect > 0.);
	ASSERT(printerAspect > 0.);
	if (nNumberOfViewsToPrint == 0 || averageAspect == 0. || printerAspect == 0.)
		return;

	// determine how many rows/columns are needed to arrange views optimal
	// nX - number of columns
	// nY - number of rows
	int nX, nY, nX1, nY1, nX2, nY2;
	nX1 = nX = (int)ceil(sqrt(nNumberOfViewsToPrint*printerAspect/averageAspect));
	nY2 = nY = (int)ceil(sqrt(nNumberOfViewsToPrint*averageAspect/printerAspect));
	nY1 = (int)ceil(double(nNumberOfViewsToPrint)/double(nX1));
	nX2 = (int)ceil(double(nNumberOfViewsToPrint)/double(nY2));
	if (nX1*nY1 < nX2*nY2)
	{
		nX = nX1;
		nY = nY1;
	}
	else
	{
		nX = nX2;
		nY = nY2;
	}

	// how does a view with averageAspect fill it's area?
	// (cut height if nessesary)
	int nXViewRes = nXRes/nX, nYViewRes = nYRes/nY;
	if (nXViewRes/nYViewRes < averageAspect)	// average view aspect is larger than reserved place, i.e. view is too wide
		nYViewRes = int(nYViewRes/averageAspect);

	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			((CGlView*)pView)->ScaleFont(double(nYViewRes)/double(pCallingView->m_DocPrintHeight));
		}
	}

	// First of all, initialize the bitmap header information...
	memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth		= nXRes;
	bitmapInfo.bmiHeader.biHeight		= nYRes;
	bitmapInfo.bmiHeader.biPlanes		= 1;
	bitmapInfo.bmiHeader.biBitCount		= 24;
	bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(2840./72.*res.cx*fac);	// res.cx*fac DPI
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(2840./72.*res.cy*fac);	// res.cy*fac DPI

	// create DIB
	hTmpDC = ::GetDC(pCallingView->m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(pCallingView->m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(pCallingView->m_pDC == NULL ? NULL : pCallingView->m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		return;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!pCallingView->SetMemDcPixelFormat(hMemDC))
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// create memory rendering context
	if ((hMemRC = wglCreateContext(hMemDC)) == NULL)
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// Store current rendering and device contexts
	pCallingView->GetCurrent(hDC, hRC);		

	// Make this hMemRC the current OpenGL rendering context.
	pCallingView->SetCurrent(hMemDC, hMemRC);

	pCallingView->SetOpenGLProperties();

	// must be done here, not in pView->OnDraw()
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// must be created once for hMemDC
	pCallingView->CreateFontBitmaps();

	// draw (loop over all views to be printed)
	BOOL bPrint;
	int i = -1;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			bPrint = TRUE;
			i++;

			// check if pView is in a hidden row/column of an extended splitter view
			CWnd* pParent = pView->GetParent();
			if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
			{
				int nRow, nCol;

				int nID = pView->GetDlgCtrlID();
				ASSERT(nID >= AFX_IDW_PANE_FIRST && nID <= AFX_IDW_PANE_LAST);

				nRow = (nID - AFX_IDW_PANE_FIRST) / 16;
				nCol = (nID - AFX_IDW_PANE_FIRST) % 16;
				if (nRow >= ((CSplitterWndEx*)pParent)->GetRowCount() || 
					nCol >= ((CSplitterWndEx*)pParent)->GetColumnCount())		// row/column is hidden
				{
					bPrint = FALSE;
					i--;
				}
			}

			if (bPrint)
			{
				nXViewRes = int(nYViewRes*((CGlView*)pView)->m_aspect);

				glViewport((nXRes/nX - nXViewRes)/2 + (i/nY) * (nXRes/nX), (nYRes/nY - nYViewRes)/2 + (nY-1-i%nY) * (nYRes/nY), nXViewRes, nYViewRes);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glDrawBuffer(GL_BACK);
				glEnable(GL_DEPTH_TEST);

				((CGlView*)pView)->m_bIsDocPrinting = TRUE;
				((CGlView*)pView)->OnDraw(pDC);
				((CGlView*)pView)->m_bIsDocPrinting = FALSE;
			}
		}
	}
	glFinish();	// Finish all OpenGL commands

	// the rendering context will be no longer needed
	pCallingView->SetCurrent(NULL, NULL);	
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pCallingView->SetCurrent(hDC, hRC);	

	// Restore the Views original font sizes
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			((CGlView*)pView)->UnScaleFont();
			((CGlView*)pView)->CreateFontBitmaps();
		}
	}

	// Get the size of our page and clear to White
	brush	   = CreateSolidBrush(0x00ffffff);	// White brush
	FillRect(pDC->GetSafeHdc(), &rect, brush);	// Clear the page

	PageSize.x = rect.right;
	PageSize.y = PageSize.x * bitmapInfo.bmiHeader.biHeight / bitmapInfo.bmiHeader.biWidth;
	if (PageSize.y > rect.bottom)
	{
		PageSize.y = rect.bottom;
		PageSize.x = PageSize.y * bitmapInfo.bmiHeader.biWidth / bitmapInfo.bmiHeader.biHeight;
	}
	ImageOffset.x = (rect.right - PageSize.x) / 2;
	ImageOffset.y = (rect.bottom - PageSize.y) / 2;

	// Does our device context support StretchDIBits?
	if (pCallingView->m_bStretchDIBits)
	{
		StretchDIBits(pDC->GetSafeHdc(),
		  ImageOffset.x, ImageOffset.y,
		  PageSize.x, PageSize.y,
		  0, 0,
		  bitmapInfo.bmiHeader.biWidth,
		  bitmapInfo.bmiHeader.biHeight,
		  (GLubyte*)pBitmapBits,
		  &bitmapInfo,
		  DIB_RGB_COLORS,
		  SRCCOPY);
	}
	else if (pCallingView->m_bStretchBlt)
	{
		HDC hdc;
		HBITMAP bitmap;

		hdc	= CreateCompatibleDC(pDC->GetSafeHdc());
		bitmap = CreateDIBitmap(hdc,
								&(bitmapInfo.bmiHeader),
								CBM_INIT,
								(GLubyte*)pBitmapBits,
								&bitmapInfo,
								DIB_RGB_COLORS);
		
		HGDIOBJ oldbitmap = SelectObject(hdc, bitmap);
		StretchBlt(pDC->GetSafeHdc(),
		 ImageOffset.x, ImageOffset.y,
		 PageSize.x, PageSize.y,
		 hdc,
		 0, 0,
		 bitmapInfo.bmiHeader.biWidth,
		 bitmapInfo.bmiHeader.biHeight,
		 SRCCOPY);
		if (oldbitmap != NULL)
			SelectObject(hdc, oldbitmap);

		// Free our bitmap and bitmap device context
		DeleteObject(bitmap);
		DeleteDC(hdc);
	}
	if (hOldDib != NULL)
		SelectObject(hMemDC, hOldDib);

	// Delete our brush, DIB and device context
	DeleteObject(brush);
	DeleteObject(hDib);
	DeleteDC(hMemDC);
}
