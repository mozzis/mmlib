// GridCWNumEdit.cpp: implementation of the CGridCWNumEdit class.
// CGridCWNumEdit is a CGridCell which presents a CWNumEdit control at edit time
// Copyright (C) 2000 Mayn Idea Software
// All rights reserved
/*
* $History: GridCWNumEdit.cpp $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 10/16/03   Time: 8:59p
 * Updated in $/Judson/PhotoTest/gridctrl
 * 
*/
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCWNumEdit.h"
#include "InPlaceCWNumEdit.h"

#ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CGridCWNumEdit, CGridCell)

CGridCWNumEdit::CGridCWNumEdit()
{
}

CGridCWNumEdit::~CGridCWNumEdit()
{
}

BOOL CGridCWNumEdit::Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar)
{
  m_bEditing = TRUE;
  // InPlaceEdit auto-deletes itself
  DWORD dwStyle = WS_VISIBLE|WS_CHILD;
  m_pEditWnd = (CWnd *)
    new CInPlaceCWNumEdit(m_pGrid, rect, dwStyle, nID, nRow, nCol, nChar);
  return TRUE;
}
