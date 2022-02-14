/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// MathAdd.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "MathAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Mathematische Funktionen
double epsilon()
{
	double ret = 1.;
	while (ret+1. != 1.)
		ret = ret/2.;
	return 2.*ret;
}

double round(const double& inValue)
{
	return ceil(inValue - 0.5);
}

double sign(const double& inValue)
{
	if (inValue > 0.)
		return 1.;
	else if (inValue < 0.)
		return -1.;
	else 
		return 0;
}

double fix(const double& inValue)
{
	return double(int(inValue));
}

double frem(const double& inValue1, const double& inValue2)
{
	return inValue1 - inValue2 * fix(inValue1/inValue2);
}

double wkreset(const double& inValue1, const double& inValue2)
{
	//wkreset   Setzt "inValue1" modulo "inValue2" in das Intervall 
	//          [-inValue2/2.,inValue2/2.] zurueck.
	//          z.B. ist  cos(inValue1) == cos(wk_reset(inValue1, 2.*pi)) .

	if (inValue1 == 0.)
		return inValue1;
	double retValue = fabs(inValue1) + inValue2/2.;
	retValue = retValue - ceil(retValue/inValue2)*inValue2;
	return sign(inValue1)*(retValue + inValue2/2.);
}

void rounderg(double& inValue1, double& inValue2)
{
	// inValue1 und inValue2 so "runden", dass hoechstens zwei 
	// Kommastellen ab der Differenzgenauigkeit (max-min) bleiben.
	// "100." bezieht sich auf 10^2=100, d.h. 2 Kommastellen 
	double delta = min(1., fabs(inValue1-inValue2));
	delta = fabs(delta) + 1. - sign(delta);
	double fac = pow(10., floor(log10(delta)))/100.;

	inValue1 = fac*round(inValue1/fac);
	inValue2 = fac*round(inValue2/fac);
}

void crossprod(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z, double& c_x, double& c_y, double& c_z)
{
	c_x = a_y * b_z - a_z * b_y;
	c_y = a_z * b_x - a_x * b_z;
	c_z = a_x * b_y - a_y * b_x;
}

double scalprod3(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z)
{
	return a_x * b_x + a_y * b_y + a_z * b_z;
}