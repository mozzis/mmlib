/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: June 1999
//
// based on a sample of
// John McTainsh <john.mctainsh@printrak.com>
//
/////////////////////////////////////////////////////////////////////////////
// Version.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// Extract version information from a file.
// Note: only the fist language data is returned.
//
// CREATED:
// 28-5-1999  10:50:39
//
// PARAMS:
// szFilePath File name to extract version info from. "C:\\WINNT\\Explorer.exe"
//		For the current app use
//		GetModuleFileName(AfxGetInstanceHandle(), szFullPath, sizeof(szFullPath));
// szSection Version section data. "ProductVersion"
//		See also <Resourcefile> -> Version.
// psData  Data read, OR error message.
//
// RETURN:
// True if all went well.

BOOL GetVersionInfo(LPTSTR szFilePath, LPCTSTR szSection, CString* const psData)
{
	// Inital parameters
	DWORD dwHandle;				// Set to Zero (Unused)
	DWORD dwInfoSize;			// Size of the Info Structure.
	UINT uiLanguageSize;		// Size of language buffer
	UINT uiSize;				// Size of Data recieved with version information
	DWORD const* pdwLanguages;

	// Retrieve the File information
	if (0 != (dwInfoSize = ::GetFileVersionInfoSize(szFilePath, &dwHandle)))
	{
		// Allocate the memory
		BYTE *const pbData = new BYTE[dwInfoSize];
		if (!pbData)
		{
			*psData = _T("Nicht genügend Speicher.");
			return FALSE;
		}

		// Get the Version data
		if (::GetFileVersionInfo(szFilePath, dwHandle, dwInfoSize, pbData))
		{
			psData->Empty();
			LPCTSTR szData = NULL;	 // Pointer to where data will be located

			// Get the translation information. (Language)
			if (::VerQueryValue(pbData, _T("\\VarFileInfo\\Translation"), (void**)&pdwLanguages, &uiLanguageSize))
			{
				if (uiLanguageSize)
				{
					TCHAR szSubBlock[512];   // Version language and section definition
					_stprintf(szSubBlock, _T("\\StringFileInfo\\%04hX%04hX\\%s"), 
						LOWORD(*pdwLanguages), HIWORD(*pdwLanguages), szSection);
					::VerQueryValue(pbData, szSubBlock, (void**)&szData, &uiSize);
					*psData = szData;
				}
			}

			// This is a happy ending
			delete [] pbData;
			return TRUE;
		}
		else
		{
			*psData = _T("Kann Versionsinformation nicht extrahieren.");
			delete [] pbData;
			return FALSE;
		}
	}

	// Failed to locate file or read version data
	*psData = _T("Keine Versionsinformation.");
	return FALSE;
}

BOOL GetVersionInfo(LPTSTR szFilePath, VS_FIXEDFILEINFO* const pInfo)
{
	// Inital parameters
	DWORD dwHandle;				// Set to Zero (Unused)
	DWORD dwInfoSize;			// Size of the Info Structure.
	UINT uiSize;				// Size of Data recieved with version information

	// Retrieve the File information
	if (0 != (dwInfoSize = ::GetFileVersionInfoSize(szFilePath, &dwHandle)))
	{
		// Allocate the memory
		BYTE *const pbData = new BYTE[dwInfoSize];
		if (!pbData)
		{
			TRACE("Nicht genügend Speicher.");
			return FALSE;
		}

		VS_FIXEDFILEINFO* pData = NULL;	 // Pointer to where data will be located
		// Get the Version data
		if (::GetFileVersionInfo(szFilePath, dwHandle, dwInfoSize, pbData) && 
			::VerQueryValue(pbData, _T("\\"), (void**)&pData, &uiSize))
		{
			// This is a happy ending
			memcpy(pInfo, pData, sizeof(VS_FIXEDFILEINFO));
			delete [] pbData;
			return TRUE;
		}
		else
		{
			TRACE("Kann Versionsinformation nicht extrahieren.");
			delete [] pbData;
			return FALSE;
		}
	}

	// Failed to locate file or read version data
	TRACE("Keine Versionsinformation.");
	return FALSE;
}
