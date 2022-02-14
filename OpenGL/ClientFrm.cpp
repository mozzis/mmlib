/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// ClientFrame.cpp : Implementierungsdatei
//

#include "StdAfx.h"
#include "ClientFrm.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ID_TOOLTIP
#define ID_TOOLTIP 1
#endif

extern CRect oldRect;

/////////////////////////////////////////////////////////////////////////////
// CClientFrame

IMPLEMENT_DYNCREATE(CClientFrame, CWnd)

BEGIN_MESSAGE_MAP(CClientFrame, CWnd)
	//{{AFX_MSG_MAP(CClientFrame)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CClientFrame::CClientFrame()
{
	m_bTile = FALSE;
	m_pTopLeft = NULL;
	m_bLButtonDownInBitmap = FALSE;
}

CClientFrame::~CClientFrame()
{
}

BOOL CClientFrame::GetBitmapAndPalette(UINT nIDResource)
{
	return ::GetBitmapAndPalette(nIDResource, m_Bitmap, m_Palette); 
}
	
/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CClientFrame

BOOL CClientFrame::OnEraseBkgnd(CDC*) 
{
	return TRUE;
}

void CClientFrame::OnPaint() 
{
	// Do not call CWnd::OnPaint() for painting messages
	InvalidateRect(&oldRect);
	CRect newRect;
	GetClientRect(&newRect);

	if (!m_bTile)
	{
		int left, top;
		BITMAP bm;
		m_Bitmap.GetBitmap(&bm);
		if (m_pTopLeft != NULL)
		{
			left = m_pTopLeft->x;
			top = m_pTopLeft->y;
		}
		else
		{
			left = (newRect.Width()-bm.bmWidth)/2;
			top = (newRect.Height()-bm.bmHeight)/2;
		}
		newRect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
	}
	InvalidateRect(&newRect);

	CPaintDC dc(this); // device context for painting
	DrawTheBackground(this, &dc, &m_Palette, &m_Bitmap, m_bTile, m_pTopLeft);
}

BOOL CClientFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		BOOL bIsOverlapped = (WindowFromPoint(point) != this);
		ScreenToClient(&point);

		if (rect.PtInRect(point) && !bIsOverlapped)
		{
			if (m_HyperLink.GetLinkCursor())
			{
				::SetCursor(m_HyperLink.GetLinkCursor());
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CClientFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		m_HyperLink.GetToolTipCtrl().RelayEvent(pMsg);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CClientFrame::PreSubclassWindow() 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		m_HyperLink.SetDefaultCursor();		// Try and load up a "hand" cursor

		// Create the tooltip
		CRect rect; 
		GetClientRect(rect);
		if (!m_bTile && m_pTopLeft != NULL)
		{
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			long& left = m_pTopLeft->x;
			long& top = m_pTopLeft->y;
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		m_HyperLink.GetToolTipCtrl().Create(this);
		m_HyperLink.GetToolTipCtrl().AddTool(this, m_HyperLink.GetURL(), rect, ID_TOOLTIP);
	}

	CWnd::PreSubclassWindow();
}

void CClientFrame::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_HyperLink.GetURL().IsEmpty())
	{

		CRect rect; 
		GetClientRect(rect);
		if (m_bTile)
		{
			m_HyperLink.GetToolTipCtrl().SetToolRect(this, ID_TOOLTIP, rect);
		}
		else if (m_pTopLeft == NULL)
		{
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			int left = (rect.Width()-bm.bmWidth)/2;
			int top = (rect.Height()-bm.bmHeight)/2;
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
			m_HyperLink.GetToolTipCtrl().SetToolRect(this, ID_TOOLTIP, rect);
		}
	}
}

void CClientFrame::SetURL(CString strURL)
{
	m_HyperLink.SetURL(strURL);
}

void CClientFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		ScreenToClient(&point);

		if (rect.PtInRect(point))
		{
			m_bLButtonDownInBitmap = TRUE;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CClientFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		ScreenToClient(&point);

		if (rect.PtInRect(point) && m_bLButtonDownInBitmap)
		{
			m_HyperLink.Clicked();
		}
		m_bLButtonDownInBitmap = FALSE;
	}

	CWnd::OnLButtonUp(nFlags, point);
}
