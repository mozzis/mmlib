#if !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// InPlaceList.h : header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2003. All Rights Reserved.
//
// The code contained in this file is based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/*
* $History: inplacelist.h $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/09/03   Time: 4:23p
 * Created in $/Judson/PhotoTest/gridctrl
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 2/28/03    Time: 3:53p
 * Updated in $/Metorex/C100sxt/GridCtrl
 * Replace with more evolved versions from the Zylux Zip project
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 2/28/03    Time: 12:09p
 * Created in $/Metorex/C100sxt/GridCtrl
// 
// *****************  Version 4  *****************
// User: Maynard      Date: 11/14/00   Time: 1:01p
// Updated in $/ZyluxZip/Grid
// Add DECLARE/IMPLEMENT_DYNAMIC macros so can do runtime type checking
// Make EndEdit public, matches InPlaceEdit and can call from grid
 * 
 * *****************  Version 3  *****************
 * User: Maynard      Date: 3/22/00    Time: 10:44p
 * Updated in $/ZyluxZip/Grid
 * Add handler for combo box close-up - so when new site name is chosen,
 * the other columns get updated right away
 * 
 * *****************  Version 2  *****************
 * User: Maynard      Date: 2/27/00    Time: 5:12p
 * Updated in $/ZyluxZip/Grid
 * Add copyright and Source Safe headers
*/
/////////////////////////////////////////////////////////////////////////////

#define IDC_COMBOEDIT 1001

/////////////////////////////////////////////////////////////////////////////
// CComboEdit window

class CComboEdit : public CEdit
{
// Construction
public:
	CComboEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList window

class CInPlaceList : public CComboBox
{
  DECLARE_DYNAMIC(CInPlaceList)

  friend class CComboEdit;

// Construction
public:
	CInPlaceList(CWnd* pParent,         // parent
                 CRect& rect,           // dimensions & location
                 DWORD dwStyle,         // window/combobox style
                 UINT nID,              // control ID
                 int nRow, int nColumn, // row and column
				 CStringArray& Items,   // Items in list
                 CString sInitText,     // initial selection
				 UINT nFirstChar);      // first character to pass to control

// Attributes
public:
   CComboEdit m_edit;  // subclassed edit control

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceList)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceList();
  void EndEdit();

// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int		m_nNumLines;
	CString m_sInitText;
	int		m_nRow;
	int		m_nCol;
 	UINT    m_nLastChar; 
	BOOL	m_bExitOnArrows; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
