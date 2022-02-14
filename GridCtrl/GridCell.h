#if !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GridCell.h : header file
//
// MFC Grid Control - Grid cell base class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2003. All Rights Reserved.
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
* $History: GridCell.h $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 3  *****************
 * User: Morris       Date: 10/16/03   Time: 8:59p
 * Updated in $/Judson/PhotoTest/gridctrl
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 10/13/03   Time: 12:08a
 * Updated in $/Judson/PhotoTest/gridctrl
 * Added cell grouping
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 2/28/03    Time: 3:53p
 * Updated in $/Metorex/C100sxt/GridCtrl
 * Replace with more evolved versions from the Zylux Zip project
 * 
*/
/////////////////////////////////////////////////////////////////////////////

class CGridCtrl;

typedef enum eGroupType { notGrp, beginGrp, inGrp, endGrp };
// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCell : public CObject
{
    DECLARE_DYNCREATE(CGridCell)
    friend class CGridCtrl;

// Construction/Destruction
public:
    CGridCell();
    virtual ~CGridCell();

// Attributes
public:
    inline void SetText(LPCTSTR szText);
    inline void SetImage(int nImage);
    inline void SetData(UINT_PTR lParam);
    inline void SetState(DWORD nState);
    inline void SetFormat(DWORD nFormat);
    inline void SetTextClr(COLORREF clr);
    inline void SetBackClr(COLORREF clr);
    inline void SetFont(const LOGFONT* plf);
    inline void SetGrid(CGridCtrl* pGrid);
    inline void SetOneClickEdit(bool bOneClick) { m_bOneClickEdit = bOneClick; }
    inline void SetGrouped(eGroupType eGrp) { m_eGroup = eGrp; }

    LPCTSTR  GetText()      { return (m_strText.IsEmpty())? _T("") : m_strText; }
    int      GetImage()     { return m_nImage;  }
    UINT_PTR GetData()      { return m_lParam;  }
    DWORD    GetState()     { return m_nState;  }
    DWORD    GetFormat()    { return m_nFormat; }
    COLORREF GetTextClr()   { return m_crFgClr; }
    COLORREF GetBackClr()   { return m_crBkClr; }
    LOGFONT* GetFont()      { return &m_lfFont; }
    CGridCtrl* GetGrid()    { return m_pGrid;   }
    bool GetOneClickEdit()  { return m_bOneClickEdit; }
    eGroupType GetGrouped() { return m_eGroup; }
    
    BOOL     IsEditing()    { return m_bEditing; }
    virtual void Reset();

    inline void operator=(CGridCell& cell);

// Operations
public:
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar);
    virtual void EndEdit();
    virtual CSize GetTextExtent(LPCTSTR str);
    virtual CSize GetCellExtent(CDC* pDC);

    // EFW - Added to print cells properly
    virtual BOOL PrintCell(CDC* pDC, int nRow, int nCol, CRect rect);

protected:
    virtual void OnEndEdit();
    virtual void OnMouseEnter();
    virtual void OnMouseOver();
    virtual void OnMouseLeave();
    virtual void OnClick();
    virtual void OnDblClick();
    virtual BOOL OnSetCursor();

protected:
    DWORD    m_nState;      // Cell state (selected/focus etc)
    DWORD    m_nFormat;     // Cell format
    CString  m_strText;     // Cell text (or binary data if you wish...)
    int      m_nImage;      // Index of the list view item�s icon
    COLORREF m_crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORREF m_crFgClr;     // Forground colour (or CLR_DEFAULT)
    UINT_PTR m_lParam;      // 32-bit value to associate with item
    LOGFONT  m_lfFont;      // Cell font
    UINT     m_nMargin;     // Internal cell margin

    BOOL     m_bEditing;    // Cell being edited?
    bool     m_bOneClickEdit;
    eGroupType m_eGroup;

    CGridCtrl* m_pGrid;     // Parent grid control
    CWnd* m_pEditWnd;
};

/////////////////////////////////////////////////////////////////////////////
// Inlined functions

inline void CGridCell::SetText(LPCTSTR szText)
{
    m_strText = szText;
}

inline void CGridCell::SetImage(int nImage)
{
    m_nImage = nImage;
}

inline void CGridCell::SetData(UINT_PTR lParam)
{
    m_lParam = lParam;
}

inline void CGridCell::SetState(DWORD nState)
{
    m_nState = nState;
}

inline void CGridCell::SetFormat(DWORD nFormat)
{
    m_nFormat = nFormat;
}

inline void CGridCell::SetTextClr(COLORREF clr)
{
    m_crFgClr = clr;
}

inline void CGridCell::SetBackClr(COLORREF clr)
{
    m_crBkClr = clr;
}

inline void CGridCell::SetFont(const LOGFONT* plf)
{
    memcpy(&(m_lfFont), plf, sizeof(LOGFONT));

    // Calculate the margin size
    /*
    CWindowDC dc(NULL);
    CFont font;
    font.CreateFontIndirect(plf);
    CFont* pOldFont = dc.SelectObject(&font);
    m_nMargin = dc.GetTextExtent(_T(" "),1).cx;
    dc.SelectObject(pOldFont);
    */
}

inline void CGridCell::SetGrid(CGridCtrl* pGrid)
{
    m_pGrid = pGrid;
}

void CGridCell::operator=(CGridCell& cell)
{
    SetText(cell.GetText());
    SetImage(cell.GetImage());
    SetData(cell.GetData());
    SetState(cell.GetState());
    SetFormat(cell.GetFormat());
    SetTextClr(cell.GetTextClr());
    SetBackClr(cell.GetBackClr());
    SetFont(cell.GetFont());
    SetGrid(cell.GetGrid());
    SetOneClickEdit(cell.m_bOneClickEdit);
    SetGrouped(cell.m_eGroup);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
