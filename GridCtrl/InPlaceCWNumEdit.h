// InPlaceCWNumEdit.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#include "VisValEdit.h"

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCWNumEdit

class CInPlaceCWNumEdit : public CVisValidEdit
{
	DECLARE_DYNAMIC(CInPlaceCWNumEdit)
// Construction
public: 
  CInPlaceCWNumEdit(class CGridCtrl* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                   int nRow, int nColumn, UINT nFirstChar);
  
  virtual ~CInPlaceCWNumEdit();

// Operations
public:
  void SetFieldWidth(int nWid) { m_nFieldWidth = nWid; }

  void EndEdit();
  
  
// Generated message map functions
protected:
  //{{AFX_MSG(CInPlaceCWNumEdit)
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PostNcDestroy();
  
// Attributes
private:
  int m_nRow, m_nColumn;
  int m_nFieldWidth;
  CString m_sInitText;
  bool m_bExitOnArrows;
  CRect m_Rect;
  UINT m_nLastChar;
};
