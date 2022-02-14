// InPlaceCheck.cpp: implementation of the CInPlaceCheck class.
// CInPlaceCheck is a CGridCell which displays a check box
// Copyright (C) 2000 Mayn Idea Software
// All rights reserved
/*
* $History: InPlaceCheck.cpp $ * 
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
 * *****************  Version 6  *****************
 * User: Maynard      Date: 3/23/00    Time: 11:07a
 * Updated in $/ZyluxZip
 * Remove unused override of GetCellExtent
 * 
 * *****************  Version 5  *****************
 * User: Maynard      Date: 3/15/00    Time: 11:53a
 * Updated in $/ZyluxZip
 * Add m_nRow and m_nCol members and pick up values in OnDraw
 * Get sense of m_bChecked right in OnDraw
 * Add OnClicked notifier
*/
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ZyluxZip.h"
#include "InPlaceCheck.h"
#include "GridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Font-attribute mask  
 
#define ATTRIBMASK 0xe 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CInPlaceCheck, CGridCell)

CInPlaceCheck::CInPlaceCheck()
{
  m_bChecked = 0;
  m_nRow = m_nCol = 0;
}

CInPlaceCheck::~CInPlaceCheck()
{

}

//
/////////////////////////////////////////////////////////////////////////////
BOOL CInPlaceCheck::Edit(int nRow, int nCol, CRect rect, UINT nID, UINT nChar)
{
    DWORD dwStyle = ES_LEFT;
    if (m_nFormat & DT_RIGHT) 
        dwStyle = ES_RIGHT;
    else if (m_nFormat & DT_CENTER) 
        dwStyle = ES_CENTER;

    m_bEditing = TRUE;
    
    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
BOOL CInPlaceCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /*=TRUE*/)
{
  if (!m_pGrid || !pDC)
    return FALSE;

  m_nRow = nRow, m_nCol = nCol;

  BOOL bDrawFixed = ((GetState() & GVIS_FIXED) == GVIS_FIXED);
  
  int nSavedDC = pDC->SaveDC();
  pDC->SetBkMode(TRANSPARENT);
  
  // Set up text and background colours
  COLORREF TextClr, TextBkClr;
  if (m_crFgClr == CLR_DEFAULT)
    TextClr = (bDrawFixed)? m_pGrid->GetFixedTextColor() : m_pGrid->GetTextColor();
  else
    TextClr = m_crFgClr;
  
  if (m_crBkClr == CLR_DEFAULT)
    TextBkClr = (bDrawFixed)? m_pGrid->GetFixedBkColor() : m_pGrid->GetTextBkColor();
  else
    {
    bEraseBkgnd = TRUE;
    TextBkClr = m_crBkClr;
    }
  
  // Draw cell background and highlighting (if necessary)
  if((m_nState & GVIS_FOCUSED) || (m_nState & GVIS_DROPHILITED))
    {
    // Always draw even in list mode so that we can tell where the
    // cursor is at.  Use the highlight colors though.
    if(m_nState & GVIS_SELECTED)
      {
      TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
      TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
      bEraseBkgnd = TRUE;
      }
    
    rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
    if (bEraseBkgnd)
      {
      CBrush brush(TextBkClr);
      pDC->FillRect(rect, &brush);
      }
    
    // Don't adjust frame rect if no grid lines so that the
    // whole cell is enclosed.
    if(m_pGrid->GetGridLines() != GVL_NONE)
      {
      rect.right--;
      rect.bottom--;
      }
    
    // Use same color as text to outline the cell so that it shows
    // up if the background is black.
    CBrush brush(TextClr);
    pDC->FrameRect(rect, &brush);
    
    // Adjust rect after frame draw if no grid lines
    if(m_pGrid->GetGridLines() == GVL_NONE)
      {
      rect.right--;
      rect.bottom--;
      }
    rect.DeflateRect(1,1);
    }
  else if ((m_nState & GVIS_SELECTED))
    {
    rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
    pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
    rect.right--; rect.bottom--;
    }
  else
    {
    if (bEraseBkgnd)
      {
      rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
      CBrush brush(TextBkClr);
      pDC->FillRect(rect, &brush);
      rect.right--; rect.bottom--;
      }
    }
  
  // If fixed, draw fixed cell highlighting
  
  // Draw lines only when wanted
  if (bDrawFixed && m_pGrid->GetGridLines() != GVL_NONE)
    {
    CCellID FocusCell = m_pGrid->GetFocusCell();
    
    // As above, always show current location even in list mode so
    // that we know where the cursor is at.
    BOOL bHiliteFixed = m_pGrid->IsValid(FocusCell) &&
      (FocusCell.row == nRow || FocusCell.col == nCol);
    
    // If this fixed cell is on the same row/col as the focus cell,
    // highlight it.
    if (bHiliteFixed)
      {
      rect.right++; rect.bottom++;
      pDC->DrawEdge(rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
      rect.DeflateRect(1,1);
      }
    else
      {
      CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
        darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
        *pOldPen = pDC->GetCurrentPen();
      
      pDC->SelectObject(&lightpen);
      pDC->MoveTo(rect.right, rect.top);
      pDC->LineTo(rect.left, rect.top);
      pDC->LineTo(rect.left, rect.bottom);
      
      pDC->SelectObject(&darkpen);
      pDC->MoveTo(rect.right, rect.top);
      pDC->LineTo(rect.right, rect.bottom);
      pDC->LineTo(rect.left, rect.bottom);
      
      pDC->SelectObject(pOldPen);
      rect.DeflateRect(1,1);
      }
    }
  // Draw image
  rect.DeflateRect(m_nMargin, 0);
  
  if (m_pGrid->GetImageList() && m_nImage >= 0)
    {
    IMAGEINFO Info;
    if (m_pGrid->GetImageList()->GetImageInfo(m_nImage, &Info))
      {
      int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
      m_pGrid->GetImageList()->Draw(pDC, m_nImage, rect.TopLeft(), ILD_NORMAL);
      rect.left += nImageWidth+m_nMargin;
      }
    }
  
  // Create a memory device context for the source bitmap
  CDC dcSource;
  dcSource.CreateCompatibleDC(0); 
  
  // Get the size of the system default check-mark bitmap and 
  // create a compatible bitmap of the same size. 
  WORD wBitmapX = GetSystemMetrics(SM_CXMENUCHECK); // width of check-mark bitmap 
  WORD wBitmapY = GetSystemMetrics(SM_CYMENUCHECK); // height of check-mark bitmap
  CBitmap bmpCheck;
  bmpCheck.CreateCompatibleBitmap(&dcSource, wBitmapX, wBitmapY); 
  
  // Select the bitmap into the target DC. 
  CBitmap *pBmpTargetOld = pDC->SelectObject(&bmpCheck); 
  
  // Load the predefined check box bitmap and select it 
  // into the source DC. 
  CBitmap bmpCheckboxes;
  bmpCheckboxes.LoadOEMBitmap(OBM_CHECKBOXES); 
  CBitmap *pBmpSourceOld = dcSource.SelectObject(&bmpCheckboxes); 
  
  // Fill a BITMAP structure with information about the 
  // check box bitmaps, and then find the upper-left corner of 
  // the unchecked check box or the checked check box. 
  BITMAP bmCheckbox;      // structure for bitmap data         
  bmpCheckboxes.GetBitmap(&bmCheckbox); 
  
  CRect rc;
  if (!m_bChecked) 
    { 
    rc.left = 0; 
    rc.right = (bmCheckbox.bmWidth / 4); 
    } 
  else 
    { 
    rc.left = (bmCheckbox.bmWidth / 4); 
    rc.right = (bmCheckbox.bmWidth / 4) * 2; 
    } 
  
  rc.top = 0; 
  rc.bottom = (bmCheckbox.bmHeight / 3); 

  int orgX = rect.CenterPoint().x - (wBitmapX / 2);
  int orgY = rect.CenterPoint().y - (wBitmapY / 2);
  // Copy the appropriate bitmap into the target DC. If the 
  // check-box bitmap is larger than the default check-mark 
  // bitmap, use StretchBlt to make it fit; otherwise, just 
  // copy it. 
  if (((rect.Width()) > (int) wBitmapX) || ((rect.Height()) > (int) wBitmapY)) 
    {
    pDC->StretchBlt(orgX, orgY, wBitmapX, wBitmapY, 
      &dcSource, rc.left, rc.top, rc.Width(), rc.Height(), SRCCOPY); 
    }
  else 
    {
    pDC->BitBlt(orgX, orgY, wBitmapX, wBitmapY, 
      &dcSource, rc.left, rc.top, SRCCOPY); 
    }
  
  // Select the old source and destination bitmaps into the 
  // source and destination DCs, and return a handle to the 
  // new check-mark bitmap
  dcSource.SelectObject(pBmpSourceOld); 
  pDC->SelectObject(pBmpTargetOld);
  pDC->RestoreDC(nSavedDC);

  return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
BOOL CInPlaceCheck::PrintCell(CDC* pDC, int nRow, int nCol, CRect rect)
{
  return Draw(pDC, nRow, nCol, rect, FALSE);
}

//
/////////////////////////////////////////////////////////////////////////////
void CInPlaceCheck::OnClick()
{
  m_bChecked = !m_bChecked;
  m_pGrid->SetModified(TRUE);
  m_pGrid->RedrawCell(m_nRow, m_nCol);

  // Send Notification to parent
  GV_DISPINFO dispinfo;

  dispinfo.hdr.hwndFrom = GetGrid()->GetSafeHwnd();
  dispinfo.hdr.idFrom   = 0;
  dispinfo.hdr.code     = GVN_COLUMNCLICK;
  dispinfo.item.mask    = LVIF_STATE;
  dispinfo.item.lParam  = LVIS_SELECTED;
  dispinfo.item.row     = m_nRow;
  dispinfo.item.col     = m_nCol;
  CWnd* pOwner = GetGrid();
  if (pOwner)
      pOwner->SendMessage(WM_NOTIFY, 0, (LPARAM)&dispinfo);
}
