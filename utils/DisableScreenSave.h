// DisableScreenSave.h
// interface for the CDisableScreenSave class.
//////////////////////////////////////////////

#if !defined(AFX_DISABLESCREENSAVE_H__94B5D6F5
        _BB06_4409_9876_1D1EB6F1480B__INCLUDED_)
#define AFX_DISABLESCREENSAVE_H__94B5D6F5
        _BB06_4409_9876_1D1EB6F1480B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDisableScreenSave  
{
public:
    CDisableScreenSave();
    virtual ~CDisableScreenSave();

protected:
    int *m_pValue;
};

#endif  
        // !defined(AFX_DISABLESCREENSAVE_H__94B5D6F5
        //      _BB06_4409_9876_1D1EB6F1480B__INCLUDED_)

// DisableScreenSave.cpp 
// implementation of the CDisableScreenSave class.
//////////////////////////////////////////////////

#include "stdafx.h"
#include "DisableScreenSave.h"
#include <../src/afximpl.h>

///////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////

static UINT dss_GetList[] = {SPI_GETLOWPOWERTIMEOUT, 
    SPI_GETPOWEROFFTIMEOUT, SPI_GETSCREENSAVETIMEOUT};
static UINT dss_SetList[] = {SPI_SETLOWPOWERTIMEOUT, 
    SPI_SETPOWEROFFTIMEOUT, SPI_SETSCREENSAVETIMEOUT};


static const int dss_ListCount = _countof(dss_GetList);


CDisableScreenSave::CDisableScreenSave()
{
    m_pValue = new int[dss_ListCount];

    for (int x=0;x<dss_ListCount;x++)
    {
        // Get the current value
        VERIFY(SystemParametersInfo (dss_GetList[x], 0, 
            &m_pValue[x], 0));

        TRACE(_T("%d = %d\n"), dss_GetList[x], m_pValue[x]);

        // Turn off the parameter
        VERIFY(SystemParametersInfo (dss_SetList[x], 0, 
            NULL, 0));
    }
}


CDisableScreenSave::~CDisableScreenSave()
{
    for (int x=0;x<dss_ListCount;x++)
    {
        // Set the old value
        VERIFY(SystemParametersInfo (dss_SetList[x], 
            m_pValue[x], NULL, 0));
    }

    delete[] m_pValue;
}
