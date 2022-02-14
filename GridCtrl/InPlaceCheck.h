// InPlaceCheck.h: interface for the CInPlaceCheck class.
// CInPlaceCheck is a CGridCell which displays a check box
// Copyright (C) 2000 Mayn Idea Software
// All rights reserved
/*
* $History: InPlaceCheck.h $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/GridCtrl
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/13/03   Time: 1:29a
 * Created in $/Judson/PhotoTest/gridctrl
 * Add to project
 * 
 * *****************  Version 4  *****************
 * User: Maynard      Date: 3/23/00    Time: 11:07a
 * Updated in $/ZyluxZip
 * Remove unused override of GetCellExtent
 * 
 * *****************  Version 3  *****************
 * User: Maynard      Date: 3/15/00    Time: 11:53a
 * Updated in $/ZyluxZip
 * Add m_nRow and m_nCol members and pick up values in OnDraw
 * Get sense of m_bChecked right in OnDraw
 * Add OnClicked notifier
*/
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPLACECHECK_H__5D57FF17_8A48_4509_9643_6CE5C49EEA34__INCLUDED_)
#define AFX_INPLACECHECK_H__5D57FF17_8A48_4509_9643_6CE5C49EEA34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class CInPlaceCheck : public CGridCell
{
  DECLARE_DYNCREATE(CInPlaceCheck)
public:    

public:
	CInPlaceCheck();
	virtual ~CInPlaceCheck();
  bool GetChecked() { return m_bChecked; }
  void SetChecked(bool bChecked) { m_bChecked = bChecked; }
protected:
  bool m_bChecked;
  int m_nRow, m_nCol;
  virtual BOOL Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar);
  virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
  virtual BOOL PrintCell(CDC* pDC, int nRow, int Col, CRect rect);
  virtual void OnClick();
};

#endif // !defined(AFX_INPLACECHECK_H__5D57FF17_8A48_4509_9643_6CE5C49EEA34__INCLUDED_)
