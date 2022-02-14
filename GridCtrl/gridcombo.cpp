// GridCombo.cpp: implementation of the CGridCombo class.
// CGridCombo is a CGridCell which presents a combo box at edit time
// Copyright (C) 2000 Mayn Idea Software
// All rights reserved
/*
* $History: gridcombo.cpp $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 10/15/03   Time: 1:51a
 * Updated in $/Judson/PhotoTest/gridctrl
 * Wow
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 2/28/03    Time: 12:09p
 * Created in $/Metorex/C100sxt/GridCtrl
// 
// *****************  Version 2  *****************
// User: Maynard      Date: 11/15/00   Time: 8:30a
// Updated in $/ZyluxZip/Grid
// Remove un-needed include of zyluxziph
// Change include path to reflect relocation of this module to the grid
// project
// 
// *****************  Version 1  *****************
// User: Maynard      Date: 11/14/00   Time: 12:37p
// Created in $/ZyluxZip/Grid
 * 
 * *****************  Version 3  *****************
 * User: Maynard      Date: 3/18/00    Time: 5:32p
 * Updated in $/ZyluxZip
 * 
 * *****************  Version 2  *****************
 * User: Maynard      Date: 3/15/00    Time: 11:52a
 * Updated in $/ZyluxZip
 * Add code to produce the combo box for editing and to add and delete
 * items
*/
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCombo.h"
#include "inplacelist.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringArray CGridCombo::m_Items;

IMPLEMENT_DYNCREATE(CGridCombo, CGridCell)

CGridCombo::CGridCombo()
{
}

CGridCombo::~CGridCombo()
{

}

void CGridCombo::RemoveAll()
{
  m_Items.RemoveAll();
}

void CGridCombo::AddItem(LPCTSTR szItem)
{
  m_Items.Add(szItem);
}

void CGridCombo::DelItem(LPCTSTR szItem)
{
  for (int i = 0; i < m_Items.GetSize(); i++)
    {
    CString csItem = m_Items.GetAt(i);
    if (csItem == szItem)
      {
      m_Items.RemoveAt(i);
      break;
      }
    }
}

BOOL CGridCombo::Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar)
{
    DWORD dwStyle = ES_LEFT;
    if (m_nFormat & DT_RIGHT) 
        dwStyle = ES_RIGHT;
    else if (m_nFormat & DT_CENTER) 
        dwStyle = ES_CENTER;

    dwStyle |= CBS_DROPDOWNLIST;
    m_bEditing = TRUE;
    // InPlaceEdit auto-deletes itself
    m_pEditWnd = new CInPlaceList((CWnd *)m_pGrid, rect, dwStyle, nID, nRow, nCol, m_Items, m_strText, nChar);
    return TRUE;
}
