/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// StdAfx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__06A3562B_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_STDAFX_H__06A3562B_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxtempl.h>       // Template support
#include <afxpriv.h>		// to see the CPreviewDC structure
#include <float.h>			// to see FLT_DIG/DBL_DIG

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>        // MFC OLE-Automatisierungsklassen
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

/////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <math.h>

#include <gl\gl.h>
#include <gl\glu.h>

/////////////////////////////////////////////////////////////////////////////
// This block is a copy from the MFC source. It contains
//	special exception handling just for MFC library implementation
#ifndef _AFX_OLD_EXCEPTIONS

// MFC does not rely on auto-delete semantics of the TRY..CATCH macros,
//  therefore those macros are mapped to something closer to the native
//  C++ exception handling mechanism when building MFC itself.

#undef TRY
#define TRY { try {

#undef CATCH
#define CATCH(class, e) } catch (class* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef AND_CATCH
#define AND_CATCH(class, e) } catch (class* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef CATCH_ALL
#define CATCH_ALL(e) } catch (CException* e) \
	{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef AND_CATCH_ALL
#define AND_CATCH_ALL(e) } catch (CException* e) \
	{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef END_TRY
#define END_TRY } catch (CException* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); e->Delete(); } }

#undef THROW_LAST
#define THROW_LAST() throw

// Because of the above definitions of TRY...CATCH it is necessary to
//  explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
#define NO_CPP_EXCEPTION(expr)

#else   //!_AFX_OLD_EXCEPTIONS

// In this case, the TRY..CATCH macros provide auto-delete semantics, so
//  it is not necessary to explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e)
#define NO_CPP_EXCEPTION(expr) expr

#endif  //_AFX_OLD_EXCEPTIONS


#ifndef _countof

// _countof is defined in <afximpl.h> which is not included in the WinNT
//  MSVC installation. So we define it here.

// determine number of elements in an array (not bytes)
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#endif  //_countof


#ifndef DLGTEMPLATEEX

// _countof is defined in <afximpl.h> which is not included in the WinNT
//  MSVC installation. So we define it here.
typedef struct
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;
typedef DLGTEMPLATEEX *LPDLGTEMPLATEEXA;
typedef DLGTEMPLATEEX *LPDLGTEMPLATEEXW;
#ifdef UNICODE
typedef LPDLGTEMPLATEEXW LPDLGTEMPLATEEX;
#else
typedef LPDLGTEMPLATEEXA LPDLGTEMPLATEEX;
#endif // UNICODE

#endif  //DLGTEMPLATEEX

/////////////////////////////////////////////////////////////////////////////

#include "MathAdd.h"		// epsilon() cannot be declared here
#include "StringEx.h"		// add mprintf to global namespace
#include "mathglobals.h" // epsilon et al.

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STDAFX_H__37EC43EF_7E06_11D2_8A55_00E0290F649D__INCLUDED_)
