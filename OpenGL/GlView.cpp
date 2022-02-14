/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
// special thanks for the printing routines to
// Craig Fahrnbach <craignan@home.com>
//
// and for WINDOW class registering to
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// GlView.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "GlWinApp.h"
#include "GlView.h"
#include "GlDocument.h"
#include "Helper.h"
#include "DlgOpenglInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlView

IMPLEMENT_DYNAMIC(CGlView, CView)

BEGIN_MESSAGE_MAP(CGlView, CView)
	//{{AFX_MSG_MAP(CGlView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_OPTIONS_FONT, OnOptionsFont)
	ON_COMMAND(ID_HELP_OPENGL_INFO, OnHelpOpenglInfo)
	ON_WM_CREATE()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlView construction/destruction

CGlView::CGlView()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	// OpenGL
	m_pDC = NULL;
	m_hRC = NULL;
	m_nBitsPerPixel = 0;
	m_aspect = 1.0;

	m_nListBaseSmall = 1000;	
	m_nListBaseNormal = 2000;	

	// Mouse
  m_hRotationCursor = pApp->LoadCursor(IDC_CURROTATE);
//	m_hRotationCursor = LoadCursor(NULL, IDC_CROSS);

	// Colors
	m_ClearColorRed   = (float)GetRValue(pApp->m_OptionColorGlBack) / 255.0f;
	m_ClearColorGreen = (float)GetGValue(pApp->m_OptionColorGlBack) / 255.0f;
	m_ClearColorBlue  = (float)GetBValue(pApp->m_OptionColorGlBack) / 255.0f;

	m_ClearColorRedTmp   = 1.0f;
	m_ClearColorGreenTmp = 1.0f;
	m_ClearColorBlueTmp  = 1.0f;

	// Set Light0
	m_AmbientColorRed   = (float)GetRValue(pApp->m_OptionColorGlLightAmbient) / 255.0f;
	m_AmbientColorGreen = (float)GetGValue(pApp->m_OptionColorGlLightAmbient) / 255.0f;
	m_AmbientColorBlue  = (float)GetBValue(pApp->m_OptionColorGlLightAmbient) / 255.0f;

	m_DiffuseColorRed   = (float)GetRValue(pApp->m_OptionColorGlLightDiffuse) / 255.0f;
	m_DiffuseColorGreen = (float)GetGValue(pApp->m_OptionColorGlLightDiffuse) / 255.0f;
	m_DiffuseColorBlue  = (float)GetBValue(pApp->m_OptionColorGlLightDiffuse) / 255.0f;

	m_SpecularColorRed   = (float)GetRValue(pApp->m_OptionColorGlLightSpecular) / 255.0f;
	m_SpecularColorGreen = (float)GetGValue(pApp->m_OptionColorGlLightSpecular) / 255.0f;
	m_SpecularColorBlue  = (float)GetBValue(pApp->m_OptionColorGlLightSpecular) / 255.0f;

	m_TextColorRed   = (float)GetRValue(pApp->m_OptionColorText) / 255.0f;
	m_TextColorGreen = (float)GetGValue(pApp->m_OptionColorText) / 255.0f;
	m_TextColorBlue  = (float)GetBValue(pApp->m_OptionColorText) / 255.0f;

	m_TextColorRedTmp   = 1.0f;
	m_TextColorGreenTmp = 1.0f;
	m_TextColorBlueTmp  = 1.0f;

	m_OptionColorText = pApp->m_OptionColorText;	
	m_bFontCreateFlagSmall = FALSE;
	m_bFontCreateFlagNormal = FALSE;
	m_dFontRatio = .5;

	// Initialize zoom
	m_bLastActivateWasDoneByLButton = FALSE;
	m_bFirstPickPoint = FALSE;
	m_bSecondPickPoint = FALSE;

	m_StartPoint.SetSize(2);
	m_EndPoint.SetSize(2);
	m_StartPoint[0] = m_StartPoint[1] = 0.0;
	m_EndPoint[0] = m_EndPoint[1] = 0.0;

	m_xMin = -1.;
	m_yMin = -1.;
	m_xMax = 1.;
	m_yMax = 1.;

	// Initialize rendering
	m_XRenderArray.SetSize(0);
	m_YRenderArray.SetSize(0);
	m_ZRenderArray.SetSize(0);
	m_CRenderArray.SetSize(0);
	m_SRenderArray.SetSize(0);

	// Initialize printing
	m_bStretchDIBits = FALSE;
	m_bStretchBlt = FALSE;
#if _MSC_VER < 1200
	m_bMDIMaximizeAndRestoreInPreview = FALSE;
#endif // _MSC_VER < 1200

	m_bIsDocPrinting = FALSE;
	m_bBlackLinesOrRemoveHidden = pApp->m_bBlackLinesOrRemoveHidden;
	m_bScaleShowFrame = pApp->m_bScaleShowFrame;
	m_bScaleShowLabels = pApp->m_bScaleShowLabels;
	m_bScaleShowGrid = pApp->m_bScaleShowGrid;
	m_bScaleShowAxis = pApp->m_bScaleShowAxis;
	m_DocPrintHeight = 0;
	m_dInvertNonRainbowColors = 1.;
}

CGlView::~CGlView()
{
}

void CGlView::OnDraw(CDC*)
{
}

void CGlView::InitGeometry()
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = 0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_xRotationStore = m_xRotation;
	m_yRotationStore = m_yRotation;

	m_ZoomHistory.SetSize(0);
}

BOOL CGlView::PreCreateWindow(CREATESTRUCT& cs)
{
// these styles are requested by OpenGL
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);			 
 
// call base class PreCreateWindow to get the cs.lpszClass filled in with the MFC default class name
	if (!CView::PreCreateWindow(cs))
		return FALSE;

// Register the window class if it has not already been registered.
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
// this is the new registered window class
#define CUSTOM_CLASSNAME _T("GL_VIEW_CLASS")
// check if our class has been already registered (typical in MDI environment)
	if (!(::GetClassInfo(hInst, CUSTOM_CLASSNAME, &wndcls)))
	{
// get default MFC class settings
		if (::GetClassInfo(hInst, cs.lpszClass, &wndcls))
		{
// set our class name
			wndcls.lpszClassName = CUSTOM_CLASSNAME;
// these styles are set for GL to work in MDI
			wndcls.style |= (CS_OWNDC | CS_HREDRAW | CS_VREDRAW);
			wndcls.hbrBackground = NULL;
// try to register class (else throw exception)
			if (!AfxRegisterClass(&wndcls)) 
				AfxThrowResourceException();
		}
// default MFC class not registered
		else 
			AfxThrowResourceException();
	}
// set our class name in CREATESTRUCT
	cs.lpszClass = CUSTOM_CLASSNAME;
// we're all set
	return TRUE;
}

void CGlView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		MakeActive();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGlView printing

BOOL CGlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// set the page count if not previously set
	if (pInfo->GetMaxPage() == 0xFFFF)
		pInfo->SetMaxPage(pInfo->GetMinPage());

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGlView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->BeginPrinting(pDC);
		}
	}

	// Note: This value will only be used in CGlDocument::OnPrint
	CRect	mRect;			// Position of this window's client area
	// Get the Views size (client area!)
	GetClientRect(&mRect);
	m_DocPrintHeight = mRect.Height();

	// Gather some general information about the printer the
	// user selected..
	LPDEVMODE* pMode = (LPDEVMODE*) pInfo->m_pPD->m_pd.hDevMode;
	DEVMODE* mode = *pMode;
	CString mMsg;
	TCHAR* pChar = (TCHAR*)&mode->dmDeviceName[0];
	
	int nBitsPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nColors		= pDC->GetDeviceCaps(NUMCOLORS);
	int nBitPlanes	= pDC->GetDeviceCaps(PLANES);
	int nColorRes	= pDC->GetDeviceCaps(COLORRES);
	int nRasterCaps	= pDC->GetDeviceCaps(RASTERCAPS);

	// Does the device support raster operations?
	if (nRasterCaps & RC_STRETCHDIB)
		m_bStretchDIBits = TRUE;
	else 
		m_bStretchDIBits = FALSE;

	if (nRasterCaps & RC_STRETCHBLT)
		m_bStretchBlt = TRUE;
	else 
		m_bStretchBlt = FALSE;

	TRACE("Your selected printer '%s'\n", pChar);
	TRACE("BitsPerPixel           = %d\n", nBitsPerPixel);
	TRACE("NumColor               = %d\n", nColors);
	TRACE("NumBitPlanes           = %d\n", nBitPlanes);
	TRACE("ColorResolution        = %d\n", nColorRes);	
	TRACE("Supports StretchBlt    = %d\n", m_bStretchBlt);
	TRACE("Supports StretchDIBits = %d\n", m_bStretchDIBits);

	PrepareBkgnd(TRUE);

	if (!m_bStretchDIBits && !m_bStretchBlt)
	{
		mMsg.Format(IDS_PRINTERGDI_ERROR, pChar);
		AfxMessageBox(mMsg, MB_OK | MB_ICONERROR);
	}
#if _MSC_VER < 1200
	else
	{
		CFrameWnd *pFrame = GetParentFrame();

		// in case of previewing the whole document 
		// in a MDI child window which is not maximized or minimized
		// maximize it and restore it at the end of printing
		if (pApp->m_nPrintObject == 1
			&& pInfo->m_bPreview
			&& pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))
			&& !pFrame->IsIconic()
			&& !pFrame->IsZoomed())
		{
			m_bMDIMaximizeAndRestoreInPreview = TRUE;
			((CMDIChildWnd*)pFrame)->MDIMaximize();
		}
	}
#endif // _MSC_VER < 1200
}

void CGlView::OnEndPrinting(CDC* pDC, CPrintInfo*) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->EndPrinting(pDC);
		}
	}

#if _MSC_VER < 1200
	// restore MDI child frame in case it was maximized
	if (m_bMDIMaximizeAndRestoreInPreview)
	{
		m_bMDIMaximizeAndRestoreInPreview = FALSE;
		((CMDIChildWnd*)GetParentFrame())->MDIRestore();
	}
#endif // _MSC_VER < 1200

	PrepareBkgnd(FALSE);

	// Note: This value will only be used in CGlDocument::OnPrint
	m_DocPrintHeight = 0;
}

void CGlView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	if (pApp->m_nPrintObject == 1)
	{
		CDocument* pDoc = GetDocument();
		if (pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
		{
			// let the document do the job
			((CGlDocument*)pDoc)->OnPrint(pDC, pInfo, this);
			return;
		}
	}
	else if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->Print(pDC);
			return;
		}
	}

	SIZE	res = {0,0};	// Device resolution
	RECT	rect;			    // Page rectangle
	HBRUSH	brush;			// Background brush for page
	POINT	PageSize;		// Size of printed image
	POINT	ImageOffset;	// Offset from edges for image
	CRect	mRect;			// Position of this window's client area
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

	// Get the Views size (client area!)
	GetClientRect(&mRect);
  rect = mRect;
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

	// Determine whether the Views aspect ratio is bigger or smaller than the printers one
	if (mRect.Width()*(rect.bottom-rect.top) > mRect.Height()*(rect.right-rect.left))
	{
		// View area is wider (in x) than Printer area
		nXRes = rect.right-rect.left;
		nYRes = nXRes*mRect.Height()/mRect.Width();

		// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
		while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024) 
		{
			nXRes /= 2;
			nYRes /= 2;
			fac /= 2.;
		}
		ScaleFont(double(nXRes)/double(mRect.Width()));
	}
	else
	{
		// Printer area is wider (in x) than View area
		nYRes = rect.bottom-rect.top;
		nXRes = nYRes*mRect.Width()/mRect.Height();

		// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
		while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024)
		{
			nXRes /= 2;
			nYRes /= 2;
			fac /= 2.;
		}
		ScaleFont(double(nYRes)/double(mRect.Height()));
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
	hTmpDC = ::GetDC(m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(m_pDC == NULL ? NULL : m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		return;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!SetMemDcPixelFormat(hMemDC))
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
	GetCurrent(hDC, hRC);		

	// Make this hMemRC the current OpenGL rendering context.
	SetCurrent(hMemDC, hMemRC);

	SetOpenGLProperties();

	glViewport(0, 0, nXRes, nYRes);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// must be created once for hMemDC
	CreateFontBitmaps();

	OnDraw(pDC);
	glFinish();	// Finish all OpenGL commands

	// the rendering context will be no longer needed
	SetCurrent(NULL, NULL);	
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);	

	// Restore the Views original font size
	UnScaleFont();
	CreateFontBitmaps();

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
	if (m_bStretchDIBits)
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
	else if (m_bStretchBlt)
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

/////////////////////////////////////////////////////////////////////////////
// Opengl specific

BOOL CGlView::InitializeOpenGL()
{
	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
		m_nBitsPerPixel = 0;
	}

	m_pDC = new CWindowDC(this);

	if (m_pDC == NULL) // failure to get DC
		return FALSE;

	m_nBitsPerPixel = m_pDC->GetDeviceCaps(BITSPIXEL);

	if (SetWindowPixelFormat() == FALSE)
		return FALSE;

	CreateRGBPalette();

	if (CreateViewGLContext() == FALSE)
		return FALSE;

	SetOpenGLProperties();
	CreateFontBitmaps();
	return TRUE;
}

void CGlView::CreateRGBPalette()
{
	if (m_Palette.m_hObject != NULL)
	{
		m_pDC->SelectPalette(&m_Palette, FALSE);
		m_pDC->RealizePalette();
		return;
	}

	PIXELFORMATDESCRIPTOR pixelDesc;
	LOGPALETTE  *pPal;
	WORD		n, i;
	int			nGLPixelIndex;

	nGLPixelIndex = GetPixelFormat(m_pDC->GetSafeHdc());
	DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc);

	if (pixelDesc.dwFlags & PFD_NEED_PALETTE)
	{
		n = (WORD)(1 << pixelDesc.cColorBits);

		pPal = (PLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];
		ASSERT(pPal != NULL);

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;

		for (i = 0; i < n; i++)
		{
			pPal->palPalEntry[i].peRed = ComponentFromIndex(i, pixelDesc.cRedBits, pixelDesc.cRedShift);
			pPal->palPalEntry[i].peGreen = ComponentFromIndex(i, pixelDesc.cGreenBits, pixelDesc.cGreenShift);
			pPal->palPalEntry[i].peBlue = ComponentFromIndex(i, pixelDesc.cBlueBits, pixelDesc.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		// fix up the palette to include the default GDI palette
		if ((pixelDesc.cColorBits == 8)									&&
			(pixelDesc.cRedBits   == 3) && (pixelDesc.cRedShift   == 0) &&
			(pixelDesc.cGreenBits == 3) && (pixelDesc.cGreenShift == 3) &&
			(pixelDesc.cBlueBits  == 2) && (pixelDesc.cBlueShift  == 6))
		{
			DefaultPaletteEntries(pPal);
		}

		m_Palette.CreatePalette(pPal);
		delete [] pPal;

		m_pDC->SelectPalette(&m_Palette, FALSE);
		m_pDC->RealizePalette();
	}
}

void CGlView::SetOpenGLProperties()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	// Default mode
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	specularProperties[] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularProperties);

	SetBackColor();
	glClearDepth(1.0);
	
	GLfloat light0Position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHT0);
	SetAmbientColor();
	SetDiffuseColor();
	SetSpecularColor();
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	// Default antialiasing
	if (pApp->m_bAntialias)
	{
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		if (pApp->m_nPolygonMode == 2)
			glLineWidth(1.0);
		else
			glLineWidth(1.5);
		glPointSize(3.0);
	}
	else
	{
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glLineWidth(1.0);
		glPointSize(2.0);
	}

	// Default smooth
	if (pApp->m_bSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	// Default lighting
	if (pApp->m_bLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	// Default polygonmode
	switch (pApp->m_nPolygonMode)
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		default:
			{}
	}
}

void CGlView::SetBackColor()
{
	glClearColor(m_ClearColorRed, m_ClearColorGreen, m_ClearColorBlue, 1.0f);
}

void CGlView::SetAmbientColor()
{
	GLfloat	ambientLight[]  = {m_AmbientColorRed, m_AmbientColorGreen, m_AmbientColorBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
}

void CGlView::SetDiffuseColor()
{
	GLfloat	diffuseLight[]  = {m_DiffuseColorRed, m_DiffuseColorGreen, m_DiffuseColorBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

void CGlView::SetSpecularColor()
{
	GLfloat	specularLight[] = {m_SpecularColorRed, m_SpecularColorGreen, m_SpecularColorBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

BOOL CGlView::MakeActive()
{
	HDC hDC = m_pDC->GetSafeHdc();
	BOOL bRet = FALSE;
	int i = 0;

	// check if m_hRC is already current
	if (wglGetCurrentContext() == m_hRC)
		return TRUE;

	while ((bRet = SetCurrent(hDC, m_hRC)) == FALSE)
	{
		if (i++ > 100)					//Try it only 100 times
		{
			TRACE("Abort \"MakeActive\" after %d unsuccessful trials\n", i);
			break;
		}
		if (GetLastError() == 2000)		//The pixel format is invalid.
		{
			InitializeOpenGL();
		}
	}
	if (i > 0 && bRet == TRUE)
		TRACE("\"MakeActive\" was successful after %d trials.\n", i);

	return bRet;
}

void CGlView::GetCurrent(HDC& hDC, HGLRC& hRC) 
{
	hDC = wglGetCurrentDC();
	hRC = wglGetCurrentContext(); 
} 

BOOL CGlView::SetCurrent(HDC hDC, HGLRC hRC) 
{
	if (wglMakeCurrent(hDC, hRC) == FALSE)
		return FALSE;
	return TRUE;
} 

/////////////////////////////////////////////////////////////////////////////
// SetPixelFormat

BOOL CGlView::SetWindowPixelFormat()
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	int nGLPixelIndex = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pixelDesc);
	if (nGLPixelIndex == 0) // Choose default
	{
		BOOL bSuccess;

		// first we try to get a double buffered format
		while ((bSuccess = DescribePixelFormat(m_pDC->GetSafeHdc(), ++nGLPixelIndex, 
					sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc)) != FALSE)
		{
			if (pixelDesc.dwFlags & PFD_DOUBLEBUFFER)
				break;
		}

		// now we take even a single buffered format
		if (!bSuccess)
		{
			nGLPixelIndex = 1;
			if (DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, 
				sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
				return FALSE;
		}
	}

	if (!SetPixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, &pixelDesc))
		return FALSE;

	return TRUE;
}

BOOL CGlView::SetMemDcPixelFormat(HDC hMemDC)
{
	int nGLPixelIndex;

	PIXELFORMATDESCRIPTOR pixelMem;
	pixelMem.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelMem.nVersion = 1;
	
	pixelMem.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL |
		PFD_STEREO_DONTCARE;
	
	pixelMem.iPixelType = PFD_TYPE_RGBA;
	pixelMem.cColorBits = 24;
	pixelMem.cRedBits = 8;
	pixelMem.cRedShift = 16;
	pixelMem.cGreenBits = 8;
	pixelMem.cGreenShift = 8;
	pixelMem.cBlueBits = 8;
	pixelMem.cBlueShift = 0;
	pixelMem.cAlphaBits = 0;
	pixelMem.cAlphaShift = 0;
	pixelMem.cAccumBits = 64;
	pixelMem.cAccumRedBits = 16;
	pixelMem.cAccumGreenBits = 16;
	pixelMem.cAccumBlueBits = 16;
	pixelMem.cAccumAlphaBits = 0;
	pixelMem.cDepthBits = 32;
	pixelMem.cStencilBits = 8;
	pixelMem.cAuxBuffers = 0;
	pixelMem.iLayerType = PFD_MAIN_PLANE;
	pixelMem.bReserved = 0;
	pixelMem.dwLayerMask = 0;
	pixelMem.dwVisibleMask = 0;
	pixelMem.dwDamageMask = 0;

	nGLPixelIndex = ChoosePixelFormat(hMemDC, &pixelMem);
	if (nGLPixelIndex == 0) // Choose default
	{
		nGLPixelIndex = 1;
		if (DescribePixelFormat(hMemDC, nGLPixelIndex, 
			sizeof(PIXELFORMATDESCRIPTOR), &pixelMem))
			return FALSE;
	}

	if (!SetPixelFormat(hMemDC, nGLPixelIndex, &pixelMem))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CreateViewGLContext

BOOL CGlView::CreateViewGLContext()
{
	// It may happen that CreateViewGLContext is
	// called recursively:
	//
	// When in MakeActive the SetCurrent failes 
	// then InitializeOpenGL may be called again which
	// calles CreateViewGLContext a second time.
	//
	// To avoid an infinite recursion we check
	// m_hRC. If m_hRC is not NULL, then we know that
	// CreateViewGLContext was called recursively
	// in the way described above.
	// Then we delete the existing rendering context 
	// before creating a new one (to avoid resource leaks)
	// and do NOT call MakeActive again.

	BOOL bRecursivelyCalled = (m_hRC != NULL);
	if (bRecursivelyCalled)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());

	if (m_hRC == NULL)
		return FALSE;

	if (bRecursivelyCalled)
		return TRUE;

	return MakeActive();
}

void CGlView::AddToZoomHistory()
{
	CMatrix tmpMatrix;
	tmpMatrix.SetSize(4);
	tmpMatrix[0] = m_xScaling;
	tmpMatrix[1] = m_yScaling;
	tmpMatrix[2] = m_xTranslation;
	tmpMatrix[3] = m_yTranslation;

	m_ZoomHistory.Add(tmpMatrix);
}

BOOL CGlView::GetFromZoomHistory()
{
	if (m_ZoomHistory.GetSize() == 0)
		return FALSE;	// Nothing to do

	int tmpUpperBound = m_ZoomHistory.GetUpperBound();
	CMatrix tmpMatrix(m_ZoomHistory.GetAt(tmpUpperBound));
	m_ZoomHistory.RemoveAt(tmpUpperBound);
	m_ZoomHistory.SetSize(tmpUpperBound);

	m_xScaling = (float)tmpMatrix[0];
	m_yScaling = (float)tmpMatrix[1];
	m_xTranslation = (float)tmpMatrix[2];
	m_yTranslation = (float)tmpMatrix[3];

	InvalidateRect(NULL, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Drawing stuff
/////////////////////////////////////////////////////////////////////////////
void CGlView::OnEditCopy()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CRect	mRect;			// Position of this window's client area
	HDC		hDC, hMemDC, hTmpDC;
	HGLRC	hRC, hMemRC;

	BITMAPINFO	bitmapInfo;
	HBITMAP		hDib;
	LPVOID		pBitmapBits;
	double		fac = 4.;		// try 72 DPI (screen) --> 288 DPI (clipboard)

	BeginWaitCursor();

	// Get the Views size (client area!)
	GetClientRect(&mRect);
	mRect.right *= (int)fac;
	mRect.bottom *= (int)fac;

	// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
	while (3*mRect.right*mRect.bottom > (pApp->m_nMaxPrintMemSize)*1024)
	{
		mRect.right /= 2;
		mRect.bottom /= 2;
		fac /= 2.;
	}

	mRect.right = (mRect.right + (sizeof(DWORD)-1))& ~(sizeof(DWORD)-1);	// aligning width to 4 bytes (sizeof(DWORD)) avoids 
																			// pixel garbage at the upper line
	ScaleFont(fac);

	// First of all, initialize the bitmap header information...
	memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth		= mRect.Width();
	bitmapInfo.bmiHeader.biHeight		= mRect.Height();
	bitmapInfo.bmiHeader.biPlanes		= 1;
	bitmapInfo.bmiHeader.biBitCount		= 24;
	bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(2840.*fac);	// 72*fac DPI
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(2840.*fac);	// 72*fac DPI

	// create DIB
	hTmpDC = ::GetDC(m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(m_pDC == NULL ? NULL : m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		EndWaitCursor();
		return;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!SetMemDcPixelFormat(hMemDC))
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		EndWaitCursor();
		return;
	}

	// create memory rendering context
	if ((hMemRC = wglCreateContext(hMemDC)) == NULL)
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		EndWaitCursor();
		return;
	}

	// Store current rendering and device contexts
	GetCurrent(hDC, hRC);		

	// Make this hMemRC the current OpenGL rendering context.
	SetCurrent(hMemDC, hMemRC);

	SetOpenGLProperties();

	glViewport(0, 0, mRect.Width(), mRect.Height());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// must be created once for hMemDC
	CreateFontBitmaps();

	CDC dcDummy;
	dcDummy.m_bPrinting = TRUE;		// avoid that OnDraw swaps buffers
	OnDraw(&dcDummy);
	glFinish();	// Finish all OpenGL commands

	// Restore the Views original font size
	UnScaleFont();
	CreateFontBitmaps();

	// the rendering context will be no longer needed
	SetCurrent(NULL, NULL);	
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);	

	if (OpenClipboard())
	{
		HGLOBAL hClipboardCopy = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(BITMAPINFOHEADER) + bitmapInfo.bmiHeader.biSizeImage);
		if (hClipboardCopy != NULL)
		{
			LPVOID lpClipboardCopy, lpBitmapBitsOffset;
			lpClipboardCopy = ::GlobalLock((HGLOBAL) hClipboardCopy);
			lpBitmapBitsOffset = (LPVOID)((BYTE*)lpClipboardCopy + sizeof(BITMAPINFOHEADER));

			memcpy(lpClipboardCopy, &bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
			memcpy(lpBitmapBitsOffset, pBitmapBits, bitmapInfo.bmiHeader.biSizeImage);
			::GlobalUnlock(hClipboardCopy);

			EmptyClipboard();
			SetClipboardData(CF_DIB, hClipboardCopy);
			CloseClipboard();
		}
	}
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);

	// Delete our DIB and device context
	DeleteObject(hDib);
	DeleteDC(hMemDC);

	EndWaitCursor();
}

void CGlView::OnOptionsFont()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	LOGFONT logFont = m_LogFont;
	CFontDialog dlg(&logFont);
	dlg.m_cf.rgbColors = m_OptionColorText;
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorText = dlg.GetColor();
		pApp->m_LogFont = logFont;
		if (pApp->m_bPropPolicy)
		{
			CDocTemplate* pDocTemplate = GetDocument()->GetDocTemplate();
			if (pDocTemplate)
			{
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							((CGlView *)pView)->OptionsFont();
						}
					}
				}
			}
		}
		else
			OptionsFont();
	}
}

void CGlView::OptionsFont()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	HDC hDC;
	HGLRC hRC;
	HGDIOBJ hOldObject;
	LOGFONT logFont;

	m_OptionColorText = pApp->m_OptionColorText;
	m_TextColorRed   = (float)GetRValue(m_OptionColorText) / 255.0f;
	m_TextColorGreen = (float)GetGValue(m_OptionColorText) / 255.0f;
	m_TextColorBlue  = (float)GetBValue(m_OptionColorText) / 255.0f;
	CheckTextColor();

	logFont = m_LogFont = pApp->m_LogFont;
	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);
	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	// store height/width
	m_nLogFontHeight = m_LogFont.lfHeight;
	m_nLogFontWidth = m_LogFont.lfWidth;

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current
	CreateFontBitmaps();

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	InvalidateRect(NULL, FALSE);
}

BOOL CGlView::CheckTextColor(BOOL bPermanent/* = TRUE*/)
{
	if ((m_ClearColorRed-m_TextColorRed)*(m_ClearColorRed-m_TextColorRed)+(m_ClearColorGreen-m_TextColorGreen)*(m_ClearColorGreen-m_TextColorGreen)+(m_ClearColorBlue-m_TextColorBlue)*(m_ClearColorBlue-m_TextColorBlue) < .12)
	{
		CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

		m_TextColorRed = (float)round(1.0-m_ClearColorRed);
		m_TextColorGreen = (float)round(1.0-m_ClearColorGreen);
		m_TextColorBlue = (float)round(min((m_ClearColorRed+m_ClearColorGreen)/2.,1.-(m_ClearColorRed+m_ClearColorGreen)/2.));
		if (bPermanent)
			pApp->m_OptionColorText = m_OptionColorText = RGB(int(round(m_TextColorRed*255.0f)),int(round(m_TextColorGreen*255.0f)),int(round(m_TextColorBlue*255.0f)));

		return FALSE;	// text color had to be changed
	}

	return TRUE;	// nothing to be changed
}

void CGlView::ScaleFont(double scaleFac)
{
	LOGFONT logFont;
	HGDIOBJ hOldObject;

	if (m_FontSmall.GetLogFont(&logFont))
	{
		logFont.lfHeight = int(m_dFontRatio*scaleFac*m_LogFont.lfHeight);
		logFont.lfWidth = int(m_dFontRatio*scaleFac*m_LogFont.lfWidth);
		hOldObject = m_FontSmall.Detach();
		if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
			m_FontSmall.Attach(hOldObject);
		else if (hOldObject != NULL)
			DeleteObject(hOldObject);
	}
	if (m_FontNormal.GetLogFont(&logFont))
	{
		logFont.lfHeight = int(scaleFac*m_LogFont.lfHeight);
		logFont.lfWidth = int(scaleFac*m_LogFont.lfWidth);
		hOldObject = m_FontNormal.Detach();
		if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&logFont)) == FALSE)
			m_FontNormal.Attach(hOldObject);
		else if (hOldObject != NULL)
			DeleteObject(hOldObject);
	}

	// modify height/width
	m_LogFont.lfHeight = int(scaleFac*m_LogFont.lfHeight);
	m_LogFont.lfWidth = int(scaleFac*m_LogFont.lfWidth);
}

void CGlView::UnScaleFont()
{
	LOGFONT logFont = m_LogFont;
	HGDIOBJ hOldObject;

	// restore height/width
	m_LogFont.lfHeight = m_nLogFontHeight;
	m_LogFont.lfWidth = m_nLogFontWidth;

	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);
	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
}

void CGlView::CreateFontBitmaps()
{
	HGDIOBJ hOldFont = NULL;
	int retry;

	if (m_bFontCreateFlagSmall == TRUE)
		hOldFont = SelectObject(m_pDC->GetSafeHdc(), m_FontSmall);
	else
		hOldFont = SelectObject(m_pDC->GetSafeHdc(), GetStockObject(SYSTEM_FONT));
	
	// the display list numbering starts at m_nListBaseSmall, an arbitrary choice 
	for (retry = 0; retry < 10; retry++)
	{
		// this loop is due to a NT bug while printing
		// it seems wglUseFontBitmaps returns FALSE the first time
		// try it at most 10 times
		if (wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_nListBaseSmall))
			break;
	}

	if (m_bFontCreateFlagNormal == TRUE)
		SelectObject(m_pDC->GetSafeHdc(), m_FontNormal);
	else
		SelectObject(m_pDC->GetSafeHdc(), GetStockObject(SYSTEM_FONT));
	
	// the display list numbering starts at m_nListBaseNormal, an arbitrary choice 
	for (retry = 0; retry < 10; retry++)
	{
		// this loop is due to a NT bug while printing
		// it seems wglUseFontBitmaps returns FALSE the first time
		// try it at most 10 times
		if (wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_nListBaseNormal))
			break;
	}

	if (hOldFont != NULL)
		SelectObject(m_pDC->GetSafeHdc(), hOldFont);
}

void CGlView::DrawZoomRect()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);	
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);
	GLushort line_stipple = 2175;
	glLineStipple(1, line_stipple);
	
	glColor3d(round(1.0-m_ClearColorRed), round(1.0-m_ClearColorGreen), round(min((m_ClearColorRed+m_ClearColorGreen)/2.,1.-(m_ClearColorRed+m_ClearColorGreen)/2.)));
	glBegin(GL_LINE_LOOP);
	glVertex2d(m_StartPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_EndPoint[1]);
	glVertex2d(m_StartPoint[0], m_EndPoint[1]);
	glEnd();
	glLineStipple(repeat, pattern);

	glLineWidth(tmpLineWidth[0]);
}

void CGlView::DrawRubberBand()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);	
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);
	GLushort line_stipple = 2175;
	glLineStipple(1, line_stipple);
	
	glColor3d(round(1.0-m_ClearColorRed), round(1.0-m_ClearColorGreen), round(min((m_ClearColorRed+m_ClearColorGreen)/2.,1.-(m_ClearColorRed+m_ClearColorGreen)/2.)));
	glBegin(GL_LINE_STRIP);
	glVertex2d(m_StartPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_EndPoint[1]);
	glEnd();
	glLineStipple(repeat, pattern);

	glBegin(GL_LINE_LOOP);
	glVertex2d(m_StartPoint[0]-.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_StartPoint[1]-.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
	glVertex2d(m_StartPoint[0]-.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_StartPoint[1]+.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
	glVertex2d(m_StartPoint[0]+.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_StartPoint[1]+.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
	glVertex2d(m_StartPoint[0]+.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_StartPoint[1]-.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
	glEnd();
	if (m_bFirstPickPoint == TRUE)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_EndPoint[0], m_EndPoint[1]-.071/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glVertex2d(m_EndPoint[0]-.071/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]);
		glVertex2d(m_EndPoint[0], m_EndPoint[1]+.071/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glVertex2d(m_EndPoint[0]+.071/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]);
		glEnd();
	}
	if (m_bSecondPickPoint == TRUE)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_EndPoint[0]-.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]-.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glVertex2d(m_EndPoint[0]-.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]+.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glVertex2d(m_EndPoint[0]+.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]+.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glVertex2d(m_EndPoint[0]+.05/((m_xScaling==0.0f) ? 1.0f : m_xScaling), m_EndPoint[1]-.05/((m_yScaling==0.0f) ? 1.0f : m_yScaling));
		glEnd();
	}

	glLineWidth(tmpLineWidth[0]);
}

/////////////////////////////////////////////////////////////////////////////
// Plot and surf

void CGlView::Plot2d(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pCC);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matX, CMatrix& matY, CMatrix& matC)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix* pXX, CMatrix* pYY)									// Aufruf mit 2 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matX, CMatrix& matY)									// Aufruf mit 2 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix* pYY)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;

	if (pYY == NULL)
		return;

	matX.SetSize(pYY->GetSize());

	int sizeX = pYY->GetSize().cx;
	int sizeY = pYY->GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		for (int j = 1; j <= sizeY; j++)
		{
			matX(i,j) = (2.*double(i) - (pYY->GetSize().cx + 1.))/(pYY->GetSize().cx - 1.);
		}
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matY)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;

	matX.SetSize(matY.GetSize());

	int sizeX = matY.GetSize().cx;
	int sizeY = matY.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		for (int j = 1; j <= sizeY; j++)
		{
			matX(i,j) = (2.*double(i) - (matY.GetSize().cx + 1.))/(matY.GetSize().cx - 1.);
		}
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderScenePlot2d();
}

void CGlView::Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC)		// Aufruf mit 4 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pCC);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC)	// Aufruf mit 4 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matC);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix* pZZ)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pZZ == NULL)
		return;

	int sizeY = pZZ->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (pZZ->GetSize().cy + 1.))/(pZZ->GetSize().cy - 1.);
	}

	int sizeX = pZZ->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (pZZ->GetSize().cx + 1.))/(pZZ->GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matZ)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matZ.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (matZ.GetSize().cy + 1.))/(matZ.GetSize().cy - 1.);
	}

	int sizeX = matZ.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (matZ.GetSize().cx + 1.))/(matZ.GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderScenePlot3d();
}

void CGlView::Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC)			// Aufruf mit 4 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC)		// Aufruf mit 4 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ)						// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix* pZZ)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pZZ == NULL)
		return;

	int sizeY = pZZ->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (pZZ->GetSize().cy + 1.))/(pZZ->GetSize().cy - 1.);
	}

	int sizeX = pZZ->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (pZZ->GetSize().cx + 1.))/(pZZ->GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matZ)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matZ.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (matZ.GetSize().cy + 1.))/(matZ.GetSize().cy - 1.);
	}

	int sizeX = matZ.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (matZ.GetSize().cx + 1.))/(matZ.GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderSceneSurf();
}

void CGlView::TopView(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matX, CMatrix& matY, CMatrix& matC)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix* pXX, CMatrix* pYY)									// Aufruf mit 2 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matX, CMatrix& matY)									// Aufruf mit 2 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix* pCC)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pCC == NULL)
		return;

	int sizeY = pCC->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (pCC->GetSize().cy + 1.))/(pCC->GetSize().cy - 1.);
	}

	int sizeX = pCC->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (pCC->GetSize().cx + 1.))/(pCC->GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matC)												// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matC.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (matC.GetSize().cy + 1.))/(matC.GetSize().cy - 1.);
	}

	int sizeX = matC.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (matC.GetSize().cx + 1.))/(matC.GetSize().cx - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneTopView();
}

void CGlView::Scale2d(CScale* pSS)													// Aufruf mit 1 Pointer auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, pSS);

	RenderSceneScale2d();
}

void CGlView::Scale2d(CScale& matS)													// Aufruf mit 1 Referenz auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, &matS);

	RenderSceneScale2d();
}

void CGlView::Scale3d(CScale* pSS)													// Aufruf mit 1 Pointer auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, pSS);

	RenderSceneScale3d();
}

void CGlView::Scale3d(CScale& matS)													// Aufruf mit 1 Referenz auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, &matS);

	RenderSceneScale3d();
}

/////////////////////////////////////////////////////////////////////////////
// Rendering

void CGlView::RenderScenePlot2d()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x, y, c;
	double r, g, b;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -=.5;
		maxCC +=.5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 0)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j <= sizeY; j++)
			{
				glBegin(GL_LINE_STRIP);
				for (i = 1; i <= sizeX; i++)
				{
					x = (*pXX)(bIsXXRow ? 1 : i,j);
					y = (*pYY)(i,bIsYYCol ? 1 : j);
					c = (*pCC)(i,j);

					fCC = (c-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r, g, b);

					glColor3d(r,g,b);
					glVertex2d(x,y);
				}
				glEnd();
			}
		}
	}
}

void CGlView::RenderScenePlot3d()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pZZ, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x, y, z, c;
	double r, g, b;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_ZRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -=.5;
		maxCC +=.5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && pZZ->GetSize() == pCC->GetSize() && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 0)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// set outer normals (to the sky)
			glNormal3d(0., 1., 0.);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j <= sizeY; j++)
			{
				glBegin(GL_LINE_STRIP);
				for (i = 1; i <= sizeX; i++)
				{
					x = (*pXX)(bIsXXRow ? 1 : i,j);
					y = (*pYY)(i,bIsYYCol ? 1 : j);
					z = (*pZZ)(i,j);
					c = (*pCC)(i,j);

					fCC = (c-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r, g, b);

					glColor3d(r,g,b);
					glVertex3d(x,z,-y);
				}
				glEnd();
			}
		}
	}
	glNormal3dv(tmpNormal);
}

void CGlView::RenderSceneSurf()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pZZ, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x11, x12, x21, x22;
	double y11, y12, y21, y22;
	double z11, z12, z21, z22;
	double c11, c12, c21, c22;
	double r11, g11, b11;
	double r12, g12, b12;
	double r21, g21, b21;
	double r22, g22, b22;
	double fCC;
	double xNormal, yNormal, zNormal;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_ZRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -=.5;
		maxCC +=.5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && pZZ->GetSize() == pCC->GetSize() && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 1)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Is surface?
			GLint tmpPolygonMode[2];
			glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
			GLboolean tmpLighting = glIsEnabled(GL_LIGHTING);

			if (m_bBlackLinesOrRemoveHidden) 
			{
				glEnable(GL_STENCIL_TEST);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilMask(1);
				glStencilFunc(GL_ALWAYS, 0, 1);
				glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
			}

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j < sizeY; j++)
			{
				for (i = 1; i < sizeX; i++)
				{
					x11 = (*pXX)(bIsXXRow ? 1 : i,j);
					x12 = (*pXX)(bIsXXRow ? 1 : i,j+1);
					x21 = (*pXX)(bIsXXRow ? 1 : i+1,j);
					x22 = (*pXX)(bIsXXRow ? 1 : i+1,j+1);

					y11 = (*pYY)(i,bIsYYCol ? 1 : j);
					y12 = (*pYY)(i,bIsYYCol ? 1 : j+1);
					y21 = (*pYY)(i+1,bIsYYCol ? 1 : j);
					y22 = (*pYY)(i+1,bIsYYCol ? 1 : j+1);

					z11 = (*pZZ)(i,j);
					z12 = (*pZZ)(i,j+1);
					z21 = (*pZZ)(i+1,j);
					z22 = (*pZZ)(i+1,j+1);

					c11 = (*pCC)(i,j);
					c12 = (*pCC)(i,j+1);
					c21 = (*pCC)(i+1,j);
					c22 = (*pCC)(i+1,j+1);

					fCC = (c11-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r11, g11, b11);

					fCC = (c12-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r12, g12, b12);

					fCC = (c21-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r21, g21, b21);

					fCC = (c22-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r22, g22, b22);

					// set outer normals
					crossprod(x11-x22, y11-y22, z11-z22, x12-x21, y12-y21, z12-z21, xNormal, yNormal, zNormal);
					if (zNormal < 0.)
					{
						xNormal *= -1.;
						yNormal *= -1.;
						zNormal *= -1.;
					}
					glNormal3d(xNormal, zNormal, -yNormal);

					if (m_bBlackLinesOrRemoveHidden) 
					{
						// Stencil buffer operations
						if (tmpPolygonMode[1] == GL_FILL)
						{
							// Draw Black lines on the surface
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glColor3d(0.,0.,0.);
							glBegin(GL_QUADS);

							glVertex3d(x11,z11,-y11);
							glVertex3d(x12,z12,-y12);
							glVertex3d(x22,z22,-y22);
							glVertex3d(x21,z21,-y21);

							glEnd();

							glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						}
						else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
						{
							// Draw the points/lines themselves
							glBegin(GL_QUADS);

							glColor3d(r11,g11,b11);
							glVertex3d(x11,z11,-y11);
							glColor3d(r12,g12,b12);
							glVertex3d(x12,z12,-y12);
							glColor3d(r22,g22,b22);
							glVertex3d(x22,z22,-y22);
							glColor3d(r21,g21,b21);
							glVertex3d(x21,z21,-y21);

							glEnd();
						}
						glStencilFunc(GL_EQUAL, 0, 1);
						glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					}

					// Normal operations
					if (!m_bBlackLinesOrRemoveHidden || tmpPolygonMode[1] == GL_FILL)
					{
						// Draw the faces themselves
						glBegin(GL_QUADS);

						glColor3d(r11,g11,b11);
						glVertex3d(x11,z11,-y11);
						glColor3d(r12,g12,b12);
						glVertex3d(x12,z12,-y12);
						glColor3d(r22,g22,b22);
						glVertex3d(x22,z22,-y22);
						glColor3d(r21,g21,b21);
						glVertex3d(x21,z21,-y21);

						glEnd();
					}
					else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
					{
						// Draw background color to inner face
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						if (tmpLighting)
							glDisable(GL_LIGHTING);

						glColor3d(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue);
						glBegin(GL_QUADS);

						glVertex3d(x11,z11,-y11);
						glVertex3d(x12,z12,-y12);
						glVertex3d(x22,z22,-y22);
						glVertex3d(x21,z21,-y21);

						glEnd();

						glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						if (tmpLighting)
							glEnable(GL_LIGHTING);
					}

					if (m_bBlackLinesOrRemoveHidden) 
					{
						// Stencil buffer operations
						glStencilFunc(GL_ALWAYS, 0, 1);
						glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
						if (tmpPolygonMode[1] == GL_FILL)
						{
							// Draw Black lines on Surfaces
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glColor3d(0.,0.,0.);
							glBegin(GL_QUADS);

							glVertex3d(x11,z11,-y11);
							glVertex3d(x12,z12,-y12);
							glVertex3d(x22,z22,-y22);
							glVertex3d(x21,z21,-y21);

							glEnd();

							glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						}
						else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
						{
							// Draw the points/lines themselves
							glBegin(GL_QUADS);

							glColor3d(r11,g11,b11);
							glVertex3d(x11,z11,-y11);
							glColor3d(r12,g12,b12);
							glVertex3d(x12,z12,-y12);
							glColor3d(r22,g22,b22);
							glVertex3d(x22,z22,-y22);
							glColor3d(r21,g21,b21);
							glVertex3d(x21,z21,-y21);

							glEnd();
						}
					}
				}
			}

			if (m_bBlackLinesOrRemoveHidden) 
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
	glNormal3dv(tmpNormal);
}

void CGlView::RenderSceneTopView()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x1, x2;
	double y1, y2;
	double c1, c2;
	double r1, g1, b1;
	double r2, g2, b2;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -=.5;
		maxCC +=.5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 1)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j < sizeY; j++)
			{
				glBegin(GL_QUAD_STRIP); 
				for (i = 1; i <= sizeX; i++)
				{
					x1 = (*pXX)(bIsXXRow ? 1 : i,j);
					x2 = (*pXX)(bIsXXRow ? 1 : i,j+1);

					y1 = (*pYY)(i,bIsYYCol ? 1 : j);
					y2 = (*pYY)(i,bIsYYCol ? 1 : j+1);

					c1 = (*pCC)(i,j);
					c2 = (*pCC)(i,j+1);

					fCC = (c1-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r1, g1, b1);

					fCC = (c2-minCC)/(maxCC-minCC);
					MakeColorsRainbow(fCC, r2, g2, b2);

					glColor3d(r1,g1,b1);
					glVertex2d(x1,y1);
					glColor3d(r2,g2,b2);
					glVertex2d(x2,y2);
				}
				glEnd();
			}
		}
	}
}

void CGlView::RenderSceneScale2d()
{
	// Default rendering 
	CScale *pSS;
	CString strValue;
	GLubyte dummyBitmap = 1;
	GLfloat tmpCurrentRasterPosition[4];
	float negStrWidth;
	int k, size;
	const double OneMinus = 1.;

	GLint tmpPolygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(m_TextColorRed, m_TextColorGreen, m_TextColorBlue);

	size = m_SRenderArray.GetSize();
	for (k = 0; k < size; k++)
	{
		pSS = (CScale*) m_SRenderArray.GetAt(k);
		if (pSS == NULL)
			continue;

		if (m_bScaleShowFrame) 
		{
			// Draw the lines themselves
			glBegin(GL_QUADS);

			glVertex2d(-OneMinus, -OneMinus);
			glVertex2d(-OneMinus, OneMinus);
			glVertex2d(OneMinus,OneMinus);
			glVertex2d(OneMinus, -OneMinus);

			glEnd();

			CMatrix matGridX, matGridY;
			CMatrix matGridXNormalized(-1., 1., .2);
			CMatrix matGridYNormalized(-1., 1., .2);

			if (m_bScaleShowGrid) 
			{
				int k, size;

				GLushort pattern;
				GLint repeat;
				glEnable(GL_LINE_STIPPLE);	
				glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
				glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);
				GLushort line_stipple = 21845;
				glLineStipple(1, line_stipple);

/*				double dXScalingRastered = 1.;
				if (m_xScaling > 0.)
				{
					double dXScalingLog = log10(m_xScaling);
					double dXScalingLog1 = floor(dXScalingLog);
					double dXScalingLog2 = dXScalingLog - dXScalingLog1;

					double dXScaling1 = round(pow(10., dXScalingLog1));
					if (dXScaling1 > 1.)
						dXScalingRastered = dXScaling1;
				}
*/
				double dDiffX = pSS->m_dXMax - pSS->m_dXMin;
				double dStepSizeX;
				if (dDiffX != HUGE && dDiffX != -HUGE && !_isnan(dDiffX) && dDiffX > 0.)
				{
					double dDiffXLog = log10(dDiffX);
					double dDiffXLog1 = floor(dDiffXLog);
					double dDiffXLog2 = dDiffXLog - dDiffXLog1;

					double dDiffX1 = pow(10., dDiffXLog1);
					double dDiffX2 = pow(10., dDiffXLog2);

					if (dDiffX2 <= 2.)
						dStepSizeX = .2*dDiffX1;
					else if (dDiffX2 <= 5.)
						dStepSizeX = .5*dDiffX1;
					else
						dStepSizeX = dDiffX1;
//					dStepSizeX /= dXScalingRastered;

					double dFirstX = ceil(pSS->m_dXMin/dStepSizeX)*dStepSizeX;
					double dLastX = floor(pSS->m_dXMax/dStepSizeX)*dStepSizeX;

/*					double dMediumX = (pSS->m_dXMax*(1.-m_xTranslation) + pSS->m_dXMin*(1.+m_xTranslation))/(2.*m_xScaling);
					if (dFirstX < dMediumX - 100.* dStepSizeX)
						dFirstX = ceil((dMediumX - 10.*m_aspect*dStepSizeX)/dStepSizeX)*dStepSizeX;;
					if (dLastX > dMediumX + 100.* dStepSizeX)
						dLastX = floor((dMediumX + 10.*m_aspect*dStepSizeX)/dStepSizeX)*dStepSizeX;;
*/
					matGridX = CMatrix(dFirstX, dLastX, dStepSizeX);

					// omit last value if very close to penultimate
					int size = matGridX.GetSize().cy;
					if (size > 2 && fabs(matGridX[size-1]-matGridX[size-2])/fabs(matGridX[size-2]-matGridX[size-3]) < .5)
						matGridX.SetSize(CSize(1, size-1));

					matGridXNormalized = matGridX;
					matGridXNormalized *= 2.;
					matGridXNormalized -= pSS->m_dXMax + pSS->m_dXMin;
					matGridXNormalized /= dDiffX;
				}

/*				double dYScalingRastered = 1.;
				if (m_yScaling > 0.)
				{
					double dYScalingLog = log10(m_yScaling);
					double dYScalingLog1 = floor(dYScalingLog);
					double dYScalingLog2 = dYScalingLog - dYScalingLog1;

					double dYScaling1 = round(pow(10., dYScalingLog1));
					if (dYScaling1 > 1.)
						dYScalingRastered = dYScaling1;
				}
*/
				double dDiffY = pSS->m_dYMax - pSS->m_dYMin;
				double dStepSizeY;
				if (dDiffY != HUGE && dDiffY != -HUGE && !_isnan(dDiffY) && dDiffY > 0.)
				{
					double dDiffYLog = log10(dDiffY);
					double dDiffYLog1 = floor(dDiffYLog);
					double dDiffYLog2 = dDiffYLog - dDiffYLog1;

					double dDiffY1 = pow(10., dDiffYLog1);
					double dDiffY2 = pow(10., dDiffYLog2);

					if (dDiffY2 <= 2.)
						dStepSizeY = .2*dDiffY1;
					else if (dDiffY2 <= 5.)
						dStepSizeY = .5*dDiffY1;
					else
						dStepSizeY = dDiffY1;
//					dStepSizeY /= dYScalingRastered;

					double dFirstY = ceil(pSS->m_dYMin/dStepSizeY)*dStepSizeY;
					double dLastY = floor(pSS->m_dYMax/dStepSizeY)*dStepSizeY;

/*					double dMediumY = (pSS->m_dYMax*(1.-m_yTranslation) + pSS->m_dYMin*(1.+m_yTranslation))/(2.*m_yScaling);
					if (dFirstY < dMediumY - 100.* dStepSizeY)
						dFirstY = ceil((dMediumY - 10.* dStepSizeY)/dStepSizeY)*dStepSizeY;;
					if (dLastY > dMediumY + 100.* dStepSizeY)
						dLastY = floor((dMediumY + 10.* dStepSizeY)/dStepSizeY)*dStepSizeY;;
*/
					matGridY = CMatrix(dFirstY, dLastY, dStepSizeY);

					// omit last value if very close to penultimate
					int size = matGridY.GetSize().cy;
					if (size > 2 && fabs(matGridY[size-1]-matGridY[size-2])/fabs(matGridY[size-2]-matGridY[size-3]) < .5)
						matGridY.SetSize(CSize(1, size-1));

					matGridYNormalized = matGridY ;
					matGridYNormalized *= 2.;
					matGridYNormalized -= pSS->m_dYMax + pSS->m_dYMin;
					matGridYNormalized /= dDiffY;
				}

				size = matGridXNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(matGridXNormalized[k], -OneMinus);
					glVertex2d(matGridXNormalized[k], OneMinus);
					glEnd();
				}
				size = matGridYNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(-OneMinus, matGridYNormalized[k]);
					glVertex2d(OneMinus, matGridYNormalized[k]);
					glEnd();
				}

				glLineStipple(repeat, pattern);
			}

			if (m_bScaleShowLabels) 
			{
				glListBase(m_nListBaseSmall); 

				glPushAttrib(GL_CURRENT_RASTER_POSITION);

				// text x min
				if (pSS->m_dXMin != HUGE && pSS->m_dXMin != -HUGE && !_isnan(pSS->m_dXMin))
				{
					strValue.Format("%.4g", pSS->m_dXMin);
					glRasterPos2d(-1., -1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text x max
				if (pSS->m_dXMax != HUGE && pSS->m_dXMax != -HUGE && !_isnan(pSS->m_dXMax))
				{
					strValue.Format("%.4g", pSS->m_dXMax);
					if (!pSS->m_strXLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strXLabel;
					glRasterPos3d(1., -1., -eps6);

					glGetFloatv(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth = tmpCurrentRasterPosition[0];
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glGetFloatv(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth -= tmpCurrentRasterPosition[0];

					glRasterPos2d(1., -1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, negStrWidth, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text x grid
				size = matGridX.GetTotalSize();
				for (k = 1; k < size-1; k++)
				{
					strValue.Format("%.10g", matGridX[k]);
					glRasterPos3d(matGridXNormalized[k], -1., -eps6);

					glGetFloatv(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth = tmpCurrentRasterPosition[0];
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glGetFloatv(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth -= tmpCurrentRasterPosition[0];

					glRasterPos2d(matGridXNormalized[k], -1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, negStrWidth/2.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text y min
				if (pSS->m_dYMin != HUGE && pSS->m_dYMin != -HUGE && !_isnan(pSS->m_dYMin))
				{
					strValue.Format("  %.4g", pSS->m_dYMin);
					glRasterPos2d(1., -1.);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text y max
				if (pSS->m_dYMax != HUGE && pSS->m_dYMax != -HUGE && !_isnan(pSS->m_dYMax))
				{
					strValue.Format("  %.4g", pSS->m_dYMax);
					if (!pSS->m_strYLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strYLabel;
					glRasterPos2d(1., 1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text y grid
				size = matGridY.GetTotalSize();
				for (k = 1; k < size-1; k++)
				{
					strValue.Format("  %.10g", matGridY[k]);
					glRasterPos2d(1., matGridYNormalized[k]);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio*.5/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				glPopAttrib();
			}
		}

		if (m_bScaleShowAxis) 
		{
			double x, y;

			if (pSS->m_dXMax == HUGE && pSS->m_dXMin == -HUGE)
				x = 0.;
			else if (pSS->m_dXMax == HUGE)
				x = -OneMinus;
			else if (pSS->m_dXMin == -HUGE)
				x = OneMinus;
			else
				x = max(-OneMinus, min(OneMinus, -(pSS->m_dXMax + pSS->m_dXMin)/(pSS->m_dXMax - pSS->m_dXMin)));

			if (pSS->m_dYMax == HUGE && pSS->m_dYMin == -HUGE)
				y = 0.;
			else if (pSS->m_dYMax == HUGE)
				y = -OneMinus;
			else if (pSS->m_dYMin == -HUGE)
				y = OneMinus;
			else
				y = max(-OneMinus, min(OneMinus, -(pSS->m_dYMax + pSS->m_dYMin)/(pSS->m_dYMax - pSS->m_dYMin)));

			GLfloat tmpLineWidth[1];
			glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
			glLineWidth(1.5);

			// Draw the lines themselves
			glBegin(GL_LINE_STRIP);
			glVertex2d(-OneMinus,y);
			glVertex2d(OneMinus,y);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex2d(x, -OneMinus);
			glVertex2d(x, OneMinus);
			glEnd();

			glLineWidth(tmpLineWidth[0]);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
}

void CGlView::RenderSceneScale3d()
{
	// Default rendering 
	CScale *pSS;
	CString strValue;
	GLubyte dummyBitmap = 1;
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	GLdouble winx, winy, winz;

	int k, size;
	double distFactor = 1.025, rotXShift = (1.-cos(pi/180*(m_xRotation+45.)))/2.;
	const double OnePlus = 1.;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLint tmpPolygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	glColor3f(m_TextColorRed, m_TextColorGreen, m_TextColorBlue);

	size = m_SRenderArray.GetSize();
	for (k = 0; k < size; k++)
	{
		pSS = (CScale*) m_SRenderArray.GetAt(k);
		if (pSS == NULL)
			continue;

		if (m_bScaleShowFrame) 
		{
			// Draw the lines themselves
			glBegin(GL_QUADS);

			glNormal3d(  -OnePlus,  0.0,  0.0);
			glVertex3d(  OnePlus,  OnePlus,  OnePlus);
			glVertex3d(  OnePlus, -OnePlus,  OnePlus);
			glVertex3d(  OnePlus, -OnePlus, -OnePlus);
			glVertex3d(  OnePlus,  OnePlus, -OnePlus);

			glNormal3d( OnePlus,  0.0,  0.0);
			glVertex3d( -OnePlus, -OnePlus,  OnePlus);
			glVertex3d( -OnePlus,  OnePlus,  OnePlus);
			glVertex3d( -OnePlus,  OnePlus, -OnePlus);
			glVertex3d( -OnePlus, -OnePlus, -OnePlus);

			glNormal3d(  0.0,  -OnePlus,  0.0);
			glVertex3d(  OnePlus,  OnePlus,  OnePlus);
			glVertex3d( -OnePlus,  OnePlus,  OnePlus);
			glVertex3d( -OnePlus,  OnePlus, -OnePlus);
			glVertex3d(  OnePlus,  OnePlus, -OnePlus);

			glNormal3d(  0.0, OnePlus,  0.0);
			glVertex3d( -OnePlus, -OnePlus,  OnePlus);
			glVertex3d(  OnePlus, -OnePlus,  OnePlus);
			glVertex3d(  OnePlus, -OnePlus, -OnePlus);
			glVertex3d( -OnePlus, -OnePlus, -OnePlus);

			glNormal3d(  0.0,  0.0,  -OnePlus);
			glVertex3d(  OnePlus,  OnePlus,  OnePlus);
			glVertex3d( -OnePlus,  OnePlus,  OnePlus);
			glVertex3d( -OnePlus, -OnePlus,  OnePlus);
			glVertex3d(  OnePlus, -OnePlus,  OnePlus);

			glNormal3d(  0.0,  0.0, OnePlus);
			glVertex3d( -OnePlus,  OnePlus, -OnePlus);
			glVertex3d(  OnePlus,  OnePlus, -OnePlus);
			glVertex3d(  OnePlus, -OnePlus, -OnePlus);
			glVertex3d( -OnePlus, -OnePlus, -OnePlus);

			glEnd();

			CMatrix matGridX, matGridY, matGridZ;
			CMatrix matGridXNormalized(-1., 1., .2);
			CMatrix matGridYNormalized(-1., 1., .2);
			CMatrix matGridZNormalized(-1., 1., .2);

			double dRasterPos1Remember = 0., dRasterPos2Remember = 0., dRasterPos3Remember = 0.;

			if (m_bScaleShowGrid) 
			{
				int k, size;

				GLushort pattern;
				GLint repeat;
				glEnable(GL_LINE_STIPPLE);	
				glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
				glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);
				GLushort line_stipple = 21845;
				glLineStipple(1, line_stipple);

				double dDiffX = pSS->m_dXMax - pSS->m_dXMin;
				double dStepSizeX;
				if (dDiffX != HUGE && dDiffX != -HUGE && !_isnan(dDiffX) && dDiffX > 0.)
				{
					double dDiffXLog = log10(dDiffX);
					double dDiffXLog1 = floor(dDiffXLog);
					double dDiffXLog2 = dDiffXLog - dDiffXLog1;

					double dDiffX1 = pow(10., dDiffXLog1);
					double dDiffX2 = pow(10., dDiffXLog2);

					if (dDiffX2 <= 2.)
						dStepSizeX = .2*dDiffX1;
					else if (dDiffX2 <= 5.)
						dStepSizeX = .5*dDiffX1;
					else
						dStepSizeX = dDiffX1;

					double dFirstX = ceil(pSS->m_dXMin/dStepSizeX)*dStepSizeX;
					double dLastX = floor(pSS->m_dXMax/dStepSizeX)*dStepSizeX;

					matGridX = CMatrix(dFirstX, dLastX, dStepSizeX);

					// omit last value if very close to penultimate
					int size = matGridX.GetSize().cy;
					if (size > 2 && fabs(matGridX[size-1]-matGridX[size-2])/fabs(matGridX[size-2]-matGridX[size-3]) < .5)
						matGridX.SetSize(CSize(1, size-1));

					matGridXNormalized = matGridX;
					matGridXNormalized *= 2.;
					matGridXNormalized -= pSS->m_dXMax + pSS->m_dXMin;
					matGridXNormalized /= dDiffX;
				}

				double dDiffY = pSS->m_dYMax - pSS->m_dYMin;
				double dStepSizeY;
				if (dDiffY != HUGE && dDiffY != -HUGE && !_isnan(dDiffY) && dDiffY > 0.)
				{
					double dDiffYLog = log10(dDiffY);
					double dDiffYLog1 = floor(dDiffYLog);
					double dDiffYLog2 = dDiffYLog - dDiffYLog1;

					double dDiffY1 = pow(10., dDiffYLog1);
					double dDiffY2 = pow(10., dDiffYLog2);

					if (dDiffY2 <= 2.)
						dStepSizeY = .2*dDiffY1;
					else if (dDiffY2 <= 5.)
						dStepSizeY = .5*dDiffY1;
					else
						dStepSizeY = dDiffY1;

					double dFirstY = ceil(pSS->m_dYMin/dStepSizeY)*dStepSizeY;
					double dLastY = floor(pSS->m_dYMax/dStepSizeY)*dStepSizeY;

					matGridY = CMatrix(dFirstY, dLastY, dStepSizeY);

					// omit last value if very close to penultimate
					int size = matGridY.GetSize().cy;
					if (size > 2 && fabs(matGridY[size-1]-matGridY[size-2])/fabs(matGridY[size-2]-matGridY[size-3]) < .5)
						matGridY.SetSize(CSize(1, size-1));

					matGridYNormalized = matGridY;
					matGridYNormalized *= 2.;
					matGridYNormalized -= pSS->m_dYMax + pSS->m_dYMin;
					matGridYNormalized /= dDiffY;
				}

				double dDiffZ = pSS->m_dZMax - pSS->m_dZMin;
				double dStepSizeZ;
				if (dDiffZ != HUGE && dDiffZ != -HUGE && !_isnan(dDiffZ) && dDiffZ > 0.)
				{
					double dDiffZLog = log10(dDiffZ);
					double dDiffZLog1 = floor(dDiffZLog);
					double dDiffZLog2 = dDiffZLog - dDiffZLog1;

					double dDiffZ1 = pow(10., dDiffZLog1);
					double dDiffZ2 = pow(10., dDiffZLog2);

					if (dDiffZ2 <= 2.)
						dStepSizeZ = .2*dDiffZ1;
					else if (dDiffZ2 <= 5.)
						dStepSizeZ = .5*dDiffZ1;
					else
						dStepSizeZ = dDiffZ1;

					double dFirstZ = ceil(pSS->m_dZMin/dStepSizeZ)*dStepSizeZ;
					double dLastZ = floor(pSS->m_dZMax/dStepSizeZ)*dStepSizeZ;

					matGridZ = CMatrix(dFirstZ, dLastZ, dStepSizeZ);

					// omit last value if very close to penultimate
					int size = matGridZ.GetSize().cy;
					if (size > 2 && fabs(matGridZ[size-1]-matGridZ[size-2])/fabs(matGridZ[size-2]-matGridZ[size-3]) < .5)
						matGridZ.SetSize(CSize(1, size-1));

					matGridZNormalized = matGridZ;
					matGridZNormalized *= 2.;
					matGridZNormalized -= pSS->m_dZMax + pSS->m_dZMin;
					matGridZNormalized /= dDiffZ;
				}

				double dRasterPos1 = 0, dRasterPos2 = 0, dRasterPos3 = 0, zMin;

				// select best x plane
				zMin = HUGE;
				gluProject(-1., 0., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos1Remember = dRasterPos1 = OnePlus;
				}
				gluProject(1., 0., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos1Remember = dRasterPos1 = -OnePlus;
				}

				glNormal3d(-dRasterPos1, 0.0, 0.0);
				size = matGridYNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, -OnePlus, -matGridYNormalized[k]);
					glVertex3d(dRasterPos1, OnePlus, -matGridYNormalized[k]);
					glEnd();
				}
				size = matGridZNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, matGridZNormalized[k], OnePlus);
					glVertex3d(dRasterPos1, matGridZNormalized[k], -OnePlus);
					glEnd();
				}

				// select best y plane
				zMin = HUGE;
				gluProject(0., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos3Remember = dRasterPos3 = OnePlus;
				}
				gluProject(0., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos3Remember = dRasterPos3 = -OnePlus;
				}

				glNormal3d(0.0, 0.0, -dRasterPos3);
				size = matGridXNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(matGridXNormalized[k], -OnePlus, dRasterPos3);
					glVertex3d(matGridXNormalized[k], OnePlus, dRasterPos3);
					glEnd();
				}
				size = matGridZNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(-OnePlus, matGridZNormalized[k], dRasterPos3);
					glVertex3d(OnePlus, matGridZNormalized[k], dRasterPos3);
					glEnd();
				}

				// select best z plane
				zMin = HUGE;
				gluProject(0., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos2Remember = dRasterPos2 = OnePlus;
				}
				gluProject(0., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos2Remember = dRasterPos2 = -OnePlus;
				}

				glNormal3d(0.0, -dRasterPos2, 0.0);
				size = matGridXNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(matGridXNormalized[k], dRasterPos2, OnePlus);
					glVertex3d(matGridXNormalized[k], dRasterPos2, -OnePlus);
					glEnd();
				}
				size = matGridYNormalized.GetTotalSize();
				for (k = 0; k < size; k++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(-OnePlus, dRasterPos2, -matGridYNormalized[k]);
					glVertex3d(OnePlus, dRasterPos2, -matGridYNormalized[k]);
					glEnd();
				}

				glLineStipple(repeat, pattern);
			}

			if (m_bScaleShowLabels) 
			{
				glNormal3d(0., 1., 0.);

				glListBase(m_nListBaseSmall); 

				glPushAttrib(GL_CURRENT_RASTER_POSITION);

				double dRasterPos1 = 0, dRasterPos2 = 0, dRasterPos3 = 0, xMax = -HUGE, yMin = HUGE, yMax = -HUGE;

				// select best x axis
				gluProject(0., 1., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(!m_bScaleShowGrid || dRasterPos2Remember == OnePlus || dRasterPos3Remember == OnePlus))
				{
					yMin = winy;
					dRasterPos2 = 1.;
					dRasterPos3 = 1.;
				}
				gluProject(0., 1., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(!m_bScaleShowGrid || dRasterPos2Remember == OnePlus || dRasterPos3Remember == -OnePlus))
				{
					yMin = winy;
					dRasterPos2 = 1.;
					dRasterPos3 = -1.;
				}
				gluProject(0., -1., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(!m_bScaleShowGrid || dRasterPos2Remember == -OnePlus || dRasterPos3Remember == -OnePlus))
				{
					yMin = winy;
					dRasterPos2 = -1.;
					dRasterPos3 = -1.;
				}
				gluProject(0., -1., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(!m_bScaleShowGrid || dRasterPos2Remember == -OnePlus || dRasterPos3Remember == OnePlus))
				{
					yMin = winy;
					dRasterPos2 = -1.;
					dRasterPos3 = 1.;
				}

				// text x min
				if (pSS->m_dXMin != HUGE && pSS->m_dXMin != -HUGE && !_isnan(pSS->m_dXMin))
				{
					strValue.Format("%.4g", pSS->m_dXMin);
					glRasterPos3d(-1., dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text x max
				if (pSS->m_dXMax != HUGE && pSS->m_dXMax != -HUGE && !_isnan(pSS->m_dXMax))
				{
					strValue.Format("%.4g", pSS->m_dXMax);
					if (!pSS->m_strXLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strXLabel;
					glRasterPos3d(1., dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text x grid
				size = matGridX.GetTotalSize();
				for (k = 1; k < size-1; k++)
				{
					strValue.Format("%.10g", matGridX[k]);
					glRasterPos3d(matGridXNormalized[k], dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}


				// select best y axis
				gluProject(1., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == OnePlus || dRasterPos2Remember == OnePlus))
				{
					yMax = winy;
					dRasterPos1 = 1.;
					dRasterPos2 = 1.;
				}
				gluProject(1., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == OnePlus || dRasterPos2Remember == -OnePlus))
				{
					yMax = winy;
					dRasterPos1 = 1.;
					dRasterPos2 = -1.;
				}
				gluProject(-1., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == -OnePlus || dRasterPos2Remember == -OnePlus))
				{
					yMax = winy;
					dRasterPos1 = -1.;
					dRasterPos2 = -1.;
				}
				gluProject(-1., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == -OnePlus || dRasterPos2Remember == OnePlus))
				{
					yMax = winy;
					dRasterPos1 = -1.;
					dRasterPos2 = 1.;
				}

				// text y min
				if (pSS->m_dYMin != HUGE && pSS->m_dYMin != -HUGE && !_isnan(pSS->m_dYMin))
				{
					strValue.Format("%.4g", pSS->m_dYMin);
					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, 1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text y max
				if (pSS->m_dYMax != HUGE && pSS->m_dYMax != -HUGE && !_isnan(pSS->m_dYMax))
				{
					strValue.Format("%.4g", pSS->m_dYMax);
					if (!pSS->m_strYLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strYLabel;
					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, -1.);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text y grid
				size = matGridY.GetTotalSize();
				for (k = 1; k < size-1; k++)
				{
					strValue.Format("%.10g", matGridY[k]);
					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, -matGridYNormalized[k]);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}


				// select best z axis
				gluProject(1., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == OnePlus || dRasterPos3Remember == OnePlus))
				{
					xMax = winx;
					dRasterPos1 = 1.;
					dRasterPos3 = 1.;
				}
				gluProject(1., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == OnePlus || dRasterPos3Remember == -OnePlus))
				{
					xMax = winx;
					dRasterPos1 = 1.;
					dRasterPos3 = -1.;
				}
				gluProject(-1., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == -OnePlus || dRasterPos3Remember == -OnePlus))
				{
					xMax = winx;
					dRasterPos1 = -1.;
					dRasterPos3 = -1.;
				}
				gluProject(-1., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(!m_bScaleShowGrid || dRasterPos1Remember == -OnePlus || dRasterPos3Remember == OnePlus))
				{
					xMax = winx;
					dRasterPos1 = -1.;
					dRasterPos3 = 1.;
				}

				// text z min
				if (pSS->m_dZMin != HUGE && pSS->m_dZMin != -HUGE && !_isnan(pSS->m_dZMin))
				{
					strValue.Format(" %.4g", pSS->m_dZMin);
					glRasterPos3d(dRasterPos1*distFactor, -1., dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(rotXShift*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text z max
				if (pSS->m_dZMax != HUGE && pSS->m_dZMax != -HUGE && !_isnan(pSS->m_dZMax))
				{
					strValue.Format(" %.4g", pSS->m_dZMax);
					if (!pSS->m_strZLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strZLabel;
					glRasterPos3d(dRasterPos1*distFactor, 1., dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float((1.-rotXShift)*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				// text z grid
				size = matGridZ.GetTotalSize();
				for (k = 1; k < size-1; k++)
				{
					strValue.Format(" %.10g", matGridZ[k]);
					glRasterPos3d(dRasterPos1*distFactor, matGridZNormalized[k], dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float((1.-rotXShift)*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue); 
				}

				glPopAttrib();
			}
		}

		if (m_bScaleShowAxis) 
		{
			double x, y, z;

			if (pSS->m_dXMax == HUGE && pSS->m_dXMin == -HUGE)
				x = 0.;
			else if (pSS->m_dXMax == HUGE)
				x = -OnePlus;
			else if (pSS->m_dXMin == -HUGE)
				x = OnePlus;
			else
				x = max(-OnePlus, min(OnePlus, -(pSS->m_dXMax + pSS->m_dXMin)/(pSS->m_dXMax - pSS->m_dXMin)));

			if (pSS->m_dYMax == HUGE && pSS->m_dYMin == -HUGE)
				y = 0.;
			else if (pSS->m_dYMax == HUGE)
				y = -OnePlus;
			else if (pSS->m_dYMin == -HUGE)
				y = OnePlus;
			else
				y = max(-OnePlus, min(OnePlus, -(pSS->m_dYMax + pSS->m_dYMin)/(pSS->m_dYMax - pSS->m_dYMin)));

			if (pSS->m_dZMax == HUGE && pSS->m_dZMin == -HUGE)
				z = 0.;
			else if (pSS->m_dZMax == HUGE)
				z = -OnePlus;
			else if (pSS->m_dZMin == -HUGE)
				z = OnePlus;
			else
				z = max(-OnePlus, min(OnePlus, -(pSS->m_dZMax + pSS->m_dZMin)/(pSS->m_dZMax - pSS->m_dZMin)));

			// Draw the lines themselves
			glNormal3d(0., 1., 0.);

			glBegin(GL_LINE_STRIP);
			glVertex3d(-OnePlus,z,-y);
			glVertex3d(OnePlus,z,-y);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex3d(x,z,OnePlus);
			glVertex3d(x,z,-OnePlus);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex3d(x,-OnePlus,-y);
			glVertex3d(x,OnePlus,-y);
			glEnd();
		}
	}
	glNormal3dv(tmpNormal);
	glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
}

void CGlView::MakeColorsRainbow(const double& fcol, double& r, double& g, double& b)
{
	ASSERT(fabs(m_dInvertNonRainbowColors) == 1.);

	if (_isnan(fcol))
		r = g = b = .5;
	else if (fcol == HUGE*m_dInvertNonRainbowColors)
		r = g = b = 0.;
	else if (fcol == -HUGE*m_dInvertNonRainbowColors)
		r = g = b = 1.;
	else
	{
		//r = max(0.,min(-3.5*fcol+4.,min(1.,max(0.,7.*fcol-3.))));
		//g = max(0.,min(-3.5*fcol+3.,min(1.,max(0.,7.*fcol-1.))));
		//b = max(0.,min(-7.*fcol+3.,min(1.,max(0.,3.5*fcol+0.5))));
		b = max(0.,min(-17./3.*fcol+8./3.,min(1.,max(0.,17./4.*fcol+1./2.))));
		g = max(0.,min(-17./4.*fcol+15./4.,min(1.,max(0.,17./3.*fcol-2./3.))));
		r = max(0.,min(-17./4.*fcol+19./4.,min(1.,max(0.,17./3.*fcol-8./3.))));
	}
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGlView 

void CGlView::OnDestroy() 
{
	if (wglGetCurrentContext() != NULL)
		SetCurrent(NULL, NULL);
	
	if (m_hRC != NULL)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}
	
	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
		m_nBitsPerPixel = 0;
	}

	CView::OnDestroy();
}

void CGlView::OnSize(UINT nType, int cx, int cy) 
{
	HDC hDC;
	HGLRC hRC;

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current

	// Set OpenGL perspective, viewport and mode
	m_aspect = (cy == 0) ? (double)cx : (double)cx/(double)cy;
	
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	CView::OnSize(nType, cx, cy);
}

BOOL CGlView::OnEraseBkgnd(CDC*) 
{
	// OpenGl erases the background itself
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

void CGlView::OnHelpOpenglInfo() 
{
	CDlgOpenglInfo dlgOpenglInfo;
	dlgOpenglInfo.m_info = GetOpenGlInfo();
	dlgOpenglInfo.DoModal();
}

GlInfoStruct CGlView::GetOpenGlInfo()
{
	GlInfoStruct info;
	PIXELFORMATDESCRIPTOR pixelInf;
	HDC hDC;
	HGLRC hRC;

	// Get information about the DC's current pixel format 
	int nGLPixelIndex = GetPixelFormat(m_pDC->GetSafeHdc());
	DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelInf);

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current

	// Extract driver information
	if ((ICD_DRIVER_MASK & pixelInf.dwFlags) == 0)
	{
		info.strGlAcceleration.LoadString(IDS_OGL_HARDWARE);			// fully in hardware (fastest)
	}
	else if ((ICD_DRIVER_MASK & pixelInf.dwFlags) == ICD_DRIVER_MASK)
	{
		info.strGlAcceleration.LoadString(IDS_OGL_HWPARTIAL);	// partially in hardware (pretty fast, maybe..)
	}
	else
	{
		info.strGlAcceleration.LoadString(IDS_OGL_SOFTWARE);	// software
	}

	// get the company name responsible for this implementation
	info.strGlVendor = (char*)::glGetString(GL_VENDOR);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGlVendor.Format(IDS_NOTAVAILABLE);				// failed!

	// get the renderer name; this is specific of an hardware configuration
	info.strGlRenderer = (char*)::glGetString(GL_RENDERER);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGlRenderer.Format(IDS_NOTAVAILABLE);				// failed!

	// get the version of the GL library
	info.strGlVersion = (char*)::glGetString(GL_VERSION);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGlVersion.Format(IDS_NOTAVAILABLE);				// failed!

	// return a space separated list of extensions
	info.strGlExtensions = (char*)::glGetString(GL_EXTENSIONS);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGlExtensions.Format(IDS_NOTAVAILABLE);			// failed!

	// get the version of the GLU library
	info.strGluVersion = (char*)::gluGetString(GLU_VERSION);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGluVersion.Format(IDS_NOTAVAILABLE);				// failed!

	// as above a space separated list of extensions
	info.strGluExtensions = (char*)::gluGetString(GLU_EXTENSIONS);
	if (::glGetError() != GL_NO_ERROR) 
		info.strGluExtensions.Format(IDS_NOTAVAILABLE);			// failed!

	glGetIntegerv(GL_MAX_LIGHTS, &info.nMaxLights);
	glGetIntegerv(GL_MAX_CLIP_PLANES, &info.nMaxClipPlanes);
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &info.nMaxModelStackDepth);
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &info.nMaxProjectionStackDepth);
	glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &info.nMaxTextureStackDepth);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &info.nMaxNameStackDepth);
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &info.nMaxAttributeStackDepth);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info.nMaxTextureSize);
	glGetIntegerv(GL_MAX_LIST_NESTING, &info.nMaxListNesting);
	glGetIntegerv(GL_MAX_EVAL_ORDER, &info.nMaxEvalOrder);
	int tmpInt[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpInt);
	info.maxViewportDims.cx = tmpInt[0];
	info.maxViewportDims.cy = tmpInt[1];
	glGetIntegerv(GL_AUX_BUFFERS, &info.nAuxiliaryBuffers);
	float tmpFloat[2];
	glGetFloatv(GL_POINT_SIZE_RANGE, tmpFloat);
	info.minSmoothPointSize = tmpFloat[0];
	info.maxSmoothPointSize = tmpFloat[1];
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &info.smoothPointGranularity);
	glGetFloatv(GL_LINE_WIDTH_RANGE, tmpFloat);
	info.minSmoothLineSize = tmpFloat[0];
	info.maxSmoothLineSize = tmpFloat[1];
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &info.smoothLineGranularity);
	glGetIntegerv(GL_RED_BITS, &info.nRedBits);
	glGetIntegerv(GL_GREEN_BITS, &info.nGreenBits);
	glGetIntegerv(GL_BLUE_BITS, &info.nBlueBits);
	glGetIntegerv(GL_ALPHA_BITS, &info.nAlphaBits);
	glGetIntegerv(GL_DEPTH_BITS, &info.nDepthBits);
	glGetIntegerv(GL_STENCIL_BITS, &info.nStencilBits);

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	return info;
}

/////////////////////////////////////////////////////////////////////////////
// OnCreate
// Create OpenGL rendering context 
/////////////////////////////////////////////////////////////////////////////

int CGlView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	LOGFONT logFont;
	HGDIOBJ hOldObject;

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	logFont = m_LogFont = pApp->m_LogFont;	
	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);

	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	// store height/width
	m_nLogFontHeight = m_LogFont.lfHeight;
	m_nLogFontWidth = m_LogFont.lfWidth;

	InitializeOpenGL();
	return 0;
}

void CGlView::PrepareBkgnd(BOOL bBegin) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	if (bBegin)
	{
		if (pApp->m_bPrintBkgndWhite)
		{
			// store original colors
			m_ClearColorRedTmp   = m_ClearColorRed;
			m_ClearColorGreenTmp = m_ClearColorGreen;
			m_ClearColorBlueTmp  = m_ClearColorBlue;
			m_TextColorRedTmp    = m_TextColorRed;
			m_TextColorGreenTmp  = m_TextColorGreen;
			m_TextColorBlueTmp   = m_TextColorBlue;

			// change colors
			m_ClearColorRed   = 1.0f;
			m_ClearColorGreen = 1.0f;
			m_ClearColorBlue  = 1.0f;
			CheckTextColor(FALSE);

			m_dInvertNonRainbowColors = -1.;
		}

		if (pApp->m_nPrintObject == 1)
		{
			CDocument* pDoc = GetDocument();
			CView *pView;
			POSITION pos;

			pos = pDoc->GetFirstViewPosition();
			while (pos != NULL)
			{
				pView = pDoc->GetNextView(pos);
				if (pView != this && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					// store original colors
					((CGlView*)pView)->m_ClearColorRedTmp   = ((CGlView*)pView)->m_ClearColorRed;
					((CGlView*)pView)->m_ClearColorGreenTmp = ((CGlView*)pView)->m_ClearColorGreen;
					((CGlView*)pView)->m_ClearColorBlueTmp  = ((CGlView*)pView)->m_ClearColorBlue;
					((CGlView*)pView)->m_TextColorRedTmp    = ((CGlView*)pView)->m_TextColorRed;
					((CGlView*)pView)->m_TextColorGreenTmp  = ((CGlView*)pView)->m_TextColorGreen;
					((CGlView*)pView)->m_TextColorBlueTmp   = ((CGlView*)pView)->m_TextColorBlue;

					// change colors
					((CGlView*)pView)->m_ClearColorRed   = m_ClearColorRed;
					((CGlView*)pView)->m_ClearColorGreen = m_ClearColorGreen;
					((CGlView*)pView)->m_ClearColorBlue  = m_ClearColorBlue;
					((CGlView*)pView)->m_TextColorRed    = m_TextColorRed;
					((CGlView*)pView)->m_TextColorGreen  = m_TextColorGreen;
					((CGlView*)pView)->m_TextColorBlue   = m_TextColorBlue;

					((CGlView*)pView)->m_dInvertNonRainbowColors = m_dInvertNonRainbowColors;
				}
			}
		}
	}
	else
	{
		if (pApp->m_bPrintBkgndWhite)
		{
			// restore original colors
			m_ClearColorRed   = m_ClearColorRedTmp;
			m_ClearColorGreen = m_ClearColorGreenTmp;
			m_ClearColorBlue  = m_ClearColorBlueTmp;
			m_TextColorRed    = m_TextColorRedTmp;
			m_TextColorGreen  = m_TextColorGreenTmp;
			m_TextColorBlue   = m_TextColorBlueTmp;

			m_dInvertNonRainbowColors = 1.;
			InvalidateRect(NULL, FALSE);
		}

		if (pApp->m_nPrintObject == 1)
		{
			CDocument* pDoc = GetDocument();
			CView *pView;
			POSITION pos;

			pos = pDoc->GetFirstViewPosition();
			while (pos != NULL)
			{
				pView = pDoc->GetNextView(pos);
				if (pView != this && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					// restore original colors
					((CGlView*)pView)->m_ClearColorRed   = ((CGlView*)pView)->m_ClearColorRedTmp;
					((CGlView*)pView)->m_ClearColorGreen = ((CGlView*)pView)->m_ClearColorGreenTmp;
					((CGlView*)pView)->m_ClearColorBlue  = ((CGlView*)pView)->m_ClearColorBlueTmp;
					((CGlView*)pView)->m_TextColorRed    = ((CGlView*)pView)->m_TextColorRedTmp;
					((CGlView*)pView)->m_TextColorGreen  = ((CGlView*)pView)->m_TextColorGreenTmp;
					((CGlView*)pView)->m_TextColorBlue   = ((CGlView*)pView)->m_TextColorBlueTmp;

					((CGlView*)pView)->m_dInvertNonRainbowColors = 1.;
					((CGlView*)pView)->InvalidateRect(NULL, FALSE);
				}
			}
		}
	}
}

void CGlView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CView::OnSettingChange(uFlags, lpszSection);

	if (m_pDC->GetDeviceCaps(BITSPIXEL) != m_nBitsPerPixel)
	{
		if (wglGetCurrentContext() != NULL)
			SetCurrent(NULL, NULL);
		
		if (m_hRC != NULL)
		{
			wglDeleteContext(m_hRC);
			m_hRC = NULL;
		}
		
		if (m_pDC != NULL)
		{
			delete m_pDC;
			m_pDC = NULL;
			m_nBitsPerPixel = 0;
		}

		// Must temporarily remove the doc so MFC won't have a conniption.
		CDocument* pDocSave = m_pDocument;
		m_pDocument = NULL; // remove
		
		// Preserve same window ID (usually AFX_IDW_PANE_FIRST)
		UINT nIDView = GetDlgCtrlID();

		// Preserve same parent (frame)
		CWnd* pParent = GetParent();
		ASSERT(pParent);

		// Preserve same window pos and style
		CRect rect;
		GetWindowRect(&rect);
		pParent->ScreenToClient(&rect);
		DWORD dwStyle = GetStyle();

		// Remove the current view (window) and destroy it.
		HWND hWnd = UnsubclassWindow();
		ASSERT(hWnd);
		::DestroyWindow(hWnd);

		// Now the view is in more or less the same state as after calling
		//
		//		new CGlView;
		//
		// There is no window, no m_hWnd, and no document. So now we can 
		// re-create the view. 
		// Don't need to create context since document already exists.
		VERIFY(Create(NULL, NULL, dwStyle, rect, pParent, nIDView, NULL));

		// restore document
		m_pDocument = pDocSave;

		// Here we run into problems when recreating this view.
		// 
		// Normally the parent window sends a 
		// 		SendMessageToDescendants(WM_SETTINGCHANGE, ...);
		// message to all it's child windows, which calls OnSettingChange() for all views.
		//
		// If we recreating this view at this time, then the 
		// WM_SETTINGCHANGE message gets lost somehow, so that the other views don't
		// receive their WM_SETTINGCHANGE message. We have to repair that for the CGlViews.
		BOOL bFoundThisView = FALSE;
		POSITION posView = pDocSave->GetFirstViewPosition();
		while (posView != NULL)
		{
			CView* pView = pDocSave->GetNextView(posView);
			if (pView == this)
			{
				bFoundThisView = TRUE;
			}
			else if (bFoundThisView/* && pView->IsKindOf(RUNTIME_CLASS(CGlView))*/)
			{
				pView->SendMessage(WM_SETTINGCHANGE, (WPARAM)uFlags, (LPARAM)lpszSection);
				break;
			}
		}
	}
}
