/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: June 2000
//
// based on a sample of
// Chris Maunder <chrismaunder@codeguru.com>
//
/////////////////////////////////////////////////////////////////////////////
// HyperLink.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperLink window

class AFX_EXT_CLASS CHyperLink : public CStatic
{
// Konstruktion/Destruktion
public:
	CHyperLink();
	virtual ~CHyperLink();

// Attribute
public:

// Operationen
public:
	void SetURL(CString strURL);
	CString GetURL() const;

	void SetColors(COLORREF crLinkColor, COLORREF crVisitedColor, 
					COLORREF crHoverColor = -1);
	COLORREF GetLinkColor() const;
	COLORREF GetVisitedColor() const;
	COLORREF GetHoverColor() const;

	void SetVisited(BOOL bVisited = TRUE);
	BOOL GetVisited() const;

	void SetLinkCursor(HCURSOR hCursor);
	HCURSOR GetLinkCursor() const;

	void SetUnderline(BOOL bUnderline = TRUE);
	BOOL GetUnderline() const;

	void SetAutoSize(BOOL bAutoSize = TRUE);
	BOOL GetAutoSize() const;

	void SetDefaultCursor();
	HCURSOR& GetLinkCursor();
	CToolTipCtrl& GetToolTipCtrl();
	void Clicked();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHyperLink)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	BOOL IsOverControl();
	HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	void ReportError(int nError);
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void PositionWindow();

// Geschützte Attribute
protected:
	COLORREF		m_crLinkColor, m_crVisitedColor;	// Hyperlink Colors
	COLORREF		m_crHoverColor;						// Hover Color
	BOOL			m_bOverControl;						// cursor over control?
	BOOL			m_bVisited;							// Has it been visited?
	BOOL			m_bUnderline;						// underline hyperlink?
	BOOL			m_bAdjustToFit;						// Adjust window size to fit text?
	CString			m_strURL;							// hyperlink URL
	CFont			m_Font;								// Underline font if necessary
	HCURSOR			m_hLinkCursor;						// Cursor for hyperlink
	CToolTipCtrl	m_ToolTip;							// The tooltip

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnClicked();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
