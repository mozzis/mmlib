#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////
// Label.h : header file
// CLabel is a static label control with attribute settings,
// like VB's LAbel control
/*
* $History: Label.h $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/controls
// 
// *****************  Version 3  *****************
// User: Maynard      Date: 2/15/02    Time: 5:17p
// Updated in $/Metorex/C100sxt/1.1
// Correction to new SetupLabel function
// 
// *****************  Version 2  *****************
// User: Maynard      Date: 2/14/02    Time: 5:30p
// Updated in $/Metorex/C100sxt/1.1
// Add SetupLabel function
// 
// *****************  Version 1  *****************
// User: Maynard      Date: 8/14/01    Time: 6:07p
// Created in $/Metorex/CS100
 * 
 * *****************  Version 3  *****************
 * User: Maynard      Date: 3/02/00    Time: 8:37p
 * Updated in $/ZyluxZip
 * Make fixes for WM_SYSCOLORCHANGE bug
 * 
 * *****************  Version 2  *****************
 * User: Maynard      Date: 3/01/00    Time: 12:49p
 * Updated in $/ZyluxZip
 * Add class to project
 * 
*/
/////////////////////////////////////////////////////////////////////////////

enum FlashType {None, Text, Background };

class CMyLogFont : public LOGFONT
{
  public:
  CMyLogFont() 
    { 
    ::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),this);
    }
};

class CLabel : public CStatic
{
// Construction
public:
	CLabel();
	CLabel& SetBkColor(COLORREF crBkgnd);
	CLabel& SetTextColor(COLORREF crText);
	CLabel& SetText(const CString& strText);
	CLabel& SetFontBold(BOOL bBold);
	CLabel& SetFontName(const CString& strFont);
	CLabel& SetFontUnderline(BOOL bSet);
	CLabel& SetFontItalic(BOOL bSet);
	CLabel& SetFontSize(int nSize);
	CLabel& SetSunken(BOOL bSet);
	CLabel& SetBorder(BOOL bSet);
	CLabel& FlashText(BOOL bActivate);
	CLabel& FlashBackground(BOOL bActivate);
	CLabel& SetLink(BOOL bLink);
	CLabel& SetLinkCursor(HCURSOR hCursor);

  inline void SetUpLabel(LPCTSTR pszFontName, 
    int nFontSize = 14, BOOL bBold = TRUE, COLORREF clr = 0x00000000)
    {
    SetFontName(pszFontName);
    SetFontSize(nFontSize);
    SetFontBold(bBold);
    if (clr)
      SetTextColor(clr);
    }

// Attributes
public:
protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	CMyLogFont m_lf;
	CFont m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;
			// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
