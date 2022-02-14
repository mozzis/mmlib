// InPlaceCWNumEdit.cpp  : Definition of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#include "stdafx.h"
#include "GridCtrl.h"
#include "InPlaceCWNumEdit.h"
#include "EditItem.h"

#if _MSC_VER < 1300
#define __super CWnd
#endif

/////////////////////////////////////////////////////////////////////////////
// InPlaceCWNumEdit

IMPLEMENT_DYNAMIC(CInPlaceCWNumEdit, CWnd)

CInPlaceCWNumEdit::CInPlaceCWNumEdit(CGridCtrl* pWnd, CRect& rect, DWORD dwStyle, UINT nID,
                                     int nRow, int nCol, UINT nFirstChar)
{
    m_nRow          = nRow;
    m_nColumn       = nCol;
    m_nLastChar     = 0; 
    m_nFieldWidth   = -1;
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
    m_Rect = rect;  // For bizarre CE bug.
    
    // InPlaceEdit auto-deletes itself
    dwStyle |= WS_VISIBLE|WS_CHILD;
    if(Create(dwStyle, rect, pWnd, nID))
    {
      EDITITEM *pItem = (EDITITEM *)pWnd->GetItemData(nRow, nCol);
      if (eDouble == pItem->eType)
        SetValidCharSet(SET_FLOAT);
      else
        SetValidCharSet(SET_HEXADECIMAL);

      m_nFieldWidth = pItem->nFieldWidth;
      
      CGridCell *pCell = pWnd->GetCell(nRow, nCol);
      LPCTSTR pTxt = pCell->GetText();
      SetFont(pWnd->GetFont());    
      SetWindowText(pTxt);
      SetFocus();
      
      switch (nFirstChar)
      {
        case VK_LBUTTON: 
        case VK_RETURN:   SetSel((int)_tcslen(m_sInitText), -1); return;
        case VK_BACK:     SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_TAB:
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:
        case VK_SPACE:
        case VK_END:      SetSel(0,-1); return;
        default:          SetSel(0,-1);
      }
      SendMessage(WM_CHAR, nFirstChar);
    }
}

CInPlaceCWNumEdit::~CInPlaceCWNumEdit()
{
}

BEGIN_MESSAGE_MAP(CInPlaceCWNumEdit, CWnd)
  //{{AFX_MSG_MAP(CInPlaceCWNumEdit)
  ON_WM_KILLFOCUS()
  ON_WM_CHAR()
  ON_WM_KEYDOWN()
  ON_WM_GETDLGCODE()
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CInPlaceCWNumEdit properties

// CInPlaceCWNumEdit operations

////////////////////////////////////////////////////////////////////////////
// CInPlaceCWNumEdit message handlers

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceCWNumEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
      nChar == VK_DOWN  || nChar == VK_UP   ||
      nChar == VK_RIGHT || nChar == VK_LEFT) &&
      (m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
  {
    m_nLastChar = nChar;
    GetParent()->SetFocus();
    return;
  }
  __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

// As soon as this edit loses focus, kill it.
void CInPlaceCWNumEdit::OnKillFocus(CWnd* pNewWnd)
{
  __super::OnKillFocus(pNewWnd);
  EndEdit();
}

void CInPlaceCWNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == VK_TAB || nChar == VK_RETURN)
  {
    m_nLastChar = nChar;
    GetParent()->SetFocus();    // This will destroy this window
    return;
  }
  if (nChar == VK_ESCAPE) 
  {
    // restore previous text
    SetWindowText(m_sInitText);    // restore previous text
    m_nLastChar = nChar;
    GetParent()->SetFocus();
    return;
  }
  
  __super::OnChar(nChar, nRepCnt, nFlags);
  
  // Resize edit control if needed
  
  // Get text extent
  CString str;
  GetWindowText( str );

  // add some extra buffer
  str += _T("  ");
  
  CWindowDC dc(this);
  CFont *pFontDC = dc.SelectObject(GetFont());
  CSize size = dc.GetTextExtent( str );
  dc.SelectObject( pFontDC );
     
  // Get client rect
  CRect ParentRect;
  GetParent()->GetClientRect( &ParentRect );
  
  // Check whether control needs to be resized
  // and whether there is space to grow
  if (size.cx > m_Rect.Width())
  {
      if( size.cx + m_Rect.left < ParentRect.right )
          m_Rect.right = m_Rect.left + size.cx;
      else
          m_Rect.right = ParentRect.right;
      MoveWindow( &m_Rect );
  }
}

UINT CInPlaceCWNumEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

void CInPlaceCWNumEdit::EndEdit()
{
    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    static BOOL bAlreadyEnding = FALSE;

    if(bAlreadyEnding)
      return;

    bAlreadyEnding = TRUE;
    
    CString str;
	  GetWindowText(str);
	  if (m_nFieldWidth > 0)
	  {
	    int nPads = m_nFieldWidth - str.GetLength();
	    if (0 < nPads && nPads < g_lenPads)
        str = g_StrPads.Left(nPads) + str;
    }
   
    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.strText  = str;
    dispinfo.item.lParam  = 0x0d; // m_nLastChar;

    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
      PostMessage(WM_CLOSE, 0, 0);
    bAlreadyEnding = FALSE;
}

// Stoopid win95 accelerator key problem workaround
BOOL CInPlaceCWNumEdit::PreTranslateMessage(MSG* pMsg) 
{
  // Catch the Alt key so we don't choke if focus is going to an owner drawn button
  if (pMsg->message == WM_SYSCHAR)
    return TRUE;
  
  return __super::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceCWNumEdit::PostNcDestroy() 
{
  __super::PostNcDestroy();
  delete this;
}
