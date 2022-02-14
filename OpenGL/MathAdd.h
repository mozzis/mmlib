/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// MathAdd.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Mathematische Funktionen
double AFX_EXT_API epsilon();
double AFX_EXT_API round(const double& inValue);
double AFX_EXT_API sign(const double& inValue);
double AFX_EXT_API fix(const double& inValue);
double AFX_EXT_API frem(const double& inValue1, const double& inValue2);
double AFX_EXT_API wkreset(const double& inValue1, const double& inValue2);
void AFX_EXT_API rounderg(double& inValue1, double& inValue2);
void AFX_EXT_API crossprod(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z, double& c_x, double& c_y, double& c_z);
double AFX_EXT_API scalprod3(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
