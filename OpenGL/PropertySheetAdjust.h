/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: March 2000
//
/////////////////////////////////////////////////////////////////////////////
// PropertySheetAdjust.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYSHEETADJUST_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_)
#define AFX_PROPERTYSHEETADJUST_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetAdjust

class AFX_EXT_CLASS CPropertySheetAdjust : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetAdjust)

// Konstruktion
public:
	CPropertySheetAdjust();
	CPropertySheetAdjust(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetAdjust(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attribute
public:
	CRect m_rectReal;
protected:
	BOOL m_bRcModifedIsUpToDate;

// Operationen
protected:
	void Adjust();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPropertySheetAdjust)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CPropertySheetAdjust();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	afx_msg LRESULT OnAdjustPropertySheet(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CPropertySheetAdjust)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROPERTYSHEETADJUST_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_
