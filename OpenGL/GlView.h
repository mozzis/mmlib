/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
// special thanks for the printing routines to
// Craig Fahrnbach <craignan@home.com>
//
/////////////////////////////////////////////////////////////////////////////
// GlView.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Matrix.h"
#include "Scale.h"

struct GlInfoStruct;

class AFX_EXT_CLASS CGlView : public CView
{
friend class CGlDocument;

	DECLARE_DYNAMIC(CGlView)
public:
	CGlView();

// Attributes
public:
	CPalette m_Palette;
	BOOL m_bIsDocPrinting;
	BOOL m_bBlackLinesOrRemoveHidden;
	BOOL m_bScaleShowFrame;
	BOOL m_bScaleShowLabels;
	BOOL m_bScaleShowGrid;
	BOOL m_bScaleShowAxis;
	int m_DocPrintHeight;

	// OpenGL specific
	CDC* m_pDC;
 	HGLRC m_hRC;
	int m_nBitsPerPixel;

	// Font
	COLORREF m_OptionColorText;
	LOGFONT m_LogFont;
	int m_nLogFontHeight;
	int m_nLogFontWidth;
	CFont m_FontSmall;
	CFont m_FontNormal;
	BOOL m_bFontCreateFlagSmall;
	BOOL m_bFontCreateFlagNormal;
	double m_dFontRatio;

protected:
	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;
	float m_ClearColorRedTmp;
	float m_ClearColorGreenTmp;
	float m_ClearColorBlueTmp;
	float m_AmbientColorRed;
	float m_AmbientColorGreen;
	float m_AmbientColorBlue;
	float m_DiffuseColorRed;
	float m_DiffuseColorGreen;
	float m_DiffuseColorBlue;
	float m_SpecularColorRed;
	float m_SpecularColorGreen;
	float m_SpecularColorBlue;
	float m_TextColorRed;
	float m_TextColorGreen;
	float m_TextColorBlue;
	float m_TextColorRedTmp;
	float m_TextColorGreenTmp;
	float m_TextColorBlueTmp;

	double m_dInvertNonRainbowColors;

	// OpenGL specific
	int m_nListBaseSmall;
	int m_nListBaseNormal;

	// Position, rotation ,scaling
	float m_xRotation;
	float m_yRotation;
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;
	float m_xScaling;
	float m_yScaling;
	float m_zScaling;
	float m_xRotationStore;
	float m_yRotationStore;

	// Rendering
	CPtrArray m_XRenderArray;
	CPtrArray m_YRenderArray;
	CPtrArray m_ZRenderArray;
	CPtrArray m_CRenderArray;
	CPtrArray m_SRenderArray;

	// Mouse
	HCURSOR m_hRotationCursor;
	CPoint m_LeftDownPos;
	BOOL m_bLastActivateWasDoneByLButton;

	GLdouble m_xMin;
	GLdouble m_yMin;
	GLdouble m_xMax;
	GLdouble m_yMax;

	CArray<CMatrix, CMatrix> m_ZoomHistory;

	BOOL m_bFirstPickPoint;
	BOOL m_bSecondPickPoint;

	CMatrix m_StartPoint;
	CMatrix m_EndPoint;

	// Size
	double m_aspect;
	CRect m_Rect;

	// Printing
	BOOL m_bStretchDIBits;
	BOOL m_bStretchBlt;
#if _MSC_VER < 1200
	BOOL m_bMDIMaximizeAndRestoreInPreview;
#endif // _MSC_VER < 1200

// Operations
public:
	// Position, rotation ,scaling
	virtual void InitGeometry();

	// Mouse
	virtual void AddToZoomHistory();
	virtual BOOL GetFromZoomHistory();

	// Text
	void OptionsFont();
	void ScaleFont(double scaleFac);
	void UnScaleFont();
	void CreateFontBitmaps();

	// Colors
	BOOL CheckTextColor(BOOL bPermanent = TRUE);
	void CreateRGBPalette();
	void PrepareBkgnd(BOOL bBegin);

	// Plotting
	void Plot2d(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC);
	void Plot2d(CMatrix& matX, CMatrix& matY, CMatrix& matC);
	void Plot2d(CMatrix* pXX, CMatrix* pYY);
	void Plot2d(CMatrix& matX, CMatrix& matY);
	void Plot2d(CMatrix* pYY);
	void Plot2d(CMatrix& matY);
	void Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC);
	void Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC);
	void Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ);
	void Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ);
	void Plot3d(CMatrix* pZZ);
	void Plot3d(CMatrix& matZ);
	void Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC);
	void Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC);
	void Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ);
	void Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ);
	void Surf(CMatrix* pZZ);
	void Surf(CMatrix& matZ);
	void TopView(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC);
	void TopView(CMatrix& matX, CMatrix& matY, CMatrix& matC);
	void TopView(CMatrix* pXX, CMatrix* pYY);
	void TopView(CMatrix& matX, CMatrix& matY);
	void TopView(CMatrix* pCC);
	void TopView(CMatrix& matC);
	void Scale2d(CScale* pSS);
	void Scale2d(CScale& matS);
	void Scale3d(CScale* pSS);
	void Scale3d(CScale& matS);

protected:
	virtual void OnDraw(CDC* pDC) = 0;

	// OpenGL specific
	virtual BOOL InitializeOpenGL();
	void SetOpenGLProperties();
	void SetBackColor();
	void SetAmbientColor();
	void SetDiffuseColor();
	void SetSpecularColor();
	BOOL SetWindowPixelFormat();
	BOOL SetMemDcPixelFormat(HDC hMemDC);
	BOOL CreateViewGLContext();
	BOOL MakeActive();
	void GetCurrent(HDC& hDC, HGLRC& hRC);
	BOOL SetCurrent(HDC hDC, HGLRC hRC); 
	GlInfoStruct GetOpenGlInfo();

	// Mouse
	void DrawZoomRect();
	void DrawRubberBand();

	// Rendering
	void RenderScenePlot2d();
	void RenderScenePlot3d();
	void RenderSceneSurf();
	void RenderSceneTopView();
	void RenderSceneScale2d();
	void RenderSceneScale3d();
	void MakeColorsRainbow(const double& fcol, double& r, double& g, double& b);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGlView();

// Generated message map functions
protected:
	//{{AFX_MSG(CGlView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnOptionsFont();
	afx_msg void OnHelpOpenglInfo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
