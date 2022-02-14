/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlWinApp.h : Header-Datei für alle OpenGl Windows Anwendungen
//

#if !defined(AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
#define AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'StdAfx.h' before including this file for PCH
#endif

#include "Resource.h"       // Hauptsymbole

// operating system
//	do not edit since we do 
//	maths upon these constants
typedef enum tagOSRUNNING 
{
	OSUnknown = 0,
	OSWin3 = 1, 
	OSWin95 = 2, 
	OSWin98 = 3, 
	OSWinNT = 4, 
	OSWin2K = 5 
}
OSRUNNING;

#define IDBACK								0

// properties
#define POLICY_NO_MORE_ZOOM					0
#define POLICY_MOUSE						1
#define POLICY_PROP							2
#define POLICY_REOPEN						3
#define POLICY_SAVE_GRAPH					4

// private window messages
#define WM_SHOW_PROGRESS					(WM_APP + 10001)
#define WM_ENABLE_SYSTEM					(WM_APP + 10002)
#define WM_DISABLE_SYSTEM					(WM_APP + 10003)
#define WM_ADJUST_PROPERTY_SHEET			(WM_APP + 10004)
#define WM_CALCULATE_IN_THREAD_DONE			(WM_APP + 10005)
#define WM_DOC_WAS_MODIFIED_BY_THREAD		(WM_APP + 10006)

class AFX_EXT_CLASS CGlWinApp : public CWinApp
{
	DECLARE_DYNAMIC(CGlWinApp)
public:
	CGlWinApp();

// Attributes
	int m_nSerializeGraphBytes;
	int m_nMaxPrintMemSize;
	int m_nPrintObject;
	BOOL m_bPrintBkgndWhite;

	OSRUNNING m_nOsRunning;

// Options
	BOOL m_bPropPolicy;
	BOOL m_bReopenPolicy;
	BOOL m_bBlackLinesOrRemoveHidden;
	BOOL m_bScaleShowFrame;
	BOOL m_bScaleShowLabels;
	BOOL m_bScaleShowGrid;
	BOOL m_bScaleShowAxis;

// Mouse
	int m_nMousePolicy;

// OpenGl stuff
	COLORREF m_OptionColorGlBack;
	COLORREF m_OptionColorGlLightAmbient;
	COLORREF m_OptionColorGlLightDiffuse;
	COLORREF m_OptionColorGlLightSpecular;
	COLORREF m_OptionColorText;
	LOGFONT m_LogFont;

	BOOL	m_bLighting;
	BOOL	m_bSmooth;
	BOOL	m_bAntialias;
	int		m_nPolygonMode;

// Printing
	BYTE* m_pBitmapBits;
	CRect m_Rect;
	BITMAPINFO m_bitmapInfo;

// Registry keys
	virtual BOOL LoadOptions();
	virtual BOOL SaveOptions();
	virtual void RegisterShellFileTypes(BOOL bCompat = FALSE, BOOL bOmitPrintCmd = FALSE);

// Operations
	BOOL DoPromptFileName(CFileDialog& dlgFile, UINT nIDSTitle, DWORD lFlags, CDocTemplate* pTemplate);
	void DoFilePrintAll();
	BOOL BeginPrinting(CDC* pDC);
	BOOL Print(CDC* pDC);
	BOOL EndPrinting(CDC* pDC);
	void SetStatusBarInfo(int nInfoMajor, int nInfoMinor = -1);
protected:
	static BOOL CALLBACK AbortProc(HDC hdc, int iCode);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGlWinApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGlWinApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnHelpContents();
	afx_msg void OnFilePrintSetupEx();
	afx_msg void OnChangeMousePolicy();
	afx_msg void OnChangePropPolicy();
	afx_msg void OnChangeReopenPolicy();
	afx_msg void OnSerializeGraphOneByte();
	afx_msg void OnSerializeGraphTwoBytes();
	afx_msg void OnSerializeGraphFourBytes();
	afx_msg void OnSerializeGraphEightBytes();
	afx_msg void OnChangePolicy();
	afx_msg void OnFilePrintEx();
	afx_msg void OnFilePrintPreviewEx();
	afx_msg void OnUpdateFilePrintEx(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreviewEx(CCmdUI* pCmdUI);
	//}}AFX_MSG
	//afx_msg void OnShowPolicy();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
