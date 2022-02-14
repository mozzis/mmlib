/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: February 2000
//
// based on a sample of
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// DlgOpenglInfo.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_)
#define AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ICD_DRIVER_MASK  (PFD_GENERIC_ACCELERATED | PFD_GENERIC_FORMAT)
struct GlInfoStruct
{
	CString strGlVendor;
	CString strGlRenderer;
	CString strGlVersion;
	CString strGlAcceleration;
	CString strGlExtensions;
	CString strGluVersion;
	CString strGluExtensions;
	int nMaxLights;
	int nMaxClipPlanes;
	int nMaxModelStackDepth;
	int nMaxProjectionStackDepth;
	int nMaxTextureStackDepth;
	int nMaxNameStackDepth;
	int nMaxAttributeStackDepth;
	int nMaxTextureSize;
	int nMaxListNesting;
	int nMaxEvalOrder;
	CSize maxViewportDims;
	int nAuxiliaryBuffers;
	float minSmoothPointSize;
	float maxSmoothPointSize;
	float smoothPointGranularity;
	float minSmoothLineSize;
	float maxSmoothLineSize;
	float smoothLineGranularity;
	int nRedBits;
	int nGreenBits;
	int nBlueBits;
	int nAlphaBits;
	int nDepthBits;
	int nStencilBits;
};

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOpenglInfo 

class CDlgOpenglInfo : public CDialog
{
// Konstruktion
public:
	CDlgOpenglInfo(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgOpenglInfo)
	enum { IDD = IDD_DLG_OPENGL_INFO };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA
	GlInfoStruct m_info;


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgOpenglInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgOpenglInfo)
	afx_msg void OnDetails();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_
