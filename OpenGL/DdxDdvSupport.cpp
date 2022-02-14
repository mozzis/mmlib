/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DdxDdvSupport.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "OpenGL.h"
#include "DdxDdvSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Helper functions

#ifdef _DEBUG
void AFXAPI AfxTrace(UINT nIDPrompt)
{
	CString string;
	if (!string.LoadString(nIDPrompt))
	{
		TRACE1("Error: failed to load trace prompt string 0x%04x.\n",
			nIDPrompt);
		ASSERT(FALSE);
	}
	TRACE(_T("AfxTrace: %s\n"), string);
}
#endif //_DEBUG

// only supports '%d', '%u', '%sd', '%su', '%ld' and '%lu'
static BOOL AFXAPI AfxSimpleScanf(LPCTSTR lpszText,
	LPCTSTR lpszFormat, va_list pData)
{
	ASSERT(lpszText != NULL);
	ASSERT(lpszFormat != NULL);

	ASSERT(*lpszFormat == '%');
	lpszFormat++;        // skip '%'

	BOOL bLong = FALSE;
	BOOL bShort = FALSE;
	if (*lpszFormat == 'l')
	{
		bLong = TRUE;
		lpszFormat++;
	}
	else if (*lpszFormat == 's')
	{
		bShort = TRUE;
		lpszFormat++;
	}

	ASSERT(*lpszFormat == 'd' || *lpszFormat == 'u');
	ASSERT(lpszFormat[1] == '\0');

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	TCHAR chFirst = lpszText[0];
	long l, l2;
	if (*lpszFormat == 'd')
	{
		// signed
		l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (int)l;
	}
	else
	{
		// unsigned
		if (*lpszText == '-')
			return FALSE;
		l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (unsigned int)l;
	}
	if (l == 0 && chFirst != '0')
		return FALSE;   // could not convert

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	if (bShort)
	{
		if ((short)l != l)
			return FALSE;   // too big for short
		*va_arg(pData, short*) = (short)l;
	}
	else
	{
		ASSERT(sizeof(long) == sizeof(int));
		ASSERT(l == l2);
		*va_arg(pData, long*) = l;
	}

	// all ok
	return TRUE;
}

static BOOL AFXAPI AfxSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

static void AFX_CDECL DDX_TextWithFormatNMB(CDataExchange* pDX, int nIDC,
	LPCTSTR lpszFormat, UINT nIDPrompt, ...)
	// only supports windows output formats - no floating point
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szT[32];
	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szT, _countof(szT));
		if (!AfxSimpleScanf(szT, lpszFormat, pData))
		{
			AfxTrace(nIDPrompt);
			pDX->Fail();        // throws exception
		}
	}
	else
	{
		wvsprintf(szT, lpszFormat, pData);
			// does not support floating point numbers - see dlgfloat.cpp
		AfxSetWindowText(hWndCtrl, szT);
	}

	va_end(pData);
}

static void AFXAPI AfxTextFloatFormatNMB(CDataExchange* pDX, int nIDC,
	void* pData, double value, int nSizeGcvt)
{
	ASSERT(pData != NULL);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[32];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		double d;
		if (!AfxSimpleFloatParse(szBuffer, d))
		{
			AfxTrace(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

static void AFXAPI AfxTextFloatFormatComma(CDataExchange* pDX, int nIDC,
	void* pData, double value, int nSizeGcvt)
{
	ASSERT(pData != NULL);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[32];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		// replace all occurances of ',' by '.'
		int size = _countof(szBuffer);
		for (int k = 0; k < size; k++)
		{
			if (szBuffer[k] == _T(','))
				szBuffer[k] = _T('.');
		}
		double d;
		if (!AfxSimpleFloatParse(szBuffer, d))
		{
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

static void AFXAPI AfxTextFloatFormatCommaNMB(CDataExchange* pDX, int nIDC,
	void* pData, double value, int nSizeGcvt)
{
	ASSERT(pData != NULL);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szBuffer[32];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		// replace all occurances of ',' by '.'
		int size = _countof(szBuffer);
		for (int k = 0; k < size; k++)
		{
			if (szBuffer[k] == _T(','))
				szBuffer[k] = _T('.');
		}
		double d;
		if (!AfxSimpleFloatParse(szBuffer, d))
		{
			AfxTrace(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// DDX/DDV Unterstützung

/////////////////////////////////////////////////////////////////////////////
// simple text operations (NoMessageBox)

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, BYTE& value)
{
	int n = (int)value;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, &n);
		if (n > 255)
		{
			AfxTrace(AFX_IDP_PARSE_BYTE);
			pDX->Fail();        // throws exception
		}
		value = (BYTE)n;
	}
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, n);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, short& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%sd"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%hd"), AFX_IDP_PARSE_INT, value);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, int& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, value);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, UINT& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, &value);
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, value);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, long& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, &value);
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, value);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, DWORD& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, &value);
	else
		DDX_TextWithFormatNMB(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, value);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		::GetWindowText(hWndCtrl, value.GetBufferSetLength(nLen), nLen+1);
		value.ReleaseBuffer();
	}
	else
	{
		AfxSetWindowText(hWndCtrl, value);
	}
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, float& value, int digits)
{
	AfxTextFloatFormatNMB(pDX, nIDC, &value, value, digits);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, double& value, int digits)
{
	AfxTextFloatFormatNMB(pDX, nIDC, &value, value, digits);
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleDateTime& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		CString strTemp;

		::GetWindowText(hWndCtrl, strTemp.GetBufferSetLength(nLen), nLen+1);
		strTemp.ReleaseBuffer();

		if (!value.ParseDateTime(strTemp))  // throws exception
		{
			// Can't convert string to datetime
			AfxTrace(AFX_IDP_PARSE_DATETIME);
			pDX->Fail();    // throws exception
		}
	}
	else
	{
		CString strTemp = value.Format();
		AfxSetWindowText(hWndCtrl, strTemp);
	}
}

void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleCurrency& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		CString strTemp;

		::GetWindowText(hWndCtrl, strTemp.GetBufferSetLength(nLen), nLen+1);
		strTemp.ReleaseBuffer();

		if (!value.ParseCurrency(strTemp))  // throws exception
		{
			// Can't convert string to currency
			AfxTrace(AFX_IDP_PARSE_CURRENCY);
			pDX->Fail();    // throws exception
		}
	}
	else
	{
		CString strTemp = value.Format();
		AfxSetWindowText(hWndCtrl, strTemp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// simple text operations (Comma)

void AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, float& value)
{
	AfxTextFloatFormatComma(pDX, nIDC, &value, value, FLT_DIG);
}

void AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, double& value)
{
	AfxTextFloatFormatComma(pDX, nIDC, &value, value, DBL_DIG);
}

/////////////////////////////////////////////////////////////////////////////
// simple text operations (Comma,NoMessageBox)

void AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, float& value)
{
	AfxTextFloatFormatCommaNMB(pDX, nIDC, &value, value, FLT_DIG);
}

void AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, double& value)
{
	AfxTextFloatFormatCommaNMB(pDX, nIDC, &value, value, DBL_DIG);
}

/////////////////////////////////////////////////////////////////////////////
// valid range including border

void AFXAPI DDV_MinByte(CDataExchange* pDX, BYTE value, BYTE minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxByte(CDataExchange* pDX, BYTE value, BYTE maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinShort(CDataExchange* pDX, short value, short minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxShort(CDataExchange* pDX, short value, short maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinInt(CDataExchange* pDX, int value, int minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxInt(CDataExchange* pDX, int value, int maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinLong(CDataExchange* pDX, long value, long minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxLong(CDataExchange* pDX, long value, long maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinUInt(CDataExchange* pDX, UINT value, UINT minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxUInt(CDataExchange* pDX, UINT value, UINT maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinDWord(CDataExchange* pDX, DWORD value, DWORD minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxDWord(CDataExchange* pDX, DWORD value, DWORD maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinFloat(CDataExchange* pDX, float const& value, float minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxFloat(CDataExchange* pDX, float const& value, float maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinDouble(CDataExchange* pDX, double const& value, double minVal)
{
	if (value < minVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_EQUAL, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxDouble(CDataExchange* pDX, double const& value, double maxVal)
{
	if (value > maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding border (NoBorder)

void AFXAPI DDV_MinByteNB(CDataExchange* pDX, BYTE value, BYTE minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxByteNB(CDataExchange* pDX, BYTE value, BYTE maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinShortNB(CDataExchange* pDX, short value, short minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxShortNB(CDataExchange* pDX, short value, short maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinIntNB(CDataExchange* pDX, int value, int minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxIntNB(CDataExchange* pDX, int value, int maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinLongNB(CDataExchange* pDX, long value, long minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxLongNB(CDataExchange* pDX, long value, long maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinUIntNB(CDataExchange* pDX, UINT value, UINT minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxUIntNB(CDataExchange* pDX, UINT value, UINT maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxDWordNB(CDataExchange* pDX, DWORD value, DWORD maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinFloatNB(CDataExchange* pDX, float const& value, float minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxFloatNB(CDataExchange* pDX, float const& value, float maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinDoubleNB(CDataExchange* pDX, double const& value, double minVal)
{
	if (value <= minVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER, minVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MaxDoubleNB(CDataExchange* pDX, double const& value, double maxVal)
{
	if (value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxByteNB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value <= minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding left border (NoLeftBorder)

void AFXAPI DDV_MinMaxByteNLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNLB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNLB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNLB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNLB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNLB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value <= minVal || value > maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_LESS_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding right border (NoRightBorder)

void AFXAPI DDV_MinMaxByteNRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNRB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNRB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNRB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNRB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNRB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value < minVal || value >= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_GREATER_EQUAL_LESS, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding single value (NotEqual)

void AFXAPI DDV_MinMaxByteNE(CDataExchange* pDX, BYTE value, BYTE val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNE(CDataExchange* pDX, short value, short val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNE(CDataExchange* pDX, int value, int val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNE(CDataExchange* pDX, long value, long val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNE(CDataExchange* pDX, UINT value, UINT val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNE(CDataExchange* pDX, DWORD value, DWORD val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_INTEGER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNE(CDataExchange* pDX, float const& value, float val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_NUMBER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNE(CDataExchange* pDX, double const& value, double val)
{
	if (value == val)
	{
		CString mString;
		mString.Format(IDS_NUMBER_NOT_EQUAL, val);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding border (NotIn)

void AFXAPI DDV_MinMaxByteNI(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNI(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNI(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNI(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNI(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNI(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNI(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNI(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value > minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding border (NotIn) (NoBorder)

void AFXAPI DDV_MinMaxByteNINB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNINB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNINB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNINB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNINB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNINB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNINB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNINB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value >= minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding left border (NotIn) (NoLeftBorder)

void AFXAPI DDV_MinMaxByteNINLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNINLB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNINLB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNINLB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNINLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNINLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNINLB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNINLB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value >= minVal && value < maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_GREATER_EQUAL, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

/////////////////////////////////////////////////////////////////////////////
// valid range excluding right border (NotIn) (NoRightBorder)

void AFXAPI DDV_MinMaxByteNINRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxShortNINRB(CDataExchange* pDX, short value, short minVal, short maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxIntNINRB(CDataExchange* pDX, int value, int minVal, int maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxLongNINRB(CDataExchange* pDX, long value, long minVal, long maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxUIntNINRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDWordNINRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_INTEGER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxFloatNINRB(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}

void AFXAPI DDV_MinMaxDoubleNINRB(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	if (value > minVal && value <= maxVal)
	{
		CString mString;
		mString.Format(IDS_NUMBER_LESS_EQUAL_GREATER, minVal, maxVal);
		AfxMessageBox(mString);
		pDX->Fail();
	}
}
