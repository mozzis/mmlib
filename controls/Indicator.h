#if !defined(AFX_MYBTN_H__289CB976_3A41_11D1_9774_004033298A13__INCLUDED_)
#define AFX_MYBTN_H__289CB976_3A41_11D1_9774_004033298A13__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Indicator.h : header file
//

const int IndicOff = 0; // off
const int IndicGrn = 1; // green
const int IndicYel = 2; // flashing yellow
const int IndicRed = 3; // Red;

#define IDB_BTNOFF 29000
#define IDB_BTNRED 29001
#define IDB_BTNGRN 29002
#define IDB_BTNYEL 29003

#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CIndicator window
class CIndicator : public CButton
{
// Construction
public:
	CIndicator();

// Attributes
public:

// Operations
public:
	void DrawWhiteText(CDC* pDC, CRect* pRect, CString& text );
	void GradientFill (CDC* pDC, CRect* pRect);
  void SetCheck(int nCheck) { OnSetCheck(nCheck, 0); }
  int GetCheck(void ) { return m_bChecked; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndicator)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndicator();
protected:
  CBitmap m_BmpOff, m_BmpRed, m_BmpGreen, m_BmpYellow;
  BOOL m_bChecked;
  static BOOL m_bFlashOn;
  static UINT_PTR m_uIdTimer;
  static CList<CIndicator *, CIndicator *> m_arrIndics;
	// Generated message map functions
protected:
	//{{AFX_MSG(CIndicator)
	afx_msg void OnClicked();
	afx_msg void OnDoubleclicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg LONG OnSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBTN_H__289CB976_3A41_11D1_9774_004033298A13__INCLUDED_)
