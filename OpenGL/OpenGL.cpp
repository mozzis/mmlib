/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: November 1998
//
/////////////////////////////////////////////////////////////////////////////
// OpenGL.cpp : Legt die Initialisierungsroutinen f�r die DLL fest.
//

#include "StdAfx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE OpenGLDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// See comment below.
		//
		//#ifdef _DEBUG
		//TRACE0("GENERAL32D.DLL Initializing!\n");
		//#endif //_DEBUG
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(OpenGLDLL, hInstance))
			return 0;

		// Diese DLL in Ressourcenkette einf�gen
		// HINWEIS: Wird diese Erweiterungs-DLL implizit durch eine regul�re
		//  MFC-DLL (wie z.B. ein ActiveX-Steuerelement) anstelle einer
		//  MFC-Anwendung eingebunden, dann m�chten Sie m�glicherweise diese
		//  Zeile aus DllMain entfernen und eine eigene Funktion einf�gen,
		//  die von dieser Erweiterungs-DLL exportiert wird. Die regul�re DLL,
		//  die diese Erweiterungs-DLL verwendet, sollte dann explizit die 
		//  Initialisierungsfunktion der Erweiterungs-DLL aufrufen. Anderenfalls 
		//  wird das CDynLinkLibrary-Objekt nicht mit der Recourcenkette der 
		//  regul�ren DLL verbunden, was zu ernsthaften Problemen
		//  f�hren kann.

		new CDynLinkLibrary(OpenGLDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// When using OLE we find that the TRACE0 macro never returns.
		// So we omit it here in order to finish correctly in DEBUG mode.
		//
		//#ifdef _DEBUG
		//TRACE0("GENERAL32D.DLL Terminating!\n");
		//#endif //_DEBUG

		// Bibliothek vor dem Aufruf der Destruktoren schlie�en
		AfxTermExtensionModule(OpenGLDLL);
	}
	return 1;   // OK
}
