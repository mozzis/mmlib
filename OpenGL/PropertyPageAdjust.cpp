/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: March 2000
//
/////////////////////////////////////////////////////////////////////////////
// PropertyPageAdjust.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "GlWinApp.h"
#include "PropertyPageAdjust.h"
#include "PropertySheetAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CPropertyPageAdjust 

IMPLEMENT_DYNAMIC(CPropertyPageAdjust, CPropertyPage)

CPropertyPageAdjust::CPropertyPageAdjust() : CPropertyPage()
{
}

CPropertyPageAdjust::CPropertyPageAdjust(UINT nIDTemplate, UINT nIDCaption)
	:CPropertyPage(nIDTemplate, nIDCaption)
{
}

CPropertyPageAdjust::CPropertyPageAdjust(LPCTSTR lpszTemplateName, UINT nIDCaption)
	:CPropertyPage(lpszTemplateName, nIDCaption)
{
}

CPropertyPageAdjust::~CPropertyPageAdjust()
{
}


BEGIN_MESSAGE_MAP(CPropertyPageAdjust, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageAdjust)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPropertyPageAdjust 

BOOL CPropertyPageAdjust::OnSetActive() 
{
	if (CPropertyPage::OnSetActive() == FALSE)
		return FALSE;

	CWnd* pParent = GetParent();
	if (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CPropertySheetAdjust)))
		pParent->SendMessage(WM_ADJUST_PROPERTY_SHEET);

	return TRUE;
}

int CPropertyPageAdjust::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRSRC hDlg;
	if (AfxIsValidString(m_psp.pszTemplate))
		hDlg = FindResource(m_psp.hInstance, m_psp.pszTemplate, RT_DIALOG);
	else
		hDlg = FindResource(m_psp.hInstance,
									MAKEINTRESOURCE(m_psp.pszTemplate), RT_DIALOG);

	if (hDlg)
	{
		HGLOBAL hResource = LoadResource(m_psp.hInstance, hDlg);
		if (hResource)
		{
			LPDLGTEMPLATE lpDialogTemplate;
			lpDialogTemplate = (LPDLGTEMPLATE)LockResource(hResource);

			LPDLGTEMPLATEEX lpDialogTemplateEx = (LPDLGTEMPLATEEX) lpDialogTemplate;

			if (lpDialogTemplate)
			{
				// Support for DIALOGEX PropertyPages, although those aren't very well supported either
				if (lpDialogTemplateEx->signature == 0xFFFF)
				{
					// DIALOGEX resource
					m_rectTemplateDLU.SetRect(lpDialogTemplateEx->x, lpDialogTemplateEx->y, 
						lpDialogTemplateEx->x + lpDialogTemplateEx->cx, lpDialogTemplateEx->y + lpDialogTemplateEx->cy);
				}
				else
				{
					// DIALOG resource
					m_rectTemplateDLU.SetRect(lpDialogTemplate->x, lpDialogTemplate->y, 
						lpDialogTemplate->x+lpDialogTemplate->cx, lpDialogTemplate->y + lpDialogTemplate->cy);
				}
				UnlockResource(hResource);
			}
			FreeResource(hResource);
		}
	}

	return 0;
}
