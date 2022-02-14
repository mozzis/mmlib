// GridCWNumEdit.h: interface for the CGridCWNumEdit class.
// CGridCWNumEdit is a CGridCell which presents a CWNumericEdit control at edit time
// Copyright (C) 2003 Mayn Idea Software
// All rights reserved
/*
* $History: GridCWNumEdit.h $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 10/16/03   Time: 8:59p
 * Updated in $/Judson/PhotoTest/gridctrl
*/
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GridCWNum_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_)
#define AFX_GridCWNum_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class CGridCWNumEdit : public CGridCell  
{
  DECLARE_DYNCREATE(CGridCWNumEdit)
public:
	CGridCWNumEdit();
	virtual ~CGridCWNumEdit();
  virtual BOOL Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar);
    
protected:
};

#endif // !defined(AFX_GridCWNum_H__3FF259D7_A714_42C7_89E1_D34382CB61AB__INCLUDED_)
