/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
// based on a sample of
// Oleg G. Galkin
//
/////////////////////////////////////////////////////////////////////////////
// SplitterWndEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx

IMPLEMENT_DYNAMIC(CSplitterWndEx, CSplitterWnd)

BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
//{{AFX_MSG_MAP(CSplitterWndEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx Konstruktion/Destruktion

CSplitterWndEx::CSplitterWndEx()
{
	m_bEnableResizing = TRUE;
}

CSplitterWndEx::~CSplitterWndEx()
{
}

BOOL CSplitterWndEx::Create(CWnd* pParentWnd,
	int nMaxRows, int nMaxCols, SIZE sizeMin,
	CCreateContext* pContext, DWORD dwStyle, UINT nID)
{
	// MFC source code
	ASSERT(pParentWnd != NULL);
	ASSERT(sizeMin.cx > 0 && sizeMin.cy > 0);   // minimum must be non-zero

	ASSERT(pContext != NULL);
	ASSERT(pContext->m_pNewViewClass != NULL);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(dwStyle & SPLS_DYNAMIC_SPLIT);   // must have dynamic split behavior

	// Dynamic splitters are limited to 2x2
	ASSERT(nMaxRows >= 1 && nMaxRows <= 2);
	ASSERT(nMaxCols >= 1 && nMaxCols <= 2);
	ASSERT(nMaxCols > 1 || nMaxRows > 1);	   // 1x1 is not permitted

	m_nMaxRows = nMaxRows;
	m_nMaxCols = nMaxCols;
	ASSERT(m_nRows == 0 && m_nCols == 0);	   // none yet
	m_nRows = m_nCols = 1;	  // start off as 1x1
	if (!CreateCommon(pParentWnd, sizeMin, dwStyle, nID))
		return FALSE;
	ASSERT(m_nRows == 1 && m_nCols == 1);	   // still 1x1

	ASSERT(pContext->m_pNewViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	m_pDynamicViewClass = pContext->m_pNewViewClass;
	// save for later dynamic creations

	// add the first initial pane
	if (!CreateView(0, 0, m_pDynamicViewClass, sizeMin, pContext))
	{
		DestroyWindow(); // will clean up child windows
		return FALSE;
	}
	m_pColInfo[0].nIdealSize = sizeMin.cx;
	m_pRowInfo[0].nIdealSize = sizeMin.cy;

	// additional code
	int k;

	m_RowInfoEx.SetSize(m_nMaxRows);
	m_RowInfoEx[0].nRowColNumber	= 0; 
	m_RowInfoEx[0].bIsHidden		= FALSE; 
	for (k = 1; k < m_nMaxRows; k++)
	{
		m_RowInfoEx[k].nRowColNumber	= -1; 
		m_RowInfoEx[k].bIsHidden		= FALSE; 
	}

	m_ColInfoEx.SetSize(m_nMaxCols);
	m_ColInfoEx[0].nRowColNumber	= 0; 
	m_ColInfoEx[0].bIsHidden		= FALSE; 
	for (k = 1; k < m_nMaxCols; k++)
	{
		m_ColInfoEx[k].nRowColNumber	= -1; 
		m_ColInfoEx[k].bIsHidden		= FALSE; 
	}

	return TRUE;
}

// simple "wiper" splitter
BOOL CSplitterWndEx::CreateStatic(CWnd* pParentWnd,
	int nRows, int nCols, DWORD dwStyle, UINT nID)
{
	// MFC source code
	ASSERT(pParentWnd != NULL);
	ASSERT(nRows >= 1 && nRows <= 16);
	ASSERT(nCols >= 1 && nCols <= 16);
	ASSERT(nCols > 1 || nRows > 1);	 // 1x1 is not permitted
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(!(dwStyle & SPLS_DYNAMIC_SPLIT)); // can't have dynamic split

	ASSERT(m_nRows == 0 && m_nCols == 0);	   // none yet
	m_nRows = m_nMaxRows = nRows;
	m_nCols = m_nMaxCols = nCols;

	// create with zero minimum pane size
	if (!CreateCommon(pParentWnd, CSize(0, 0), dwStyle, nID))
		return FALSE;

	// all panes must be created with explicit calls to CreateView

	// additional code
	int k;

	m_RowInfoEx.SetSize(m_nMaxRows);
	for (k = 0; k < m_nMaxRows; k++)
	{
		m_RowInfoEx[k].nRowColNumber	= k; 
		m_RowInfoEx[k].bIsHidden		= FALSE; 
	}
	m_ColInfoEx.SetSize(m_nMaxCols);

	for (k = 0; k < m_nMaxCols; k++)
	{
		m_ColInfoEx[k].nRowColNumber	= k; 
		m_ColInfoEx[k].bIsHidden		= FALSE; 
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx Kommandos

void CSplitterWndEx::ShowColumn(int colShow)
{
	int row, col, k, colOld = 0, colNew = 0;
	ASSERT_VALID(this);
	ASSERT(m_nCols < m_nMaxCols);

	// find index of colShow
	for (k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == colShow)
		{
			colOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxCols);
	ASSERT(colOld >= m_nCols);
	ASSERT(m_ColInfoEx[colOld].bIsHidden == TRUE);

	// find index, where to insert colShow
	for (k = 0; k <= m_nCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber > colShow || k == m_nCols)
		{
			colNew = k;
			break;
		}
	}

	int cxNew = m_pColInfo[colOld].nCurSize;
	m_nCols++;  // add a column
	ASSERT(m_nCols <= m_nMaxCols);

	// fill the hidden column
	for (row = 0; row < m_nMaxRows; row++)
	{
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + colOld);
		if (row < m_nRows)				// visible rows
		{
			ASSERT(pPaneShow != NULL);
			pPaneShow->ShowWindow(SW_SHOWNA);
		}
		else							// hidden rows
		{
			if (pPaneShow == NULL)
				continue;
		}

		for (col = colOld - 1; col >= colNew; col--)
		{
			CWnd* pPane = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + (col + 1));
		}

		pPaneShow->SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + colNew);
	}

	// update m_ColInfoEx
	m_ColInfoEx[colOld].bIsHidden = FALSE;
	CRowColInfoEx tmpInfo = m_ColInfoEx[colOld];
	m_ColInfoEx.RemoveAt(colOld);
	m_ColInfoEx.InsertAt(colNew, tmpInfo);

	// new panes have been created -- recalculate layout
	for (col = colNew + 1; col <= colOld; col++)
		m_pColInfo[col].nIdealSize = m_pColInfo[col - 1].nCurSize;
	m_pColInfo[colNew].nIdealSize = cxNew;
	RecalcLayout();
}

void CSplitterWndEx::ShowRow(int rowShow)
{
	int row, col, k, rowOld = 0, rowNew = 0;
	ASSERT_VALID(this);
	ASSERT(m_nRows < m_nMaxRows);

	// find index of rowShow
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == rowShow)
		{
			rowOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxRows);
	ASSERT(rowOld >= m_nRows);
	ASSERT(m_RowInfoEx[rowOld].bIsHidden == TRUE);

	// find index, where to insert rowShow
	for (k = 0; k <= m_nRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber > rowShow || k == m_nRows)
		{
			rowNew = k;
			break;
		}
	}

	int cyNew = m_pRowInfo[rowOld].nCurSize;
	m_nRows++;  // add a row
	ASSERT(m_nRows <= m_nMaxRows);

	// fill the hidden row
	for (col = 0; col < m_nMaxCols; col++)
	{
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + rowOld * 16 + col);
		if (col < m_nCols)				// visible columns
		{
			ASSERT(pPaneShow != NULL);
			pPaneShow->ShowWindow(SW_SHOWNA);
		}
		else							// hidden columns
		{
			if (pPaneShow == NULL)
				continue;
		}

		for (row = rowOld - 1; row >= rowNew; row--)
		{
			CWnd* pPane = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(AFX_IDW_PANE_FIRST + (row + 1) * 16 + col);
		}

		pPaneShow->SetDlgCtrlID(AFX_IDW_PANE_FIRST + rowNew * 16 + col);
	}

	// update m_RowInfoEx
	m_RowInfoEx[rowOld].bIsHidden = FALSE;
	CRowColInfoEx tmpInfo = m_RowInfoEx[rowOld];
	m_RowInfoEx.RemoveAt(rowOld);
	m_RowInfoEx.InsertAt(rowNew, tmpInfo);

	// new panes have been created -- recalculate layout
	for (row = rowNew + 1; row <= rowOld; row++)
		m_pRowInfo[row].nIdealSize = m_pRowInfo[row - 1].nCurSize;
	m_pRowInfo[rowNew].nIdealSize = cyNew;
	RecalcLayout();
}

void CSplitterWndEx::HideColumn(int colHide)
{
	int rowActive, colActive, row, col, k, colOld = 0, colNew = m_nCols - 1;
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);

	// find index of colHide
	for (k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == colHide)
		{
			colOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxCols);
	ASSERT(colOld < m_nCols);
	ASSERT(m_ColInfoEx[colOld].bIsHidden == FALSE);

	// if the column has an active window -- change it
	if (GetActivePane(&rowActive, &colActive) != NULL && colActive == colOld)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all column panes
	for (row = 0; row < m_nMaxRows; row++)
	{
		CWnd* pPaneHide = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + colOld);
		if (row < m_nRows)				// visible rows
		{
			ASSERT(pPaneHide != NULL);
			pPaneHide->ShowWindow(SW_HIDE);
		}
		else							// hidden rows
		{
			if (pPaneHide == NULL)
				continue;
		}

		for (col = colOld + 1; col <= colNew; col++)
		{
			CWnd* pPane = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + (col - 1));
		}

		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + colNew);
	}

	// update m_ColInfoEx
	m_ColInfoEx[colOld].bIsHidden = TRUE;
	CRowColInfoEx tmpInfo = m_ColInfoEx[colOld];
	m_ColInfoEx.RemoveAt(colOld);
	m_ColInfoEx.InsertAt(colNew, tmpInfo);

	m_pColInfo[colNew].nCurSize = m_pColInfo[colOld].nCurSize;
	m_nCols--;
	RecalcLayout();
}

void CSplitterWndEx::HideRow(int rowHide)
{
	int rowActive, colActive, row, col, k, rowOld = 0, rowNew = m_nRows - 1;
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);

	// find index of rowHide
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == rowHide)
		{
			rowOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxRows);
	ASSERT(rowOld < m_nRows);
	ASSERT(m_RowInfoEx[rowOld].bIsHidden == FALSE);

	// if the row has an active window -- change it
	if (GetActivePane(&rowActive, &colActive) != NULL && rowActive == rowOld)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all row panes
	for (col = 0; col < m_nMaxCols; col++)
	{
		CWnd* pPaneHide = GetDlgItem(AFX_IDW_PANE_FIRST + rowOld * 16 + col);
		if (col < m_nCols)				// visible columns
		{
			ASSERT(pPaneHide != NULL);
			pPaneHide->ShowWindow(SW_HIDE);
		}
		else							// hidden columns
		{
			if (pPaneHide == NULL)
				continue;
		}

		for (row = rowOld + 1; row <= rowNew; row++)
		{
			CWnd* pPane = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(AFX_IDW_PANE_FIRST + (row - 1) * 16 + col);
		}

		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + rowNew * 16 + col);
	}

	// update m_RowInfoEx
	m_RowInfoEx[rowOld].bIsHidden = TRUE;
	CRowColInfoEx tmpInfo = m_RowInfoEx[rowOld];
	m_RowInfoEx.RemoveAt(rowOld);
	m_RowInfoEx.InsertAt(rowNew, tmpInfo);

	m_pRowInfo[rowNew].nCurSize = m_pRowInfo[rowOld].nCurSize;
	m_nRows--;
	RecalcLayout();
}

BOOL CSplitterWndEx::IsColHidden(int col)
{
	for (int k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == col)
			return m_ColInfoEx[k].bIsHidden;
	}
	ASSERT(k < m_nMaxCols);
	return FALSE;
}

BOOL CSplitterWndEx::IsRowHidden(int row)
{
	for (int k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == row)
			return m_RowInfoEx[k].bIsHidden;
	}
	ASSERT(k < m_nMaxRows);
	return FALSE;
}

int CSplitterWndEx::GetHiddenColumnCount()
{
	int count = 0;
	for (int k = 0; k < m_nMaxCols; k++)
	{
		count += m_ColInfoEx[k].bIsHidden;
	}
	return count;
}

int CSplitterWndEx::GetHiddenRowCount()
{
	int count = 0;
	for (int k = 0; k < m_nMaxRows; k++)
	{
		count += m_RowInfoEx[k].bIsHidden;
	}
	return count;
}

int CSplitterWndEx::GetSplitterBarWidth()
{
	return m_cxSplitter;
}

int CSplitterWndEx::GetSplitterBarHeight()
{
	return m_cySplitter;
}

void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bEnableResizing == FALSE)
		return;
	CSplitterWnd::OnLButtonDown(nFlags, point);
}

BOOL CSplitterWndEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bEnableResizing == FALSE)
		return TRUE;
	else
		return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bEnableResizing == FALSE)
		CWnd::OnMouseMove(nFlags, point);
	else
		CSplitterWnd::OnMouseMove(nFlags, point);
}

BOOL CSplitterWndEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	BOOL bRetVal = FALSE;
	int row;
	int col;

	CRect mRect;
	CScrollBar* pBar;
	CWnd* pPane;
	CScrollView* pView;
	int nOldPos = 0;

	for (row = 0; row < m_nRows; row++)
	{
		for (col = 0; col < m_nCols; col++)
		{
			pPane = GetPane(row, col);
			if (!pPane->IsKindOf(RUNTIME_CLASS(CScrollView)))
				continue;

			pView = STATIC_DOWNCAST(CScrollView, pPane);
			if (pView != NULL)
			{
				pBar = pView->GetScrollBarCtrl(SB_VERT);
				if (pBar == NULL)
				{
					pBar = pView->GetScrollBarCtrl(SB_HORZ);
					/*if (pBar == NULL)
						continue;*/
				}
				pView->GetWindowRect(&mRect);

				if (pBar)
					nOldPos = pBar->GetScrollPos();

				if (pt.x >= mRect.left && pt.x <= mRect.right && pt.y >= mRect.top && pt.y <= mRect.bottom)
					if (pView->DoMouseWheel(nFlags, zDelta, pt))
					{
						bRetVal = TRUE;
					}

				if (pBar && col < m_nCols -1)
					pBar->SetScrollPos(nOldPos, FALSE);
			}
		}
	}

	return bRetVal;
}

CWnd* CSplitterWndEx::GetActivePane(int* pRow, int* pCol)
	// return active view, NULL when no active view
{
	ASSERT_VALID(this);

	// attempt to use active view of frame window
	CWnd* pView = NULL;
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pView = pFrameWnd->GetActiveView();

	// failing that, use the current focus
	if (pView == NULL)
		pView = GetFocus();

	// Note: The original SplitterWnd::GetActivePane causes
	// problems with nested splitters - IsChildPane return 
	// nonsense in case pView is a pane of a nested splitter.
	// So we try to iterate to the correct splitter.
	while (pView)
	{
		if (pView->GetParent() == this)
			break;
		else
			pView = pView->GetParent();
	}

	// make sure the pane is a child pane of the splitter
	if (pView != NULL && !IsChildPane(pView, pRow, pCol))
		pView = NULL;

	return pView;
}

BOOL CSplitterWndEx::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4 && 
		GetAsyncKeyState(VK_CONTROL) >= 0 && GetAsyncKeyState(VK_MENU) >= 0)
	{
		ASSERT_VALID(this);

		// find the coordinate of the current pane
		int row, col;
		CWnd* ret;
		if ((ret = GetActivePane(&row, &col)) == NULL)
		{
			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				row = m_nRows - 1;
				col = m_nCols - 1;
			}
			else
			{
				row = 0;
				col = 0;
			}
			TRACE("Warning: There is no current view - go to pane (%d,%d).\n", row, col);
		}
		else
		{
			ASSERT(row >= 0 && row < m_nRows);
			ASSERT(col >= 0 && col < m_nCols);

			// determine next pane
			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				// prev
				if (--col < 0)
				{
					col = m_nCols - 1;
					if (--row < 0)
					{
						// after first panes check for parent (nested) splitter
						CWnd* pParent = GetParent();
						if (pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
						{
							// let the parent splitter do it
							return ((CSplitterWndEx*)pParent)->PreTranslateMessage(pMsg);
						}
						else
						{
							row = m_nRows - 1;
						}
					}
				}
			}
			else
			{
				// next
				if (++col >= m_nCols)
				{
					col = 0;
					if (++row >= m_nRows)
					{
						// after last panes check for parent (nested) splitter
						CWnd* pParent = GetParent();
						if (pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
						{
							return ((CSplitterWndEx*)pParent)->PreTranslateMessage(pMsg);
						}
						else
						{
							row = 0;
						}
					}
				}
			}
		}

		// check for child (nested) splitter
		CWnd* pPane = GetPane(row, col);
		if (pPane->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
		{
			// let the child splitter do it
			return ((CSplitterWndEx*)pPane)->PreTranslateMessage(pMsg);
		}
		else
		{		
			// set newly active pane
			SetActivePane(row, col);
			return TRUE;
		}
	}

	return CSplitterWnd::PreTranslateMessage(pMsg);
}
