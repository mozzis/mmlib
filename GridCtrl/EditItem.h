// Viewdata.h contains common data and constants for views

#pragma once

static const CString g_StrPads = _T("                                       ");
static const int g_lenPads = g_StrPads.GetLength();

// Ad hoc data type identifiers
/////////////////////////////////////////////////////////////////////////////
typedef enum eDataType 
{
  eInvalid = 0,
  eInt,
  eHexInt,
  eDouble,
  eString,    // zero-terminated string
  eDate,      // absolute time/date
  eTimeSpan,  // relative time elapsed
  eStruct,    // triggers recursive descent
  eStructEnd, // ends recursive descent
  eEnum,
  eBool,
} eDataType;

// A struct whose address will be stored in the ITEMDATA of editable fields
// This will allow runtime editing of the data in the grid
/////////////////////////////////////////////////////////////////////////////
class EDITITEM
{
public:
//  LPVOID pData;
  double dData;        // also stores time values
  long iData;          // also stores hex and enum types
  TCHAR sData[80];     // string data - static for now
  LPCTSTR *pStrings;   // strings for enums
  eDataType eType;     // which field(s) is valid
  int nFieldWidth;     // width of field in grid
};
