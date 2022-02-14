/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: August 1998
//
/////////////////////////////////////////////////////////////////////////////
// StringEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_)
#define AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class AFX_EXT_CLASS CStringEx : public CString
{
// Konstruktion
public:
	CStringEx();

// Attribute
public:

// Operationen
public:
	void PubFormatV(LPCTSTR lpszFormat, va_list argList);

// Implementierung
public:
	virtual ~CStringEx();
};

/////////////////////////////////////////////////////////////////////////////

void AFX_EXT_API AFX_CDECL mprintf(LPCTSTR lpszFormat, ...);
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_)
