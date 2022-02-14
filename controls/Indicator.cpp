// Indicator.cpp : implement an indicator LED
//
// Copyright © 2001-2002, Mayn Idea, Inc.
// Copyright © 2001-2002, Metorex USA, Inc.
//
//////////////////////////////////////////////////////////////////////////
/*   $History: Indicator.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Morris       Date: 11/11/03   Time: 9:59a
 * Updated in $/mmlib/controls
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 10/29/03   Time: 11:43a
 * Created in $/mmlib/controls
*/
//////////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "Indicator.h"
#include "..\MemDC\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT_PTR CIndicator::m_uIdTimer = 0;
CList<CIndicator *, CIndicator *> CIndicator::m_arrIndics;
BOOL CIndicator::m_bFlashOn = 0;

/////////////////////////////////////////////////////////////////////////////
// CIndicator

CIndicator::CIndicator()
{
  m_BmpOff.LoadBitmap(IDB_BTNOFF);
  m_BmpRed.LoadBitmap(IDB_BTNRED);
  m_BmpGreen.LoadBitmap(IDB_BTNGRN);
  m_BmpYellow.LoadBitmap(IDB_BTNYEL);
  m_bChecked = -2;
}

CIndicator::~CIndicator()
{
}

BEGIN_MESSAGE_MAP(CIndicator, CButton)
  //{{AFX_MSG_MAP(CIndicator)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, OnDoubleclicked)
	ON_WM_LBUTTONDOWN()
  ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndicator message handlers

void CIndicator::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  CRect Rect = lpDrawItemStruct->rcItem;
  CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
  CRect rect;
  
  rect = Rect;
  
  UINT state = lpDrawItemStruct->itemState;
  
  CMemDC	*pMemDC = new CMemDC(pDC, Rect);
  CBrush hbr;
  hbr.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
  pMemDC->FillRect(&Rect, &hbr);
  hbr.DeleteObject();
  
  if(!(state & ODS_DISABLED))
    {
    if (state & ODS_SELECTED)
      {
      CPen pen(PS_SOLID, 2, GetSysColor(COLOR_3DHILIGHT));
      CPen *pPen;
      CPoint start, end;
      
      start.x = rect.left;
      start.y = rect.bottom;		
      end.x = rect.right;
      end.y = rect.top;
      
      rect.left += 2;
      rect.top += 2;
      rect.right -= 2;
      rect.bottom -= 2;
      
      pPen = pMemDC->SelectObject(&pen);
      pMemDC->Arc(&rect, start, end);
      pMemDC->SelectObject(pPen);
      pen.DeleteObject();
      }
    }

  CBitmap *pBmp = 0;

  if (IndicGrn == m_bChecked)
    pBmp = &m_BmpGreen;
  else if (IndicYel == m_bChecked)
    {
    if (m_bFlashOn)
      pBmp = &m_BmpYellow;
    else
      pBmp = &m_BmpOff;
    }
  else if (IndicRed == m_bChecked)
    pBmp = &m_BmpRed;
  else
    pBmp = &m_BmpOff;

//  CBitmap *pTmp = pMemDC->SelectObject(pBmp);
//
//  pDC->BitBlt(
//   rect.left,
//   rect.top,
//   rect.Width(),
//   rect.Height(),
//   pMemDC,
//   0, 0,   
//   SRCCOPY);

//  pMemDC->SelectObject(pTmp);

  pMemDC->DrawState(rect.TopLeft(), CSize(rect.Width(), rect.Height()), pBmp, DST_BITMAP|DSS_NORMAL, 0);
  
  CString title;
  GetWindowText(title);
  if(title.GetLength() != 0)
    {
    DrawWhiteText(pMemDC, &Rect, title);
    title.ReleaseBuffer();
    }

  delete pMemDC;
}

void CIndicator::DrawWhiteText(CDC* pDC, CRect* pRect, CString& text)
{
  CFont font;
  int nHeight = -16;
  
  font.CreateFont (nHeight, 0, 0, 0, FW_BOLD,
    TRUE, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
    CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
    FF_DONTCARE, "Times New Roman");
  
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(RGB (255, 255, 255));
  
  CFont* pOldFont = pDC->SelectObject (&font);
  pDC->DrawText (text, -1, pRect, DT_SINGLELINE | DT_CENTER |
    DT_VCENTER);
  
  pDC->SelectObject (pOldFont);
}

void CIndicator::PreSubclassWindow() 
{
  CButton::PreSubclassWindow();

  UINT uStyle = BS_OWNERDRAW;

	ModifyStyle(0, uStyle);

	CRect rect;
	GetClientRect(rect);

	// Resize the window to make it square
	rect.bottom = rect.right = min(rect.bottom,rect.right);

	// Set the window region so mouse clicks only activate the round section 
	// of the button
  SetWindowRgn(NULL, FALSE);
	HRGN hRgn = ::CreateEllipticRgnIndirect(rect);
	SetWindowRgn(hRgn, TRUE);
  DeleteObject(hRgn);

	// Convert client coords to the parents client coords
	ClientToScreen(rect);
	CWnd* pParent = GetParent();
	if (pParent) pParent->ScreenToClient(rect);

	// Resize the window
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

  m_arrIndics.AddTail(this);
  
}

void CIndicator::OnClicked() 
{
  // do nothing	
}

void CIndicator::OnDoubleclicked() 
{
	// do nothing
}

void CIndicator::OnLButtonDown(UINT, CPoint) 
{
	// do nothing
}

LRESULT CIndicator::OnSetCheck(WPARAM wParam, LPARAM)
{
  if (m_bChecked != wParam)
    {
    m_bChecked = (BOOL)wParam;
    Invalidate(FALSE);
    if (2 == m_bChecked)
      {
      if (!m_uIdTimer)
        m_uIdTimer = SetTimer((UINT_PTR)100U, 400, 0);
      }
    }
  return (LONG)Default();
}

void CIndicator::OnDestroy() 
{
  POSITION pos = m_arrIndics.Find(this);
  if (pos)
    m_arrIndics.RemoveAt(pos);
  if (!m_arrIndics.GetCount())
    {
    KillTimer(m_uIdTimer);
    m_uIdTimer = 0;
    }
  CButton::OnDestroy();
}

void CIndicator::OnTimer(UINT nIDEvent) 
{
	if (m_uIdTimer == nIDEvent)
    {
    m_bFlashOn = !m_bFlashOn;
    bool bAtLeastOne = false;
    // all instances of this indicator share one timer object
    for (POSITION pos = m_arrIndics.GetHeadPosition();pos != NULL;)
      {
      CIndicator *pInd = m_arrIndics.GetNext(pos);
      if (2 == pInd->GetCheck())
        {
        bAtLeastOne = true; // found a flasher
        pInd->InvalidateRect(0, 1);
        }
      }
    if (!bAtLeastOne) // if flashers are gone
      {
      KillTimer(m_uIdTimer); // don't need timer
      m_uIdTimer = 0;
      }
    }

	CButton::OnTimer(nIDEvent);
}
