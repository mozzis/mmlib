/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgOptions.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_)
#define AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOptions 

class CDlgOptions : public CDialog
{
// Konstruktion
public:
	CDlgOptions(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgOptions)
	enum { IDD = IDD_DLG_OPTIONS };
	CButton	m_SerializeGraphBytesGboxButton;
	CButton	m_SerializeGraphEightBytesButton;
	CButton	m_SerializeGraphFourBytesButton;
	CButton	m_SerializeGraphTwoBytesButton;
	CButton	m_SerializeGraphOneByteButton;
	int		m_nRadionMousePolicy;
	BOOL	m_bCheckPropPolicy;
	BOOL	m_bCheckReopenPolicy;
	int		m_nRadioSerializeGraphBytes;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void EnableExtraEdit();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgOptions)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_
