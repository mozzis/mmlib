/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: March 2000
//
/////////////////////////////////////////////////////////////////////////////
// PropertyPageAdjust.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYPAGEADJUST_H__A15D9044_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_)
#define AFX_PROPERTYPAGEADJUST_H__A15D9044_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageAdjust 

class AFX_EXT_CLASS CPropertyPageAdjust : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageAdjust)

// Konstruktion
public:
	CPropertyPageAdjust();
	CPropertyPageAdjust(UINT nIDTemplate, UINT nIDCaption = 0);
	CPropertyPageAdjust(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	~CPropertyPageAdjust();

// Attribute
	CRect m_rectTemplateDLU;


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPropertyPageAdjust)
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPropertyPageAdjust)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROPERTYPAGEADJUST_H__A15D9044_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_
