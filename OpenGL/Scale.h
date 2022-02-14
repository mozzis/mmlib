/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: June 2000
//
/////////////////////////////////////////////////////////////////////////////
// Scale.h: Schnittstelle der Klasse CScale
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALE_H__3CBDA4D4_46B4_11D4_8BBF_00001CD5E4D1__INCLUDED_)
#define AFX_SCALE_H__3CBDA4D4_46B4_11D4_8BBF_00001CD5E4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

class AFX_EXT_CLASS CScale : public CObject  
{
	DECLARE_DYNAMIC(CScale)

public:
	CScale(double dXMax = HUGE, double dXMin = -HUGE, double dYMax = HUGE, double dYMin = -HUGE, double dZMax = HUGE, double dZMin = -HUGE);
	CScale(const CScale& inScale);
	virtual ~CScale();

	void Reset();
	CScale& operator=(const CScale& inScale);

public:
	double m_dXMax;
	double m_dXMin;

	double m_dYMax;
	double m_dYMin;

	double m_dZMax;
	double m_dZMin;

	CString m_strXLabel;
	CString m_strYLabel;
	CString m_strZLabel;
};

#endif // !defined(AFX_SCALE_H__3CBDA4D4_46B4_11D4_8BBF_00001CD5E4D1__INCLUDED_)
