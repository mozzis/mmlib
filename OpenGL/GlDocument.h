/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: September 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlDocument.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGlView;

class AFX_EXT_CLASS CGlDocument : public CDocument
{
	DECLARE_DYNAMIC(CGlDocument)
public:
	CGlDocument();

// Attributes
public:

protected:

// Operations
public:
	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	void AddStarToTitle();
	void RemoveStarFromTitle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlDocument)
	public:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo, CGlView* pCallingView);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	protected:
	virtual BOOL SaveModified();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGlDocument();

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGlDocument)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
