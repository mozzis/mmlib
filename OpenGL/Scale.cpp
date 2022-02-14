/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: June 2000
//
/////////////////////////////////////////////////////////////////////////////
// Scale.cpp: Implementierung der Klasse CScale
//

#include "StdAfx.h"
#include "Scale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CScale		

IMPLEMENT_DYNAMIC(CScale, CObject)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CScale::CScale(double dXMax, double dXMin, double dYMax, double dYMin, double dZMax, double dZMin)
{
	m_dXMax = dXMax;
	m_dXMin = dXMin;
	m_dYMax = dYMax;
	m_dYMin = dYMin;
	m_dZMax = dZMax;
	m_dZMin = dZMin;
}

CScale::CScale(const CScale& inScale)
{
	m_dXMax = inScale.m_dXMax;
	m_dXMin = inScale.m_dXMin;
	m_dYMax = inScale.m_dYMax;
	m_dYMin = inScale.m_dYMin;
	m_dZMax = inScale.m_dZMax;
	m_dZMin = inScale.m_dZMin;

	m_strXLabel = inScale.m_strXLabel;
	m_strYLabel = inScale.m_strYLabel;
	m_strZLabel = inScale.m_strZLabel;
}

CScale::~CScale()
{
}

void CScale::Reset()
{
	m_dXMax = m_dYMax = m_dZMax = HUGE;
	m_dXMin = m_dYMin = m_dZMin = -HUGE;

	m_strXLabel.Empty();
	m_strYLabel.Empty();
	m_strZLabel.Empty();
}

CScale& CScale::operator=(const CScale& inScale)
{
	m_dXMax = inScale.m_dXMax;
	m_dXMin = inScale.m_dXMin;
	m_dYMax = inScale.m_dYMax;
	m_dYMin = inScale.m_dYMin;
	m_dZMax = inScale.m_dZMax;
	m_dZMin = inScale.m_dZMin;

	m_strXLabel = inScale.m_strXLabel;
	m_strYLabel = inScale.m_strYLabel;
	m_strZLabel = inScale.m_strZLabel;

	return *this;
}
