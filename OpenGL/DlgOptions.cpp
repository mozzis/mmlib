/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgOptions.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "DlgOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOptions 


CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptions)
	m_nRadionMousePolicy = -1;
	m_bCheckPropPolicy = FALSE;
	m_bCheckReopenPolicy = FALSE;
	m_nRadioSerializeGraphBytes = -1;
	//}}AFX_DATA_INIT
}


void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptions)
	DDX_Control(pDX, IDC_STATIC_SERIALZE_GRAPH_BYTES_GBOX, m_SerializeGraphBytesGboxButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_GRAPH_BYTES_8, m_SerializeGraphEightBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_GRAPH_BYTES_4, m_SerializeGraphFourBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_GRAPH_BYTES_2, m_SerializeGraphTwoBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_GRAPH_BYTES_1, m_SerializeGraphOneByteButton);
	DDX_Radio(pDX, IDC_RADIO_MOUSE_POLICY_0, m_nRadionMousePolicy);
	DDX_Check(pDX, IDC_CHECK_PROP_POLICY, m_bCheckPropPolicy);
	DDX_Check(pDX, IDC_CHECK_REOPEN_POLICY, m_bCheckReopenPolicy);
	DDX_Radio(pDX, IDC_RADIO_SERIALZE_GRAPH_BYTES_1, m_nRadioSerializeGraphBytes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgOptions 

BOOL CDlgOptions::OnInitDialog() 
{
	BOOL bRet = CDialog::OnInitDialog();

	EnableExtraEdit();

	return bRet;
}

void CDlgOptions::EnableExtraEdit()
{
	BOOL bEnable = (m_nRadioSerializeGraphBytes == 0 
		|| m_nRadioSerializeGraphBytes == 1 
		|| m_nRadioSerializeGraphBytes == 2 
		|| m_nRadioSerializeGraphBytes == 3);

	m_SerializeGraphBytesGboxButton.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_SerializeGraphOneByteButton.EnableWindow(bEnable);
	m_SerializeGraphOneByteButton.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_SerializeGraphTwoBytesButton.EnableWindow(bEnable);
	m_SerializeGraphTwoBytesButton.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_SerializeGraphFourBytesButton.EnableWindow(bEnable);
	m_SerializeGraphFourBytesButton.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_SerializeGraphEightBytesButton.EnableWindow(bEnable);
	m_SerializeGraphEightBytesButton.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);

	int yInflate = 0;
	CRect rect;

	HDC hDC = ::GetDC(NULL);
	int nPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);

	if (!bEnable)
	{
		yInflate -= 48*5/3*nPixelsY/96;
	}

	GetWindowRect(&rect);
	rect.InflateRect(0, 0, 0, yInflate);
	MoveWindow(rect);
}
