/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// ClientFrame.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTFRM_H__9F201E20_8B54_11D1_87E4_00AA00242F4F__INCLUDED_)
#define AFX_CLIENTFRM_H__9F201E20_8B54_11D1_87E4_00AA00242F4F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HyperLink.h"

/////////////////////////////////////////////////////////////////////////////
// CClientFrame frame

class AFX_EXT_CLASS CClientFrame : public CWnd
{
	DECLARE_DYNCREATE(CClientFrame)
public:
	CClientFrame();

// Attribute
public:
	BOOL m_bTile;
	LPPOINT m_pTopLeft;
	CPalette m_Palette;
	CBitmap m_Bitmap;

// Operationen
public:
	BOOL GetBitmapAndPalette(UINT nIDResource);
	void SetURL(CString strURL);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CClientFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CClientFrame();
protected:
	BOOL m_bLButtonDownInBitmap;
	CHyperLink m_HyperLink;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CClientFrame)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_CLIENTFRM_H__9F201E20_8B54_11D1_87E4_00AA00242F4F__INCLUDED_)
