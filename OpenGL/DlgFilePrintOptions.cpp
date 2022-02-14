/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgFilePrintOptions.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "DlgFilePrintOptions.h"
#include "DdxDdvSupport.h"
#include "Resource.hm"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgFilePrintOptions 


CDlgFilePrintOptions::CDlgFilePrintOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilePrintOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilePrintOptions)
	m_nMaxPrintMemSize = 0;
	m_nPrintObject = -1;
	m_bPrintBkgndWhite = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFilePrintOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilePrintOptions)
	DDX_Control(pDX, IDC_PRINT_BKGND_WHITE, m_PrintBkgndButton);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
	DDX_Control(pDX, IDC_BUTTON_PRINTER, m_PrinterButton);
	DDX_Control(pDX, IDC_STATIC_TEXT_F_TWO, m_TextFTwoStatic);
	DDX_Control(pDX, IDC_STATIC_TEXT_F_ONE, m_TextFOneStatic);
	DDX_Control(pDX, IDC_STATIC_OPTIONS, m_OptionsButton);
	DDX_Control(pDX, IDC_PRINT_MEM_STANDARD, m_MaxPrintMemSizeStandardButton);
	DDX_Control(pDX, IDC_PRINT_MEM, m_MaxPrintMemSizeEdit);
	DDX_Text(pDX, IDC_PRINT_MEM, m_nMaxPrintMemSize);
	DDV_MinInt(pDX, m_nMaxPrintMemSize, 100);
	DDX_Radio(pDX, IDC_PRINT_VIEW, m_nPrintObject);
	DDX_Check(pDX, IDC_PRINT_BKGND_WHITE, m_bPrintBkgndWhite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFilePrintOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgFilePrintOptions)
	ON_BN_CLICKED(IDC_PRINT_MEM_STANDARD, OnPrintMemStandard)
	ON_BN_CLICKED(IDC_PRINT_VIEW, OnPrintView)
	ON_BN_CLICKED(IDC_PRINT_DOC, OnPrintDoc)
	ON_BN_CLICKED(IDC_PRINT_APP, OnPrintApp)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER, OnButtonPrinter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgFilePrintOptions 

void CDlgFilePrintOptions::OnPrintMemStandard() 
{
	// ZU ERLEDIGEN: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	CString newSize;
	newSize.Format("%d", 8*1024);
	m_MaxPrintMemSizeEdit.SetSel(0, -1);
	m_MaxPrintMemSizeEdit.ReplaceSel(LPCTSTR(newSize), TRUE);
	GotoDlgCtrl(&m_MaxPrintMemSizeEdit);
}

void CDlgFilePrintOptions::EnableControls(BOOL bEnable)
{
	m_PrintBkgndButton.EnableWindow(bEnable);
	m_MaxPrintMemSizeStandardButton.EnableWindow(bEnable);
	m_MaxPrintMemSizeEdit.EnableWindow(bEnable);
	m_TextFOneStatic.EnableWindow(bEnable);
	m_TextFTwoStatic.EnableWindow(bEnable);
	m_HelpButton.EnableWindow(bEnable);
}

BOOL CDlgFilePrintOptions::OnInitDialog() 
{
	BOOL bRet = CDialog::OnInitDialog();

	EnableControls(m_nPrintObject != 2);

	return bRet;
}

void CDlgFilePrintOptions::OnPrintView() 
{
	EnableControls(TRUE);
}

void CDlgFilePrintOptions::OnPrintDoc() 
{
	EnableControls(TRUE);
}

void CDlgFilePrintOptions::OnPrintApp() 
{
	EnableControls(FALSE);
}

DWORD CDlgFilePrintOptions::m_dwHelpMap[] =
{
	IDC_STATIC_OPTIONS, HIDC_STATIC_OPTIONS,
	0,0
};

BOOL CDlgFilePrintOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	HWND hWnd = m_OptionsButton.m_hWnd;
	ASSERT(hWnd != NULL);
	
	if (pHelpInfo->iContextType == HELPINFO_WINDOW) 
	{
		::WinHelp(hWnd/*(HWND)pHelpInfo->hItemHandle*/,
			AfxGetApp()->m_pszHelpFilePath,
			HELP_WM_HELP,
			(DWORD)(LPVOID)m_dwHelpMap);
	}

	return TRUE;
}

void CDlgFilePrintOptions::OnButtonHelp() 
{
	HELPINFO helpInfo;
	helpInfo.iContextType = HELPINFO_WINDOW;
	OnHelpInfo(&helpInfo);
}

void CDlgFilePrintOptions::OnButtonPrinter() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_PRINT_SETUP);
}
