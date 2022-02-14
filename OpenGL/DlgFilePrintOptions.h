/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgFilePrintOptions.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_)
#define AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgFilePrintOptions 

class CDlgFilePrintOptions : public CDialog
{
// Konstruktion
public:
	CDlgFilePrintOptions(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgFilePrintOptions)
	enum { IDD = IDD_DLG_FILE_PRINT_OPTIONS };
	CButton	m_PrintBkgndButton;
	CButton	m_HelpButton;
	CButton	m_PrinterButton;
	CStatic	m_TextFTwoStatic;
	CStatic	m_TextFOneStatic;
	CButton	m_OptionsButton;
	CButton	m_MaxPrintMemSizeStandardButton;
	CEdit	m_MaxPrintMemSizeEdit;
	int		m_nMaxPrintMemSize;
	int		m_nPrintObject;
	BOOL	m_bPrintBkgndWhite;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgFilePrintOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	static DWORD m_dwHelpMap[];
	void EnableControls(BOOL bEnable = TRUE);

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgFilePrintOptions)
	afx_msg void OnPrintMemStandard();
	virtual BOOL OnInitDialog();
	afx_msg void OnPrintView();
	afx_msg void OnPrintDoc();
	afx_msg void OnPrintApp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonPrinter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_)
