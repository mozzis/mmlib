/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: August 1998
//
/////////////////////////////////////////////////////////////////////////////
// StringEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringEx

CStringEx::CStringEx()
{
}

CStringEx::~CStringEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// CStringEx Kommandos
void CStringEx::PubFormatV(LPCTSTR lpszFormat, va_list argList)
{
	CString::FormatV(lpszFormat, argList);
}

/////////////////////////////////////////////////////////////////////////////
// Globale Funktionen
void AFX_CDECL mprintf(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat, FALSE));
	CStringEx msg;

	va_list argList;
	va_start(argList, lpszFormat);
	msg.PubFormatV(lpszFormat, argList);
	va_end(argList);

	AfxMessageBox(msg);
}
