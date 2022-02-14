/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DdxDdvSupport.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Helper functions

#ifdef _DEBUG

void AFX_EXT_API AFXAPI AfxTrace(UINT nIDPrompt);

#else   // _DEBUG

inline void AFX_EXT_API AFXAPI AfxTrace(UINT) { }

#endif // !_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DDX/DDV support

// simple text operations (NoMessageBox)
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, BYTE& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, short& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, int& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, UINT& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, long& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, DWORD& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, CString& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, float& value, int digits = FLT_DIG);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, double& value, int digits = DBL_DIG);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleCurrency& value);
void AFX_EXT_API AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleDateTime& value);

// simple text operations (Comma)
void AFX_EXT_API AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, float& value);
void AFX_EXT_API AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, double& value);

// simple text operations (Comma,NoMessageBox)
void AFX_EXT_API AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, float& value);
void AFX_EXT_API AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, double& value);

// valid range including border
void AFX_EXT_API AFXAPI DDV_MinByte(CDataExchange* pDX, BYTE value, BYTE minVal);
void AFX_EXT_API AFXAPI DDV_MaxByte(CDataExchange* pDX, BYTE value, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinShort(CDataExchange* pDX, short value, short minVal);
void AFX_EXT_API AFXAPI DDV_MaxShort(CDataExchange* pDX, short value, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinInt(CDataExchange* pDX, int value, int minVal);
void AFX_EXT_API AFXAPI DDV_MaxInt(CDataExchange* pDX, int value, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinLong(CDataExchange* pDX, long value, long minVal);
void AFX_EXT_API AFXAPI DDV_MaxLong(CDataExchange* pDX, long value, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinUInt(CDataExchange* pDX, UINT value, UINT minVal);
void AFX_EXT_API AFXAPI DDV_MaxUInt(CDataExchange* pDX, UINT value, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinDWord(CDataExchange* pDX, DWORD value, DWORD minVal);
void AFX_EXT_API AFXAPI DDV_MaxDWord(CDataExchange* pDX, DWORD value, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinFloat(CDataExchange* pDX, float const& value, float minVal);
void AFX_EXT_API AFXAPI DDV_MaxFloat(CDataExchange* pDX, float const& value, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinDouble(CDataExchange* pDX, double const& value, double minVal);
void AFX_EXT_API AFXAPI DDV_MaxDouble(CDataExchange* pDX, double const& value, double maxVal);

// valid range excluding border (NoBorder)
void AFX_EXT_API AFXAPI DDV_MinByteNB(CDataExchange* pDX, BYTE value, BYTE minVal);
void AFX_EXT_API AFXAPI DDV_MaxByteNB(CDataExchange* pDX, BYTE value, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinShortNB(CDataExchange* pDX, short value, short minVal);
void AFX_EXT_API AFXAPI DDV_MaxShortNB(CDataExchange* pDX, short value, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinIntNB(CDataExchange* pDX, int value, int minVal);
void AFX_EXT_API AFXAPI DDV_MaxIntNB(CDataExchange* pDX, int value, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinLongNB(CDataExchange* pDX, long value, long minVal);
void AFX_EXT_API AFXAPI DDV_MaxLongNB(CDataExchange* pDX, long value, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinUIntNB(CDataExchange* pDX, UINT value, UINT minVal);
void AFX_EXT_API AFXAPI DDV_MaxUIntNB(CDataExchange* pDX, UINT value, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal);
void AFX_EXT_API AFXAPI DDV_MaxDWordNB(CDataExchange* pDX, DWORD value, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinFloatNB(CDataExchange* pDX, float const& value, float minVal);
void AFX_EXT_API AFXAPI DDV_MaxFloatNB(CDataExchange* pDX, float const& value, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinDoubleNB(CDataExchange* pDX, double const& value, double minVal);
void AFX_EXT_API AFXAPI DDV_MaxDoubleNB(CDataExchange* pDX, double const& value, double maxVal);

void AFX_EXT_API AFXAPI DDV_MinMaxByteNB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NoLeftBorder)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNLB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNLB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNLB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNLB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNLB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NoRightBorder)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNRB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNRB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNRB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNRB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNRB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding single value (NotEqual)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNE(CDataExchange* pDX, BYTE value, BYTE val);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNE(CDataExchange* pDX, short value, short val);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNE(CDataExchange* pDX, int value, int val);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNE(CDataExchange* pDX, long value, long val);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNE(CDataExchange* pDX, UINT value, UINT val);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNE(CDataExchange* pDX, DWORD value, DWORD val);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNE(CDataExchange* pDX, float const& value, float val);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNE(CDataExchange* pDX, double const& value, double val);

// valid range excluding border (NotIn)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNI(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNI(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNI(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNI(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNI(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNI(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNI(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNI(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoBorder)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNINB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNINB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNINB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNINB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNINB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNINB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNINB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNINB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoLeftBorder)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNINLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNINLB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNINLB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNINLB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNINLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNINLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNINLB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNINLB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoRightBorder)
void AFX_EXT_API AFXAPI DDV_MinMaxByteNINRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxShortNINRB(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxIntNINRB(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxLongNINRB(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxUIntNINRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDWordNINRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxFloatNINRB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFX_EXT_API AFXAPI DDV_MinMaxDoubleNINRB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
