/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: February 2000
//
// based on a sample of
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// DlgOpenglInfo.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "DlgOpenglInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenglInfo 


CDlgOpenglInfo::CDlgOpenglInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpenglInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpenglInfo)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CDlgOpenglInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpenglInfo)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_GL_VENDOR, m_info.strGlVendor);
	DDX_Text(pDX, IDC_STATIC_GL_RENDERER, m_info.strGlRenderer);
	DDX_Text(pDX, IDC_STATIC_GL_VERSION, m_info.strGlVersion);
	DDX_Text(pDX, IDC_STATIC_GL_ACCELERATION, m_info.strGlAcceleration);
	DDX_Text(pDX, IDC_EDIT_GL_EXTENSIONS, m_info.strGlExtensions);
	DDX_Text(pDX, IDC_STATIC_GLU_VERSION, m_info.strGluVersion);
	DDX_Text(pDX, IDC_EDIT_GLU_EXTENSIONS, m_info.strGluExtensions);
}


BEGIN_MESSAGE_MAP(CDlgOpenglInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgOpenglInfo)
	ON_BN_CLICKED(IDC_DETAILS, OnDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgOpenglInfo 

void CDlgOpenglInfo::OnDetails() 
{
	CString strDetails;
	strDetails.Format(IDS_OPENGLINFO,
		m_info.nMaxLights, 
		m_info.nMaxClipPlanes, 
		m_info.nMaxModelStackDepth, 
		m_info.nMaxProjectionStackDepth, 
		m_info.nMaxTextureStackDepth, 
		m_info.nMaxNameStackDepth, 
		m_info.nMaxAttributeStackDepth, 
		m_info.nMaxTextureSize, 
		m_info.nMaxListNesting, 
		m_info.nMaxEvalOrder, 
		m_info.maxViewportDims.cx, m_info.maxViewportDims.cy, 
		m_info.nAuxiliaryBuffers, 
		m_info.minSmoothPointSize, m_info.maxSmoothPointSize, 
		m_info.smoothPointGranularity, 
		m_info.minSmoothLineSize, m_info.maxSmoothLineSize, 
		m_info.smoothLineGranularity, 
		m_info.nRedBits, m_info.nGreenBits, m_info.nBlueBits, m_info.nAlphaBits, 
		m_info.nDepthBits, 
		m_info.nStencilBits);
	MessageBox(strDetails, "OpenGl Information - Details", MB_OK | MB_ICONINFORMATION);	
}

BOOL CDlgOpenglInfo::OnInitDialog() 
{
	// convert the spaces in newlines (in extensions fields) 
	int pos;

	pos = 0;
	while ((pos = m_info.strGlExtensions.Find(_T(' '))) != -1)
	{
		// substitute space with LF
		m_info.strGlExtensions.SetAt(pos, _T('\n'));
		// insert a CR before LF
#if _MSC_VER >= 1200
		m_info.strGlExtensions.Insert(pos, _T('\r'));
#else // _MSC_VER >= 1200
		m_info.strGlExtensions = m_info.strGlExtensions.Left(pos) + _T('\r') + m_info.strGlExtensions.Right(m_info.strGlExtensions.GetLength() - pos);
#endif // _MSC_VER >= 1200
	}
	pos = 0;
	while ((pos = m_info.strGluExtensions.Find(_T(' '))) != -1)
	{
		// substitute space with LF
		m_info.strGluExtensions.SetAt(pos, _T('\n'));
		// insert a CR before LF
#if _MSC_VER >= 1200
		m_info.strGluExtensions.Insert(pos, _T('\r'));
#else // _MSC_VER >= 1200
		m_info.strGluExtensions = m_info.strGluExtensions.Left(pos) + _T('\r') + m_info.strGluExtensions.Right(m_info.strGluExtensions.GetLength() - pos);
#endif // _MSC_VER >= 1200
	}

	BOOL bRet = CDialog::OnInitDialog();

	return bRet;
}
