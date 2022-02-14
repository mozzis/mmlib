/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: January 2000
//
/////////////////////////////////////////////////////////////////////////////
// ToolBarEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _MSC_VER >= 1200
/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

IMPLEMENT_DYNAMIC(CToolBarEx, CToolBar)

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx Überschreibungen

#define CX_GRIPPER  3
#define CY_GRIPPER  3
#define CX_BORDER_GRIPPER 0
#define CY_BORDER_GRIPPER 0

void CToolBarEx::DrawGripper(CDC* pDC, const CRect& rect)
{
	// only draw the gripper if not floating and gripper is specified
	if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
	{
		// draw the gripper in the border
		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			pDC->Draw3dRect(rect.left+CX_BORDER_GRIPPER,
				rect.top+m_cyTopBorder,
				CX_GRIPPER, rect.Height()-m_cyTopBorder-m_cyBottomBorder,
				::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(rect.left+CX_BORDER_GRIPPER+3,
				rect.top+m_cyTopBorder,
				CX_GRIPPER, rect.Height()-m_cyTopBorder-m_cyBottomBorder,
				::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
		}
		else
		{
			pDC->Draw3dRect(rect.left+m_cyTopBorder,
				rect.top+CY_BORDER_GRIPPER,
				rect.Width()-m_cyTopBorder-m_cyBottomBorder, CY_GRIPPER,
				::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(rect.left+m_cyTopBorder,
				rect.top+CY_BORDER_GRIPPER+3,
				rect.Width()-m_cyTopBorder-m_cyBottomBorder, CY_GRIPPER,
				::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
		}
	}
}

// EraseNonClient is a copy of MFC source code
void CToolBarEx::EraseNonClient()
{
	// get window DC that is clipped to the non-client area
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	// draw borders in non-client area
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawBorders(&dc, rectWindow);

	// erase parts not drawn
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// draw gripper in non-client area
	DrawGripper(&dc, rectWindow);
}

void CToolBarEx::OnNcPaint()
{
	EraseNonClient();
}
#endif // _MSC_VER >= 1200
