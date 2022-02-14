/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: March 2000
//
/////////////////////////////////////////////////////////////////////////////
// PropertySheetAdjust.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "GlWinapp.h"
#include "PropertySheetAdjust.h"
#include "PropertyPageAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetAdjust

IMPLEMENT_DYNAMIC(CPropertySheetAdjust, CPropertySheet)

CPropertySheetAdjust::CPropertySheetAdjust()
	:CPropertySheet()
{
}

CPropertySheetAdjust::CPropertySheetAdjust(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropertySheetAdjust::CPropertySheetAdjust(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CPropertySheetAdjust::~CPropertySheetAdjust()
{
}


BEGIN_MESSAGE_MAP(CPropertySheetAdjust, CPropertySheet)
	ON_MESSAGE(WM_ADJUST_PROPERTY_SHEET, OnAdjustPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetAdjust)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPropertySheetAdjust 

LRESULT CPropertySheetAdjust::OnAdjustPropertySheet(WPARAM, LPARAM) 
{
	if (::IsWindow(m_hWnd))
		Adjust();

	return 0;
}

void CPropertySheetAdjust::Adjust()
{
	CTabCtrl* pTabCtrl = GetTabControl();
	CPropertyPageAdjust* pPage = (CPropertyPageAdjust*)GetPage(GetActiveIndex());
	ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CPropertyPageAdjust)));

	// Retrieve the original size of PropertyPage.
	CRect rectTemplate(pPage->m_rectTemplateDLU);
	pPage->MapDialogRect(rectTemplate);

	// When calling it the first time m_rectReal is unititalized.
	// So we setup rectReal now.
	CRect rectReal(m_rectReal);
	if (!m_bRcModifedIsUpToDate)
	{
		pPage->GetWindowRect(rectReal);
		ScreenToClient(rectReal);
	}

	int dcx = rectTemplate.Width() - rectReal.Width();
	int dcy = rectTemplate.Height() - rectReal.Height();
	int dx = 0, dy = 0;
	
	if (!m_bRcModifedIsUpToDate && IsWizard())
	{
		dx = -rectReal.left;
		dy = -rectReal.top;

		// Move all childs one dialog unit to the left/top
		CRect dummyRect(0, 0, 1, 1);
		pPage->MapDialogRect(dummyRect);
		dx -= dummyRect.Width();
		dy -= dummyRect.Height();
	}

	CRect rectCtrlWnd;

	// Resize PropertyPage
	rectCtrlWnd = rectReal;
	rectCtrlWnd.InflateRect(0, 0, dcx, dcy);
	rectCtrlWnd.OffsetRect(dx, dy);
	pPage->MoveWindow(rectCtrlWnd);
	m_rectReal = rectCtrlWnd;

	// Resize TabControl
	pTabCtrl->GetWindowRect(rectCtrlWnd);
	ScreenToClient(rectCtrlWnd);
	if (!IsWizard())
	{
		rectCtrlWnd.InflateRect(0, 0, dcx, dcy);
	}
	else
	{
		rectCtrlWnd.SetRectEmpty();
	}
	pTabCtrl->MoveWindow(rectCtrlWnd);

	// Move all other controls
	for (CWnd* pChild = GetWindow(GW_CHILD); pChild; pChild = pChild->GetNextWindow(GW_HWNDNEXT)) 
	{
		if (pChild == pTabCtrl || pChild == pPage)
			continue;

		pChild->GetWindowRect(rectCtrlWnd);
		ScreenToClient(rectCtrlWnd);
		rectCtrlWnd.OffsetRect(dcx + 2*dx, dcy + 3*dy);
		if (IsWizard() && (pChild->GetStyle() & SS_SUNKEN))
		{
			pChild->ShowWindow(SW_HIDE);
		}
		pChild->MoveWindow(rectCtrlWnd);
	}

	// Resize PropertySheet
	GetWindowRect(rectCtrlWnd);
	rectCtrlWnd.InflateRect(0, 0, dcx + 2*dx, dcy + 3*dy);

	// Invalidate the whole sheet to get it paint properly
	InvalidateRect(NULL, FALSE);
	MoveWindow(rectCtrlWnd);

	if (!m_bRcModifedIsUpToDate)
	{
		m_bRcModifedIsUpToDate = TRUE;
	}
}

int CPropertySheetAdjust::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bRcModifedIsUpToDate = FALSE;

	return 0;
}
