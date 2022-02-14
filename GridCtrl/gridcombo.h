// GridCombo.h: interface for the CGridCombo class.
// CGridCombo is a CGridCell which presents a combo box at edit time
// Copyright (C) 2003 Mayn Idea Software
// All rights reserved
/*
* $History: gridcombo.h $ * 
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
 * *****************  Version 2  *****************
 * User: Morris       Date: 2/28/03    Time: 3:53p
 * Updated in $/Metorex/C100sxt/GridCtrl
 * Replace with more evolved versions from the Zylux Zip project
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 2/28/03    Time: 12:09p
 * Created in $/Metorex/C100sxt/GridCtrl
*/
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCOMBO_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_)
#define AFX_GRIDCOMBO_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class CGridCombo : public CGridCell  
{
  DECLARE_DYNCREATE(CGridCombo)
public:
	CGridCombo();
	virtual ~CGridCombo();
  virtual BOOL Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar);
  static void AddItem(LPCTSTR szItem);
  static void DelItem(LPCTSTR szItem);
  static void RemoveAll();
protected:
  static CStringArray m_Items;
};

#endif // !defined(AFX_GRIDCOMBO_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_)
