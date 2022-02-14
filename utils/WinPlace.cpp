////////////////////////////////////////////////////////////////
// CWindowPlacement Copyright 1996 Microsoft Systems Journal.
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Modified 25 Apr 1997 Morris Maynard
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 Mayn Idea Software
// All rights reserved
/////////////////////////////////////////////////////////////////////////////
/*
* $History: WinPlace.cpp $ * 
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 11/05/03   Time: 10:53a
 * Created in $/mmlib/utils
*/

#include "stdafx.h"
#include "winplace.h"

const TCHAR FormatWP[] = "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
const TCHAR txtSection[] = _T("Window Positions");
const TCHAR g_szDefault[] = _T("Main Window");
const NFIELDS = 10;
const POINT pt0 = { 0, 0 };
const RECT rc0 = { 0, 0, 0, 0 };

CWindowPlacement::CWindowPlacement()
{
	// Note: all these fields are inherited from WINDOWPLACEMENT
	length = sizeof(WINDOWPLACEMENT);
  flags = SWP_NOZORDER | SWP_SHOWWINDOW;
	ptMinPosition = pt0;
	ptMinPosition = pt0;
	ptMaxPosition = pt0;
	ptMaxPosition = pt0;
	rcNormalPosition = rc0;
	rcNormalPosition = rc0;
	rcNormalPosition = rc0;
	rcNormalPosition = rc0;
}

CWindowPlacement::~CWindowPlacement()
{
}

static void SetWPosition(const CRect& rcNew, CWnd *pWnd, WINDOWPLACEMENT *wp)
{
  wp->rcNormalPosition = rcNew;
  wp->ptMaxPosition = CPoint(0, 0);
  pWnd->SetWindowPlacement(wp);
}
//////////////////
// Restore window placement from profile key
//
BOOL CWindowPlacement::Restore(UINT nEntry, CWnd* pWnd, BOOL bDialog, BOOL bMaxDefault)
{
  CString strSection = txtSection, strEntry;
  strEntry.LoadString(nEntry);

  if (strEntry.IsEmpty())
    strEntry = g_szDefault;

  CRect rcNew;
  pWnd->GetWindowRect(rcNew);

	if (!GetProfileWP(strSection, strEntry)) // read registry into data members
    {                                      // if there was no previous entry
    if (!bMaxDefault)
      showCmd = SW_SHOW;               // normal case, default to normal
    else
      showCmd = SW_SHOWMAXIMIZED;      // brain-dead marketing case, default to max

    SetWPosition(rcNew, pWnd, this);
    WriteProfileWP(strSection, strEntry);

    return FALSE;
    }

	// Only restore if window intersects the screen.
	//
	CRect rcTemp, rcScreen;
  CWnd::GetDesktopWindow()->GetWindowRect(rcScreen);
  if (!rcTemp.IntersectRect(&rcNormalPosition, rcScreen))
    {
    showCmd = SW_SHOW;
    SetWPosition(rcNew, pWnd, this);
		return FALSE;
    }

  // handle case where dialog dimensions have changed, either
  // because of code changes or screen resolution change
  CRect rcOld(rcNormalPosition);
	if (bDialog && (rcNew.Width() != rcOld.Width() || 
    rcNew.Height() != rcOld.Height()))
    {
    rcOld.TopLeft() = rcScreen.TopLeft();
    rcOld.BottomRight() = rcOld.TopLeft() + CPoint(rcNew.Width(), rcNew.Height());
    rcOld.OffsetRect(rcScreen.Width() / 2, rcScreen.Height() / 2);
    rcNormalPosition = rcOld;
    }
  else if (showCmd == SW_SHOWMAXIMIZED)
  // if the saved window state was maximized, save
  // a position/size to use when the Restore system menu
  // command is next issued. 
    rcNormalPosition = rcNew; 

	pWnd->SetWindowPlacement(this);	// set placement
  // Now save the current state but replace the
  // command with "restore". The caller can 
  // then call the Restore function here the
  // next time the user chooses the Restore
  // command (only needs doing the first time
  // Restore is chosen when the orignal saved
  // position/size was Maximized)
  showCmd = SW_RESTORE;
  WriteProfileWP(strSection, strEntry);
	return TRUE;
}

//////////////////
// Get window placement from profile.
//
BOOL CWindowPlacement::GetProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s = pApp->GetProfileString(lpszKey, lpszVal);
	if (s.IsEmpty())
		return FALSE;

	// scanf is overkill, but I'm lazy
	return sscanf((LPCTSTR)s, FormatWP,
		&showCmd,
		&flags,
		&ptMinPosition.x,
		&ptMinPosition.y,
		&ptMaxPosition.x,
		&ptMaxPosition.y,
		&rcNormalPosition.left,
		&rcNormalPosition.right,
		&rcNormalPosition.top,
		&rcNormalPosition.bottom) == NFIELDS;
}

////////////////
// Save window placement in app profile
//
void CWindowPlacement::Save(UINT nEntry, CWnd* pWnd)
{
  CString strSection = txtSection, strEntry;
  strEntry.LoadString(nEntry);
  if (strEntry.IsEmpty())
    strEntry = g_szDefault;
	pWnd->GetWindowPlacement(this);
	WriteProfileWP(strSection, strEntry);
}

//////////////////
// Write window placement to app profile
//
BOOL CWindowPlacement::WriteProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s;
	s.Format(FormatWP,
		showCmd,
		flags,
		ptMinPosition.x,
		ptMinPosition.y,
		ptMaxPosition.x,
		ptMaxPosition.y,
		rcNormalPosition.left,
		rcNormalPosition.right,
		rcNormalPosition.top,
		rcNormalPosition.bottom);
	return pApp->WriteProfileString(lpszKey, lpszVal, s);
}

// The ugly casts are required to help the VC++ 3.0 compiler decide which
// operator<< or operator>> to use. If you're using VC++ 4.0 or later, you 
// can delete this stuff.
//
#if (_MSC_VER < 0x400) // VC++ 4.0
#define UINT_CAST (LONG)
#define UINT_CASTREF (LONG&)
#else
#define UINT_CAST
#define UINT_CASTREF
#endif

//////////////////
// Write window placement to archive
// WARNING: archiving functions are untested.
//
CArchive& operator<<(CArchive& ar, const CWindowPlacement& wp)
{
	ar << UINT_CAST wp.length;
   ar << UINT_CAST wp.flags;
	ar << UINT_CAST wp.showCmd;
   ar << wp.ptMinPosition;
   ar << wp.ptMaxPosition;
   ar << wp.rcNormalPosition;
	return ar;
}

//////////////////
// Read window placement from archive
// WARNING: archiving functions are untested.
//
CArchive& operator>>(CArchive& ar, CWindowPlacement& wp)
{
	ar >> UINT_CASTREF wp.length;
   ar >> UINT_CASTREF wp.flags;
	ar >> UINT_CASTREF wp.showCmd;
   ar >> wp.ptMinPosition;
   ar >> wp.ptMaxPosition;
   ar >> wp.rcNormalPosition;
	return ar;
}
