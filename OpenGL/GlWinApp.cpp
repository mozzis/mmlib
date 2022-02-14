/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlWinApp.cpp : Implementierungsdatei
//

#include "StdAfx.h"
#include "GlWinApp.h"
#include "GlView.h"
#include "GlDocument.h"
#include "DlgFilePrintOptions.h"
#include "Helper.h"
#include "DlgOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp

IMPLEMENT_DYNAMIC(CGlWinApp, CWinApp)

BEGIN_MESSAGE_MAP(CGlWinApp, CWinApp)
	//{{AFX_MSG_MAP(CGlWinApp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_FILE_PRINT_SETUP_EX, OnFilePrintSetupEx)
	ON_COMMAND(ID_CHANGE_MOUSE_POLICY, OnChangeMousePolicy)
	ON_COMMAND(ID_CHANGE_PROP_POLICY, OnChangePropPolicy)
	ON_COMMAND(ID_CHANGE_REOPEN_POLICY, OnChangeReopenPolicy)
	ON_COMMAND(ID_SERIALIZE_GRAPH_ONE_BYTE, OnSerializeGraphOneByte)
	ON_COMMAND(ID_SERIALIZE_GRAPH_TWO_BYTE, OnSerializeGraphTwoBytes)
	ON_COMMAND(ID_SERIALIZE_GRAPH_FOUR_BYTE, OnSerializeGraphFourBytes)
	ON_COMMAND(ID_SERIALIZE_GRAPH_EIGHT_BYTE, OnSerializeGraphEightBytes)
	ON_COMMAND(ID_CHANGE_POLICY, OnChangePolicy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp construction

CGlWinApp::CGlWinApp()
{
	// Options
	m_OptionColorGlBack = RGB(96, 96, 96);
	m_OptionColorGlLightAmbient = RGB(128, 128, 128); 
	m_OptionColorGlLightDiffuse = RGB(128, 128, 128); 
	m_OptionColorGlLightSpecular = RGB(0, 0, 0); 
	m_OptionColorText = RGB(255, 255, 255); 

	m_LogFont.lfHeight = -24;
	m_LogFont.lfWidth = 0;
	m_LogFont.lfEscapement = 0;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfWeight = 0;
	m_LogFont.lfItalic = 0;
	m_LogFont.lfUnderline = 0;
	m_LogFont.lfStrikeOut = 0;
	m_LogFont.lfCharSet = 0;
	m_LogFont.lfOutPrecision = 0;
	m_LogFont.lfClipPrecision = 0;
	m_LogFont.lfQuality = 0;
	m_LogFont.lfPitchAndFamily = 0;
	sprintf(m_LogFont.lfFaceName, "Times New Roman");

	m_bAntialias = FALSE;
	m_bSmooth = TRUE;
	m_bLighting = TRUE;
	m_nPolygonMode = 2;
	m_nMaxPrintMemSize = 8*1024;
	m_nPrintObject = 0;
	m_bPrintBkgndWhite = TRUE;
	m_nMousePolicy = 1;						// 0 = DblClk/Clk
											// 1 = Clk/Clk
											// 2 = Clk/hold
	m_bPropPolicy = TRUE;					// FALSE = properties only for current view
											// TRUE  = properties for all views
	m_bBlackLinesOrRemoveHidden = FALSE;	// TRUE  = draw black lines in case of GL_FILL, 
											//         remove hidden lines/points in case of GL_POINT/GL_LINE
	m_bScaleShowFrame = FALSE;				// TRUE  = show coordinate system (frame) if any 
	m_bScaleShowLabels = TRUE;				// TRUE  = show coordinate system (lables) if any 
	m_bScaleShowGrid = TRUE;				// TRUE  = show coordinate system (grid) if any 
	m_bScaleShowAxis = FALSE;				// TRUE  = show coordinate system (axis) if any 
	m_bReopenPolicy = FALSE;				// TRUE  = reopen first document of recent file list at startup
	m_nSerializeGraphBytes = 2;

	m_nOsRunning = GetOSRunning();
}

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp commands

BOOL CGlWinApp::InitInstance() 
{
	LoadOptions();
	return CWinApp::InitInstance();
}

int CGlWinApp::ExitInstance() 
{
	SaveOptions();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// LoadOptions
// Load from registry
/////////////////////////////////////////////////////////////////////////////
BOOL CGlWinApp::LoadOptions()
{
	unsigned int red,green,blue;
	CString facename;

	// Gl back color
	red = GetProfileInt("OpenGL back color","Red",GetRValue(m_OptionColorGlBack));
	green = GetProfileInt("OpenGL back color","Green",GetGValue(m_OptionColorGlBack));
	blue = GetProfileInt("OpenGL back color","Blue",GetBValue(m_OptionColorGlBack));
	m_OptionColorGlBack = RGB(red,green,blue);

	// Gl light ambient color
	red = GetProfileInt("OpenGL light ambient color","Red",GetRValue(m_OptionColorGlLightAmbient));
	green = GetProfileInt("OpenGL light ambient color","Green",GetGValue(m_OptionColorGlLightAmbient));
	blue = GetProfileInt("OpenGL light ambient color","Blue",GetBValue(m_OptionColorGlLightAmbient));
	m_OptionColorGlLightAmbient = RGB(red,green,blue);
	
	// Gl light diffuse color
	red = GetProfileInt("OpenGL light diffuse color","Red",GetRValue(m_OptionColorGlLightDiffuse));
	green = GetProfileInt("OpenGL light diffuse color","Green",GetGValue(m_OptionColorGlLightDiffuse));
	blue = GetProfileInt("OpenGL light diffuse color","Blue",GetBValue(m_OptionColorGlLightDiffuse));
	m_OptionColorGlLightDiffuse = RGB(red,green,blue);
	
	// Gl light specular color
	red = GetProfileInt("OpenGL light specular color","Red",GetRValue(m_OptionColorGlLightSpecular));
	green = GetProfileInt("OpenGL light specular color","Green",GetGValue(m_OptionColorGlLightSpecular));
	blue = GetProfileInt("OpenGL light specular color","Blue",GetBValue(m_OptionColorGlLightSpecular));
	m_OptionColorGlLightSpecular = RGB(red,green,blue);

	// Gl text color
	red = GetProfileInt("OpenGL text color","Red",GetRValue(m_OptionColorText));
	green = GetProfileInt("OpenGL text color","Green",GetGValue(m_OptionColorText));
	blue = GetProfileInt("OpenGL text color","Blue",GetBValue(m_OptionColorText));
	m_OptionColorText = RGB(red,green,blue);

	// LOGFONT
	m_LogFont.lfHeight = GetProfileInt("OpenGL text font","Height", m_LogFont.lfHeight);
	m_LogFont.lfWidth = GetProfileInt("OpenGL text font","Width", m_LogFont.lfWidth);
	m_LogFont.lfEscapement = GetProfileInt("OpenGL text font","Escapement", m_LogFont.lfEscapement);
	m_LogFont.lfOrientation = GetProfileInt("OpenGL text font","Orientation", m_LogFont.lfOrientation);
	m_LogFont.lfWeight = GetProfileInt("OpenGL text font","Weight", m_LogFont.lfWeight);
	m_LogFont.lfItalic = (BYTE)GetProfileInt("OpenGL text font","Italic", m_LogFont.lfItalic);
	m_LogFont.lfUnderline = (BYTE)GetProfileInt("OpenGL text font","Underline", m_LogFont.lfUnderline);
	m_LogFont.lfStrikeOut = (BYTE)GetProfileInt("OpenGL text font","StrikeOut", m_LogFont.lfStrikeOut);
	m_LogFont.lfCharSet = (BYTE)GetProfileInt("OpenGL text font","CharSet", m_LogFont.lfCharSet);
	m_LogFont.lfOutPrecision = (BYTE)GetProfileInt("OpenGL text font","OutPrecision", m_LogFont.lfOutPrecision);
	m_LogFont.lfClipPrecision = (BYTE)GetProfileInt("OpenGL text font","ClipPrecision", m_LogFont.lfClipPrecision);
	m_LogFont.lfQuality = (BYTE)GetProfileInt("OpenGL text font","Quality", m_LogFont.lfQuality);
	m_LogFont.lfPitchAndFamily = (BYTE)GetProfileInt("OpenGL text font","PitchAndFamily", m_LogFont.lfPitchAndFamily);
	facename = GetProfileString("OpenGL text font","FaceName", m_LogFont.lfFaceName);
	sprintf(m_LogFont.lfFaceName, facename);

	// Gl appearance
	m_bAntialias = GetProfileInt("OpenGL appearance","Antialias",m_bAntialias);
	m_bSmooth = GetProfileInt("OpenGL appearance","Smooth",m_bSmooth);
	m_bLighting = GetProfileInt("OpenGL appearance","Lighting",m_bLighting);
	m_nPolygonMode = GetProfileInt("OpenGL appearance","PolygonMode",m_nPolygonMode);
	m_bBlackLinesOrRemoveHidden = GetProfileInt("OpenGL appearance","BlackLines/RemoveHidden",m_bBlackLinesOrRemoveHidden);

	// Gl coordinates
	m_bScaleShowFrame = GetProfileInt("OpenGL coordinates","Frame",m_bScaleShowFrame);
	m_bScaleShowLabels = GetProfileInt("OpenGL coordinates","Labels",m_bScaleShowLabels);
	m_bScaleShowGrid = GetProfileInt("OpenGL coordinates","Grid",m_bScaleShowGrid);
	m_bScaleShowAxis = GetProfileInt("OpenGL coordinates","Axis",m_bScaleShowAxis);

	// Printing
	m_nMaxPrintMemSize = GetProfileInt("Printing","MaxPrintMemSize",m_nMaxPrintMemSize);
	m_nPrintObject = GetProfileInt("Printing","Object",m_nPrintObject);
	m_bPrintBkgndWhite = GetProfileInt("Printing","BkgndWhite",m_bPrintBkgndWhite);

	// Mouse, prop, save graph and reopen last document policy
	m_nMousePolicy = GetProfileInt("Properties","MousePolicy",m_nMousePolicy);
	m_bPropPolicy = GetProfileInt("Properties","PropPolicy",m_bPropPolicy);
	m_bReopenPolicy = GetProfileInt("Properties","ReopenLastDoc",m_bReopenPolicy);
	m_nSerializeGraphBytes = GetProfileInt("Properties","SaveGraphPolicy",m_nSerializeGraphBytes);

	// Is MainFrm zoomed?
	m_nCmdShow = SW_SHOWNORMAL + (SW_SHOWMAXIMIZED - SW_SHOWNORMAL) * GetProfileInt("Window","Zoomed",FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// SaveOptions
// Save to registry
/////////////////////////////////////////////////////////////////////////////
BOOL CGlWinApp::SaveOptions()
{
	unsigned char red,green,blue;

	// Gl back color
	red = GetRValue(m_OptionColorGlBack);
	green = GetGValue(m_OptionColorGlBack);
	blue = GetBValue(m_OptionColorGlBack);
	WriteProfileInt("OpenGL back color","Red",red);
	WriteProfileInt("OpenGL back color","Green",green);
	WriteProfileInt("OpenGL back color","Blue",blue);
	
	// Gl light ambient color
	red = GetRValue(m_OptionColorGlLightAmbient);
	green = GetGValue(m_OptionColorGlLightAmbient);
	blue = GetBValue(m_OptionColorGlLightAmbient);
	WriteProfileInt("OpenGL light ambient color","Red",red);
	WriteProfileInt("OpenGL light ambient color","Green",green);
	WriteProfileInt("OpenGL light ambient color","Blue",blue);
	
	// Gl light diffuse color
	red = GetRValue(m_OptionColorGlLightDiffuse);
	green = GetGValue(m_OptionColorGlLightDiffuse);
	blue = GetBValue(m_OptionColorGlLightDiffuse);
	WriteProfileInt("OpenGL light diffuse color","Red",red);
	WriteProfileInt("OpenGL light diffuse color","Green",green);
	WriteProfileInt("OpenGL light diffuse color","Blue",blue);
	
	// Gl light specular color
	red = GetRValue(m_OptionColorGlLightSpecular);
	green = GetGValue(m_OptionColorGlLightSpecular);
	blue = GetBValue(m_OptionColorGlLightSpecular);
	WriteProfileInt("OpenGL light specular color","Red",red);
	WriteProfileInt("OpenGL light specular color","Green",green);
	WriteProfileInt("OpenGL light specular color","Blue",blue);
	
	// Gl text color
	red = GetRValue(m_OptionColorText);
	green = GetGValue(m_OptionColorText);
	blue = GetBValue(m_OptionColorText);
	WriteProfileInt("OpenGL text color","Red",red);
	WriteProfileInt("OpenGL text color","Green",green);
	WriteProfileInt("OpenGL text color","Blue",blue);
	
	// LOGFONT
	WriteProfileInt("OpenGL text font","Height", m_LogFont.lfHeight);
	WriteProfileInt("OpenGL text font","Width", m_LogFont.lfWidth);
	WriteProfileInt("OpenGL text font","Escapement", m_LogFont.lfEscapement);
	WriteProfileInt("OpenGL text font","Orientation", m_LogFont.lfOrientation);
	WriteProfileInt("OpenGL text font","Weight", m_LogFont.lfWeight);
	WriteProfileInt("OpenGL text font","Italic", m_LogFont.lfItalic);
	WriteProfileInt("OpenGL text font","Underline", m_LogFont.lfUnderline);
	WriteProfileInt("OpenGL text font","StrikeOut", m_LogFont.lfStrikeOut);
	WriteProfileInt("OpenGL text font","CharSet", m_LogFont.lfCharSet);
	WriteProfileInt("OpenGL text font","OutPrecision", m_LogFont.lfOutPrecision);
	WriteProfileInt("OpenGL text font","ClipPrecision", m_LogFont.lfClipPrecision);
	WriteProfileInt("OpenGL text font","Quality", m_LogFont.lfQuality);
	WriteProfileInt("OpenGL text font","PitchAndFamily", m_LogFont.lfPitchAndFamily);
	WriteProfileString("OpenGL text font","FaceName", m_LogFont.lfFaceName);

	// Gl appearance
	WriteProfileInt("OpenGL appearance","Antialias",m_bAntialias);
	WriteProfileInt("OpenGL appearance","Smooth",m_bSmooth);
	WriteProfileInt("OpenGL appearance","Lighting",m_bLighting);
	WriteProfileInt("OpenGL appearance","PolygonMode",m_nPolygonMode);
	WriteProfileInt("OpenGL appearance","BlackLines/RemoveHidden",m_bBlackLinesOrRemoveHidden);

	// Gl coordinates
	WriteProfileInt("OpenGL coordinates","Frame",m_bScaleShowFrame);
	WriteProfileInt("OpenGL coordinates","Labels",m_bScaleShowLabels);
	WriteProfileInt("OpenGL coordinates","Grid",m_bScaleShowGrid);
	WriteProfileInt("OpenGL coordinates","Axis",m_bScaleShowAxis);

	// Printing
	WriteProfileInt("Printing","MaxPrintMemSize",m_nMaxPrintMemSize);
	WriteProfileInt("Printing","Object",m_nPrintObject);
	WriteProfileInt("Printing","BkgndWhite",m_bPrintBkgndWhite);

	// Mouse, prop, save graph and reopen last document policy
	WriteProfileInt("Properties","MousePolicy",m_nMousePolicy);
	WriteProfileInt("Properties","PropPolicy",m_bPropPolicy);
	WriteProfileInt("Properties","ReopenLastDoc",m_bReopenPolicy);
	WriteProfileInt("Properties","SaveGraphPolicy",m_nSerializeGraphBytes);

	return TRUE;
}

// this is a copy from DOCMGR.CPP 
//in order to overwrite CGlWinApp::RegisterShellFileTypes

static const TCHAR szShellOpenFmt[] = _T("%s\\shell\\open\\%s");
static const TCHAR szShellPrintFmt[] = _T("%s\\shell\\print\\%s");
static const TCHAR szShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
static const TCHAR szDefaultIconFmt[] = _T("%s\\DefaultIcon");
static const TCHAR szShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

static const TCHAR szIconIndexFmt[] = _T(",%d");
static const TCHAR szCommand[] = _T("command");
static const TCHAR szOpenArg[] = _T(" \"%1\"");
static const TCHAR szPrintArg[] = _T(" /p \"%1\"");
static const TCHAR szPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
static const TCHAR szDDEArg[] = _T(" /dde");

static const TCHAR szDDEExec[] = _T("ddeexec");
static const TCHAR szDDEOpen[] = _T("[open(\"%1\")]");
static const TCHAR szDDEPrint[] = _T("[print(\"%1\")]");
static const TCHAR szDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

static const TCHAR szShellNewValueName[] = _T("NullFile");
static const TCHAR szShellNewValue[] = _T("");

static BOOL AFXAPI SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if (::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

void CGlWinApp::RegisterShellFileTypes(BOOL bCompat, BOOL bOmitPrintCmd)
{
	ASSERT(m_pDocManager != NULL);
	if (!bCompat || !bOmitPrintCmd)
	{
		m_pDocManager->RegisterShellFileTypes(bCompat);
	}
	else
	{
		// similar to m_pDocManager->RegisterShellFileTypes(TRUE)
		// only the "Print" and "PrintTo" keys are omited
		ASSERT(GetFirstDocTemplatePosition() != NULL);  // must have some doc templates

		CString strPathName, strTemp;

		AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

		POSITION pos = GetFirstDocTemplatePosition();
		for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
		{
			CDocTemplate* pTemplate = GetNextDocTemplate(pos);

			CString strOpenCommandLine = strPathName;
			CString strDefaultIconCommandLine = strPathName;

			if (bCompat)
			{
				CString strIconIndex;
				HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), strPathName, nTemplateIndex);
				if (hIcon != NULL)
				{
					strIconIndex.Format(szIconIndexFmt, nTemplateIndex);
					DestroyIcon(hIcon);
				}
				else
				{
					strIconIndex.Format(szIconIndexFmt, DEFAULT_ICON_INDEX);
				}
				strDefaultIconCommandLine += strIconIndex;
			}

			CString strFilterExt, strFileTypeId, strFileTypeName;
			if (pTemplate->GetDocString(strFileTypeId,
			   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
			{
				// enough info to register it
				if (!pTemplate->GetDocString(strFileTypeName,
				   CDocTemplate::regFileTypeName))
					strFileTypeName = strFileTypeId;    // use id name

				ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

				// first register the type ID of our server
				if (!SetRegKey(strFileTypeId, strFileTypeName))
					continue;       // just skip it

				if (bCompat)
				{
					// path\DefaultIcon = path,1
					strTemp.Format(szDefaultIconFmt, (LPCTSTR)strFileTypeId);
					if (!SetRegKey(strTemp, strDefaultIconCommandLine))
						continue;       // just skip it
				}

				// If MDI Application
				if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
					strTemp.IsEmpty())
				{
					// path\shell\open\ddeexec = [open("%1")]
					strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)szDDEExec);
					if (!SetRegKey(strTemp, szDDEOpen))
						continue;       // just skip it

					if (bCompat)
					{
						// path\shell\open\command = path /dde
						strOpenCommandLine += szDDEArg;
					}
					else
					{
						strOpenCommandLine += szOpenArg;
					}
				}
				else
				{
					// path\shell\open\command = path filename
					strOpenCommandLine += szOpenArg;
				}

				// path\shell\open\command = path filename
				strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)szCommand);
				if (!SetRegKey(strTemp, strOpenCommandLine))
					continue;       // just skip it

				pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
				if (!strFilterExt.IsEmpty())
				{
					ASSERT(strFilterExt[0] == '.');

					LONG lSize = _MAX_PATH * 2;
					LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
						strTemp.GetBuffer(lSize), &lSize);
					strTemp.ReleaseBuffer();

					if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
						strTemp == strFileTypeId)
					{
						// no association for that suffix
						if (!SetRegKey(strFilterExt, strFileTypeId))
							continue;

						if (bCompat)
						{
							strTemp.Format(szShellNewFmt, (LPCTSTR)strFilterExt);
							(void)SetRegKey(strTemp, szShellNewValue, szShellNewValueName);
						}
					}
				}
			}
		}
	}
}

void CGlWinApp::OnFileOpen() 
{
	CString newName;

	DWORD lFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		lFlags |= OFN_ALLOWMULTISELECT;

	// prompt the user (with all document templates)
	CFileDialog newDlg(TRUE);
	if (!DoPromptFileName(newDlg, AFX_IDS_OPENFILE,
	  lFlags, NULL))
		return; // open cancelled

	POSITION pos = newDlg.GetStartPosition();
	while (pos != NULL)
	{
		newName = newDlg.GetNextPathName(pos);
		OpenDocumentFile(newName);
	}
		// if returns NULL, the user has already been alerted
}

static void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL CGlWinApp::DoPromptFileName(CFileDialog& dlgFile, UINT nIDSTitle, DWORD lFlags, CDocTemplate* pTemplate)
{
	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		ASSERT(m_pDocManager != NULL);

		// do for all doc template
		POSITION pos = GetFirstDocTemplatePosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = GetNextDocTemplate(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	int nResult = dlgFile.DoModal();
	return nResult == IDOK;
}

void CGlWinApp::OnHelpContents()
{
	WinHelp(0L, HELP_CONTENTS);
}

void CGlWinApp::OnFilePrintSetupEx() 
{
	// TODO: Add your command handler code here
	CDlgFilePrintOptions dlg;
	int tmpInt;
	BOOL tmpBOOL;

	dlg.m_nMaxPrintMemSize = m_nMaxPrintMemSize;
	dlg.m_nPrintObject = m_nPrintObject;
	dlg.m_bPrintBkgndWhite = m_bPrintBkgndWhite;
	if (dlg.DoModal() == IDOK)
	{
		tmpInt = dlg.m_nMaxPrintMemSize;
		if (m_nMaxPrintMemSize != tmpInt)
		{
			m_nMaxPrintMemSize = tmpInt;
		}
		tmpInt = dlg.m_nPrintObject;
		if (m_nPrintObject != tmpInt)
		{
			m_nPrintObject = tmpInt;
		}
		tmpBOOL = dlg.m_bPrintBkgndWhite;
		if (m_bPrintBkgndWhite != tmpBOOL)
		{
			m_bPrintBkgndWhite = tmpBOOL;
		}
	}
}

void CGlWinApp::OnChangeMousePolicy()
{
	m_nMousePolicy = ++m_nMousePolicy - 3*(m_nMousePolicy/3);
	SetStatusBarInfo(POLICY_MOUSE, m_nMousePolicy);
}

void CGlWinApp::OnChangePropPolicy() 
{
	m_bPropPolicy = !m_bPropPolicy;
	SetStatusBarInfo(POLICY_PROP, m_bPropPolicy);
}

void CGlWinApp::OnChangeReopenPolicy() 
{
	m_bReopenPolicy = !m_bReopenPolicy;
	SetStatusBarInfo(POLICY_REOPEN, m_bReopenPolicy);
}

void CGlWinApp::OnSerializeGraphOneByte() 
{
	m_nSerializeGraphBytes = 1;
	SetStatusBarInfo(POLICY_SAVE_GRAPH, m_nSerializeGraphBytes);
}

void CGlWinApp::OnSerializeGraphTwoBytes() 
{
	m_nSerializeGraphBytes = 2;
	SetStatusBarInfo(POLICY_SAVE_GRAPH, m_nSerializeGraphBytes);
}

void CGlWinApp::OnSerializeGraphFourBytes() 
{
	m_nSerializeGraphBytes = 4;
	SetStatusBarInfo(POLICY_SAVE_GRAPH, m_nSerializeGraphBytes);
}

void CGlWinApp::OnSerializeGraphEightBytes() 
{
	m_nSerializeGraphBytes = 8;
	SetStatusBarInfo(POLICY_SAVE_GRAPH, m_nSerializeGraphBytes);
}

void CGlWinApp::OnChangePolicy() 
{
	CDlgOptions dlg;

	dlg.m_nRadionMousePolicy = m_nMousePolicy;
	dlg.m_bCheckPropPolicy = m_bPropPolicy;
	dlg.m_bCheckReopenPolicy = m_bReopenPolicy;
	if (m_nSerializeGraphBytes == 1)
		dlg.m_nRadioSerializeGraphBytes = 0;
	else if (m_nSerializeGraphBytes == 2)
		dlg.m_nRadioSerializeGraphBytes = 1;
	else if (m_nSerializeGraphBytes == 4)
		dlg.m_nRadioSerializeGraphBytes = 2;
	else if (m_nSerializeGraphBytes == 8)
		dlg.m_nRadioSerializeGraphBytes = 3;
	else
		dlg.m_nRadioSerializeGraphBytes = -1;

	if (dlg.DoModal() == IDOK)
	{
		int tmpInt;
		BOOL tmpBOOL;

		tmpInt = dlg.m_nRadionMousePolicy;
		if (m_nMousePolicy != tmpInt)
		{
			m_nMousePolicy = tmpInt;
		}
		tmpBOOL = dlg.m_bCheckPropPolicy;
		if (m_bPropPolicy != tmpBOOL)
		{
			m_bPropPolicy = tmpBOOL;
		}
		tmpBOOL = dlg.m_bCheckReopenPolicy;
		if (m_bReopenPolicy != tmpBOOL)
		{
			m_bReopenPolicy = tmpBOOL;
		}
		tmpInt = dlg.m_nRadioSerializeGraphBytes;
		if (tmpInt == -1)
		{
			m_nSerializeGraphBytes = 0;
		}
		else if (tmpInt == 0)
		{
			m_nSerializeGraphBytes = 1;
		}
		else if (tmpInt == 1)
		{
			m_nSerializeGraphBytes = 2;
		}
		else if (tmpInt == 2)
		{
			m_nSerializeGraphBytes = 4;
		}
		else if (tmpInt == 3)
		{
			m_nSerializeGraphBytes = 8;
		}
	}
}

/*void CGlWinApp::OnShowPolicy() 
{
	CString msg;
	if (m_nSerializeGraphBytes > 0)
		msg.Format("Linke Maustaste: \t\t%s\nEigenschaften: \t\t%s\nLetztes Dokument: \t\t%s\nSpeichern mit Graphik: \t%d Byte%s", 
			m_nMousePolicy == 0 ? "Doppelklick/Klick" : (m_nMousePolicy == 1 ? "Klick/Klick" : "Klick+Halten/Loslassen"), 
			m_bPropPolicy ? "Alle Fenster" : "Aktuelles Fenster", 
			m_bReopenPolicy ? "Erneut öffnen" : "Nicht erneut öffnen",
			m_nSerializeGraphBytes,
			m_nSerializeGraphBytes > 1 ? "s" : "");
	else
		msg.Format("Linke Maustaste: \t\t%s\nEigenschaften: \t\t%s\nLetztes Dokument: \t\t%s", 
			m_nMousePolicy == 0 ? "Doppelklick/Klick" : (m_nMousePolicy == 1 ? "Klick/Klick" : "Klick+Halten/Loslassen"), 
			m_bPropPolicy ? "Alle Fenster" : "Aktuelles Fenster",
			m_bReopenPolicy ? "Erneut öffnen" : "Nicht erneut öffnen");

	AfxGetMainWnd()->MessageBox(msg, "Aktuelle Einstellungen", MB_ICONINFORMATION);
}*/

void CGlWinApp::SetStatusBarInfo(int nInfoMajor, int nInfoMinor) 
{
	CFrameWnd* pMainFrame;
	if (m_pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		pMainFrame = (CFrameWnd *)m_pMainWnd;
	else
		return;

	if (nInfoMajor == POLICY_NO_MORE_ZOOM)
	{
		pMainFrame->SetMessageText(IDS_ERR_MAXZOOM);
	}
	else if (nInfoMajor == POLICY_MOUSE)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_LINKMAUS_DBLKLIK);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_LINKMAUS_KLIK);
		}
		else if (nInfoMinor == 2)
		{
			pMainFrame->SetMessageText(IDS_LINKMAUS_KLIKHALT);
		}
	}
	else if (nInfoMajor == POLICY_PROP)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_EIGEN_AKTUEL);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_EIGEN_ALLES);
		}
	}
	else if (nInfoMajor == POLICY_REOPEN)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_REOPEN_NONE);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_REOPEN_FIRST);
		}
	}
	else if (nInfoMajor == POLICY_SAVE_GRAPH)
	{
		if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_GRAFPREC_1BYTE);
		}
		else if (nInfoMinor == 2)
		{
			pMainFrame->SetMessageText(IDS_GRAFPREC_2BYTE);
		}
		else if (nInfoMinor == 4)
		{
			pMainFrame->SetMessageText(IDS_GRAFPREC_4BYTE);
		}
		else if (nInfoMinor == 8)
		{
			pMainFrame->SetMessageText(IDS_GRAFPREC_8BYTE);
		}
	}
	pMainFrame->SetTimer(AFX_IDS_IDLEMESSAGE, 2000, NULL);
}

void CGlWinApp::OnFilePrintEx() 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
				}
			}
		}
		break;
	case 1:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					pFirstGlView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
			}
		}
		break;
	case 2:
		DoFilePrintAll();
		break;
	default:
		{}
	}
}

void CGlWinApp::OnFilePrintPreviewEx() 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pView->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
				}
			}
		}
		break;
	case 1:
	case 2:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					pFirstGlView->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
			}
		}
		break;
	default:
		{}
	}
}

void CGlWinApp::OnUpdateFilePrintEx(CCmdUI* pCmdUI) 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;
	BOOL bEnable = FALSE;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					bEnable = TRUE;
				}
			}
		}
		break;
	case 1:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					bEnable = TRUE;
			}
		}
		break;
	case 2:
		bEnable = TRUE;
		break;
	default:
		{}
	}
	pCmdUI->Enable(bEnable);		
}

void CGlWinApp::OnUpdateFilePrintPreviewEx(CCmdUI* pCmdUI) 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;
	BOOL bEnable = FALSE;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					bEnable = TRUE;
				}
			}
		}
		break;
	case 1:
	case 2:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					bEnable = TRUE;
			}
		}
		break;
	default:
		{}
	}
	pCmdUI->Enable(bEnable);		
}

BOOL CGlWinApp::BeginPrinting(CDC* pDC) 
{

	// get the screen dc
	HDC hScreenDC = (HDC)0;
	hScreenDC = ::GetDC(NULL);
	ASSERT(hScreenDC != NULL);

	CDC ScreenDC;
	VERIFY(ScreenDC.Attach(hScreenDC));

	// create screen device dependent bitmap (DDB)
	int nScreenWidth = 0;
	nScreenWidth = ScreenDC.GetDeviceCaps(HORZRES);

	int nScreenHeight = 0;
	nScreenHeight = ScreenDC.GetDeviceCaps(VERTRES);

	CRect& mRect = m_Rect;
	AfxGetMainWnd()->GetWindowRect(&mRect);
	if (mRect.left < 0)
		mRect.left = 0;
	if (mRect.right > nScreenWidth)
		mRect.right = nScreenWidth;
	if (mRect.top < 0)
		mRect.top = 0;
	if (mRect.bottom > nScreenHeight)
		mRect.bottom = nScreenHeight;

	CBitmap ScreenBitmap;
	VERIFY(ScreenBitmap.CreateCompatibleBitmap(&ScreenDC, 
												/*nScreenWidth*/mRect.Width(), 
												/*nScreenHeight*/mRect.Height()));

	// copy image to screen DDB
	CDC MemDC;
	VERIFY(MemDC.CreateCompatibleDC(&ScreenDC));

	CBitmap* pOldBitmap = NULL;
	pOldBitmap = MemDC.SelectObject(&ScreenBitmap);

	// else could not select bitmap
	if (pOldBitmap == NULL)
	{
		// abort job
		VERIFY(pDC->AbortDoc() >= 0);
		return FALSE;
	}

	VERIFY(MemDC.BitBlt(0, 
						0, 
						/*nScreenWidth*/mRect.Width(), 
						/*nScreenHeight*/mRect.Height(),
						&ScreenDC, 
						/*0*/mRect.left, 
						/*0*/mRect.top, 
						SRCCOPY));
	ASSERT(pOldBitmap != NULL);
	MemDC.SelectObject(pOldBitmap);

	// create device independent bitmap (DIB)
	const int nBitCount = 24;
	long nImageSize = 0;
	nImageSize = ((((/*nScreenWidth*/mRect.Width() * nBitCount) + 31) & ~31) >> 3) * /*nScreenHeight*/mRect.Height();

	BYTE*& pBitmapBits = m_pBitmapBits;
	pBitmapBits = NULL;
	pBitmapBits = new BYTE[nImageSize];
	ASSERT(pBitmapBits != NULL);

	// copy image from screen DDB to DIB
	BITMAPINFO& bitmapInfo = m_bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = /*nScreenWidth*/mRect.Width();
	bitmapInfo.bmiHeader.biHeight = /*nScreenHeight*/mRect.Height();
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = nBitCount;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage = nImageSize;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 2840;	// 72 DPI 
	bitmapInfo.bmiHeader.biYPelsPerMeter = 2840;	// 72 DPI 
	bitmapInfo.bmiHeader.biClrUsed = 0; 
	bitmapInfo.bmiHeader.biClrImportant = 0; 
	memset(&bitmapInfo.bmiColors[0], 0, sizeof(RGBQUAD));

	VERIFY(GetDIBits(ScreenDC.GetSafeHdc(),
					(HBITMAP)ScreenBitmap,
					0,
					/*nScreenHeight*/mRect.Height(),
					(LPVOID)pBitmapBits,
					&bitmapInfo,
					DIB_RGB_COLORS));

	VERIFY(ScreenBitmap.DeleteObject());
	VERIFY(MemDC.DeleteDC());
	ScreenDC.Detach();

	return TRUE;
}

BOOL CGlWinApp::Print(CDC* pDC) 
{
	CRect& mRect = m_Rect;
	BITMAPINFO& bitmapInfo = m_bitmapInfo;
	BYTE*& pBitmapBits = m_pBitmapBits;

	// copy DIB to printer
	int nPrinterWidth = 0;
	nPrinterWidth = pDC->GetDeviceCaps(HORZRES);

	int nPrinterHeight = 0;
	nPrinterHeight = pDC->GetDeviceCaps(VERTRES);

	double fac = 1.;
	if (mRect.Width() <= nPrinterWidth && mRect.Height() <= nPrinterHeight)
	{
		// good condition
		while ((int)fac*mRect.Width() <= nPrinterWidth && (int)fac*mRect.Height() <= nPrinterHeight)
		{
			fac *= 2.;
		}
		fac /= 2.;
		nPrinterWidth = int(mRect.Width()*fac);
		nPrinterHeight = int(mRect.Height()*fac);
	}
	else
	{
		// bad condition
		while (mRect.Width() > (int)fac*nPrinterWidth || mRect.Height() > (int)fac*nPrinterHeight)
		{
			fac *= 2.;
		}
		nPrinterWidth = int(mRect.Width()/fac);
		nPrinterHeight = int(mRect.Height()/fac);
	}
	int nImageOffsetHorz = (pDC->GetDeviceCaps(HORZRES) - nPrinterWidth) / 2;
	int nImageOffsetVert = (pDC->GetDeviceCaps(VERTRES) - nPrinterHeight) / 2;

	BOOL bStretchDIBits, bStretchBlt;
	int nRasterCaps	= pDC->GetDeviceCaps(RASTERCAPS);

	// Does the device support raster operations?
	if (nRasterCaps & RC_STRETCHDIB)
		bStretchDIBits = TRUE;
	else 
		bStretchDIBits = FALSE;

	if (nRasterCaps & RC_STRETCHBLT)
		bStretchBlt = TRUE;
	else 
		bStretchBlt = FALSE;

	// Does our device context support StretchDIBits?
	if (bStretchDIBits)
	{
		VERIFY(StretchDIBits(pDC->GetSafeHdc(),
							nImageOffsetHorz,
							nImageOffsetVert,
							nPrinterWidth,
							nPrinterHeight,
							0,
							0,
							bitmapInfo.bmiHeader.biWidth,
							bitmapInfo.bmiHeader.biHeight,
							(LPVOID)pBitmapBits,
							&bitmapInfo,
							DIB_RGB_COLORS,
							SRCCOPY) != GDI_ERROR);
	}
	else if (bStretchBlt)
	{
		HDC hdc;
		HBITMAP bitmap;

		hdc	= CreateCompatibleDC(pDC->GetSafeHdc());
		bitmap = CreateDIBitmap(hdc,
							&(bitmapInfo.bmiHeader),
							CBM_INIT,
							(LPVOID)pBitmapBits,
							&bitmapInfo,
							DIB_RGB_COLORS);
		
		HGDIOBJ oldbitmap = SelectObject(hdc, bitmap);
		VERIFY(StretchBlt(pDC->GetSafeHdc(),
							nImageOffsetHorz,
							nImageOffsetVert,
							nPrinterWidth,
							nPrinterHeight,
							hdc,
							0,
							0,
							bitmapInfo.bmiHeader.biWidth,
							bitmapInfo.bmiHeader.biHeight,
							SRCCOPY) != GDI_ERROR);
		if (oldbitmap != NULL)
			SelectObject(hdc, oldbitmap);

		DeleteObject(bitmap);
		DeleteDC(hdc);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGlWinApp::EndPrinting(CDC*) 
{
	BYTE*& pBitmapBits = m_pBitmapBits;

	// clean up
	if (pBitmapBits)
	{
		delete pBitmapBits;
		pBitmapBits = NULL;
		return TRUE;
	}
	return FALSE;
}

void CGlWinApp::DoFilePrintAll() 
{
	// show options
	CPrintDialog PrintDlg(FALSE, 
						PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_NOSELECTION |
						PD_DISABLEPRINTTOFILE, 
						AfxGetMainWnd()/*this*/);

	if (DoPrintDialog(&PrintDlg) == IDOK) 
	{
		BeginWaitCursor();
		AfxGetMainWnd()->SendMessageToDescendants(WM_PAINT);

		// get printer dc
		HDC hPrinterDC = (HDC)0;
		hPrinterDC = PrintDlg.m_pd.hDC;
		ASSERT(hPrinterDC != NULL);

		CDC PrinterDC;
		BOOL bIsAttached = FALSE;
		bIsAttached = PrinterDC.Attach(hPrinterDC);
		ASSERT(bIsAttached);

		// attempt to set abort procedure
		int nSetAbortProcStatus = 0;
		nSetAbortProcStatus = PrinterDC.SetAbortProc(&CGlWinApp::AbortProc);

		// attempt to start print job
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof (DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszOutput = NULL;
		docInfo.lpszDocName = AfxGetAppName();
			   
		int nStartDocStatus = 0;
		nStartDocStatus = PrinterDC.StartDoc(&docInfo);

		// if abort procedure set and print job started
		if (nSetAbortProcStatus >= 0 && nStartDocStatus >= 0) 
		{
			// start page
			VERIFY(PrinterDC.StartPage() > 0);

			if (BeginPrinting(&PrinterDC))
			{
				if (!Print(&PrinterDC))
					AfxMessageBox(IDS_ERR_PRINTERFNC, MB_OK | MB_ICONERROR);
				EndPrinting(&PrinterDC);
			}
			else
				AfxMessageBox(IDS_ERR_PRINTNOMEM, MB_OK | MB_ICONERROR);

			// finish the print job
			VERIFY(PrinterDC.EndPage() >= 0);
		}

		// else job could not be started
		else 
		{
			// attempt to abort job
			PrinterDC.AbortDoc();
			AfxMessageBox(IDS_ERR_PRINTNOSTART, MB_OK | MB_ICONERROR);
		}
		VERIFY(PrinterDC.EndDoc() >= 0);

		// clean up
		PrinterDC.Detach();
		VERIFY(DeleteDC(hPrinterDC));
		EndWaitCursor();
	}
}

BOOL CALLBACK CGlWinApp::AbortProc(HDC, int iCode)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (iCode == 0);
}
