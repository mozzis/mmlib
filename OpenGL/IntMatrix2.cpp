/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// IntMatrix.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Matrix2.h"
#include "IntMatrix2.h"
#include "MathAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntMatrix

CIntMatrix::CIntMatrix(void* /*dummyPointer*/)
{
	m_pOutMatrix = NULL;
	m_pOutIntMatrix = NULL;

	// this is an empty matrix
	SetSize(0);					
}

CIntMatrix::CIntMatrix()
{
	PrepareOutMatrices();

	// this is an empty matrix
	SetSize(0);					
}

CIntMatrix::CIntMatrix(const int& inValue)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(1);
	SetAt(0, inValue);
}

CIntMatrix::CIntMatrix(const int& firstValue, const int& lastValue, const double& stepSize)
{
	PrepareOutMatrices();

	// copy inMatrix
	if (stepSize == 0.)
		SetSize(0);
	else if ((lastValue-firstValue)/stepSize < 0.)
		SetSize(0);
	else
	{
		SetSize(CSize(1, int(ceil((lastValue-firstValue+stepSize)/stepSize))));
		int size = GetTotalSize();
		for (int k = 0; k < size-1; k++)
			SetAt(k, int(firstValue + k*stepSize));
		SetAt(GetTotalSize()-1, lastValue);
	}
}

CIntMatrix::CIntMatrix(const CIntMatrix& inMatrix)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, inMatrix.GetAt(i));
}

CIntMatrix::CIntMatrix(const CMatrix& inMatrix)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, int(inMatrix.GetAt(i)));
}

CIntMatrix::CIntMatrix(const CSize& inSize, const int& initValue)
{
	PrepareOutMatrices();

	// initialize with initValue
	SetSize(inSize);
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, initValue);
}

CIntMatrix::~CIntMatrix()
{
	if (m_pOutMatrix)
		delete m_pOutMatrix;
	if (m_pOutIntMatrix)
		delete m_pOutIntMatrix;
}

//{{AFX_MSG_MAP(CIntMatrix)
	// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// Diagnose CIntMatrix

#ifdef _DEBUG
void CIntMatrix::AssertValid() const
{
	CArray<int, int>::AssertValid();
}

void CIntMatrix::Dump(CDumpContext& dc) const
{
	CArray<int, int>::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Überladungen CIntMatrix

void CIntMatrix::SetSize(int nNewSize, int nGrowBy)
{
	// format it as a column vector
	int xSize = nNewSize;
	int ySize = 1;
	if (xSize <= 0)
		xSize = ySize = 0;
	m_nXSize = xSize;
	m_nYSize = ySize;
	CArray<int, int>::SetSize(xSize*ySize, nGrowBy);
}

void CIntMatrix::SetSize(CSize nNewSize, int nGrowBy)
{
	int xSize = nNewSize.cx;
	int ySize = nNewSize.cy;
	if (xSize <= 0 || ySize <= 0)
		xSize = ySize = 0;
	m_nXSize = xSize;
	m_nYSize = ySize;
	CArray<int, int>::SetSize(xSize*ySize, nGrowBy);
}

CSize CIntMatrix::GetSize() const
{
	return CSize(m_nXSize, m_nYSize);
}

int CIntMatrix::GetTotalSize() const
{
	CSize mSize = GetSize();
	return mSize.cx * mSize.cy;
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CIntMatrix 
void CIntMatrix::PrepareOutMatrices()
{
	m_pOutMatrix = new CMatrix(this);
	m_pOutIntMatrix = new CIntMatrix(this);
	ASSERT(m_pOutMatrix);
	ASSERT(m_pOutIntMatrix);
}

void CIntMatrix::Empty()
{
	SetSize(0);
}

BOOL CIntMatrix::IsEmpty()
{
	return(GetTotalSize() == 0);
}

BOOL CIntMatrix::IsVector()
{
	return(GetSize().cx == 1 || GetSize().cy == 1);
}

void CIntMatrix::DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo)
{
	CString mMsg;
	switch (i)
	{
	case 1:
		mMsg.Format("Die Matrizen (%dx%d und %dx%d) müssen die gleiche Dimension haben.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 2:
		mMsg.Format("Die Matrizen der Größe %dx%d und %dx%d können nicht miteinander multpliziert werden.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 3:
		mMsg.Format("Die Matrizen der Größe %dx%d und %dx%d können nicht horizontal verkettet werden.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 4:
		mMsg.Format("Die Matrizen der Größe %dx%d und %dx%d können nicht vertikal verkettet werden.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 5:
		mMsg.Format("Das Indexpaar (%d,%d) liegt nicht im Gültigkeitsbereich der %dx%d Matrix.\n", sizeTwo.cx, sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 6:
		mMsg.Format("Der Zeilenindex %d liegt nicht im Gültigkeitsbereich der %dx%d Matrix.\n", sizeTwo.cx, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 7:
		mMsg.Format("Der Spaltenindex %d liegt nicht im Gültigkeitsbereich der %dx%d Matrix.\n", sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 8:
		mMsg.Format("Die %dx%d Matrix kann nicht auf die Größe %dx%d umgeformt werden.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	default:
		{}
	}
}

int CIntMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY) const
{
	if (nSubscriptX >= 1 && nSubscriptX <= GetSize().cx && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
		return (*this)[(nSubscriptY-1)*GetSize().cx+nSubscriptX-1];
	else
	{
		((class CIntMatrix *)this)->DimensionErrorMessage(5, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return 0;
	}
}

int& CIntMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY)
{
	static int ret;

	if (nSubscriptX >= 1 && nSubscriptX <= GetSize().cx && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
		return (*this)[(nSubscriptY-1)*GetSize().cx+nSubscriptX-1];
	else
	{
		DimensionErrorMessage(5, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return ret = 0;
	}
}

CIntMatrix CIntMatrix::operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (nSubscriptMatrixX.GetSize() == nSubscriptMatrixY.GetSize())
	{
		outMatrix.SetSize(nSubscriptMatrixX.GetSize());
		int size = outMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix[i] = (*this)(nSubscriptMatrixX[i], nSubscriptMatrixY[i]);
	}
	else if (nSubscriptMatrixX.GetTotalSize() > 0 && nSubscriptMatrixY.GetTotalSize() == 1)
	{
		outMatrix.SetSize(nSubscriptMatrixX.GetSize());
		int size = outMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix[i] = (*this)(nSubscriptMatrixX[i], nSubscriptMatrixY[0]);
	}
	else if (nSubscriptMatrixX.GetTotalSize() == 1 && nSubscriptMatrixY.GetTotalSize() > 0)
	{
		outMatrix.SetSize(nSubscriptMatrixY.GetSize());
		int size = outMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix[i] = (*this)(nSubscriptMatrixX[0], nSubscriptMatrixY[i]);
	}
	else
	{
		((class CMatrix *)this)->DimensionErrorMessage(1, nSubscriptMatrixX.GetSize(), nSubscriptMatrixY.GetSize());
		outMatrix.Empty();
	}
	return outMatrix;
}

CIntMatrix& CIntMatrix::operator=(const CIntMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, inMatrix.GetAt(i));

	return *this;
}

CIntMatrix& CIntMatrix::operator=(const CMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, int(inMatrix.GetAt(i)));

	return *this;
}

void CIntMatrix::operator+=(const int& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) + inValue);
}

void CIntMatrix::operator-=(const int& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) - inValue);
}

void CIntMatrix::operator*=(const int& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) * inValue);
}

void CIntMatrix::operator/=(const int& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) / inValue);
}

void CIntMatrix::operator+=(const CIntMatrix& inMatrix)
{	
	if (GetSize() == inMatrix.GetSize())
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) + inMatrix.GetAt(i));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) + inMatrix.GetAt(0));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, tmp + inMatrix.GetAt(i));
	}
	else
	{
		DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());
		SetSize(0);
	}
}

void CIntMatrix::operator-=(const CIntMatrix& inMatrix)
{	
	if (GetSize() == inMatrix.GetSize())
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) - inMatrix.GetAt(i));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) - inMatrix.GetAt(0));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, tmp - inMatrix.GetAt(i));
	}
	else
	{
		DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());
		SetSize(0);
	}
}

void CIntMatrix::DotTimesAssign(const CIntMatrix& inMatrix)
{	
	if (GetSize() == inMatrix.GetSize())
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) * inMatrix.GetAt(i));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) * inMatrix.GetAt(0));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, tmp * inMatrix.GetAt(i));
	}
	else
	{
		DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());
		SetSize(0);
	}
}

void CIntMatrix::DotOverAssign(const CIntMatrix& inMatrix)
{	
	if (GetSize() == inMatrix.GetSize())
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) / inMatrix.GetAt(i));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, GetAt(i) / inMatrix.GetAt(0));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			SetAt(i, tmp / inMatrix.GetAt(i));
	}
	else
	{
		DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());
		SetSize(0);
	}
}

CIntMatrix CIntMatrix::operator+(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix -= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator/(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CIntMatrix operator+(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix = inMatrix;
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix operator-(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix = inMatrix;
	outMatrix -= inValue;

	return -outMatrix;
}

CIntMatrix operator*(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix = inMatrix;
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix operator/(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix = CMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix CIntMatrix::operator+(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator*(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator/(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CMatrix operator+(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix operator-(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix -= inValue;

	return -outMatrix;
}

CMatrix operator*(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix operator/(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = CMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator+(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::DotTimes(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::DotOver(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CIntMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) < inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) < inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) < inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const CIntMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) > inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) > inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) > inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const CIntMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) <= inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) <= inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) <= inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const CIntMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) >= inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) >= inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) >= inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CIntMatrix::operator+(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return -outMatrix;
}

CMatrix CIntMatrix::DotTimes(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CIntMatrix::DotOver(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) < inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) < inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) < inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const CMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) > inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) > inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) > inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const CMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) <= inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) <= inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) <= inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const CMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) >= inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) >= inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) >= inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-() const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, -GetAt(i));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const CIntMatrix& inMatrix) const
{	
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	int tmp;
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		outMatrix.SetSize(CSize(GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0;
				for (int k = 1; k <= size; k++)
					tmp += (*this)(i,k)*inMatrix(k,j);
				outMatrix(i,j) = tmp;
			}
	}
	else if ((GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1) || (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0))
		outMatrix = (*this).DotTimes(inMatrix);
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(2, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CIntMatrix::operator*(const CMatrix& inMatrix) const
{	
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	double tmp;
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		outMatrix.SetSize(CSize(GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0.;
				for (int k = 1; k <= size; k++)
					tmp += (*this)(i,k)*inMatrix(k,j);
				outMatrix(i,j) = tmp;
			}
	}
	else if ((GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1) || (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0))
		outMatrix = (*this).DotTimes(inMatrix);
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(2, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

void CIntMatrix::reshape(const int& nNewSizeX, const int& nNewSizeY)
{
	if (GetTotalSize() == nNewSizeX*nNewSizeY)
		SetSize(CSize(nNewSizeX, nNewSizeY));
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(8, GetSize(), CSize(nNewSizeX, nNewSizeY));
}

void CIntMatrix::reshape(const CSize& nNewSize)
{
	if (GetTotalSize() == nNewSize.cx*nNewSize.cy)
		SetSize(nNewSize);
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(8, GetSize(), nNewSize);
}

void CIntMatrix::reshape()
{
	// make it a column vector
	SetSize(GetTotalSize());
}

CIntMatrix CIntMatrix::GetRows(const int& nSubscriptX) const
{
	CIntMatrix& outMatrix = *m_pOutIntMatrix;	
	if (nSubscriptX >=1 && nSubscriptX <= GetSize().cx)
	{
		outMatrix.SetSize(CSize(1, GetSize().cy));
		int sizeY = GetSize().cy;
		for (int j = 1; j <= sizeY; j++)
			outMatrix(1,j) = (*this)(nSubscriptX,j);
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(6, GetSize(), CSize(nSubscriptX, 1));

	return outMatrix;
}

CIntMatrix CIntMatrix::GetColumns(const int& nSubscriptY) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (nSubscriptY >=1 && nSubscriptY <= GetSize().cy)
	{
		outMatrix.SetSize(CSize(GetSize().cx, 1));
		int sizeX = GetSize().cx;
		for (int i = 1; i <= sizeX; i++)
			outMatrix(i,1) = (*this)(i,nSubscriptY);
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(7, GetSize(), CSize(1, nSubscriptY));

	return outMatrix;
}

CIntMatrix CIntMatrix::GetRows(const CIntMatrix& nSubscriptMatrixX) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (nSubscriptMatrixX.GetTotalSize() > 0)
		outMatrix = GetRows(nSubscriptMatrixX[0]);
	int size = nSubscriptMatrixX.GetTotalSize();
	for (int i = 1; i < size; i++)
		outMatrix = concatVert(outMatrix, GetRows(nSubscriptMatrixX[i]));

	return outMatrix;
}

CIntMatrix CIntMatrix::GetColumns(const CIntMatrix& nSubscriptMatrixY) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	if (nSubscriptMatrixY.GetTotalSize() > 0)
		outMatrix = GetColumns(nSubscriptMatrixY[0]);
	int size = nSubscriptMatrixY.GetTotalSize();
	for (int i = 1; i < size; i++)
		outMatrix = concatHor(outMatrix, GetColumns(nSubscriptMatrixY[i]));

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Mathematische CIntMatrix Funktionen
CIntMatrix abs(const CIntMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, abs(inMatrix.GetAt(i)));

	return outMatrix;
}

int matMax(const CIntMatrix& inMatrix)		
{
	int outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int i = 1; i < size; i++)
			outValue = max(outValue, inMatrix.GetAt(i));
	}
	else
		outValue = 0;	

	return outValue;
}

int matMin(const CIntMatrix& inMatrix)		
{
	int outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int i = 1; i < size; i++)
			outValue = min(outValue, inMatrix.GetAt(i));
	}
	else
		outValue = 0;	

	return outValue;
}

CIntMatrix matMax(const CIntMatrix& inMatrix, const int& inValue)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMax(const int& inValue, const CIntMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMax(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix1.m_pOutIntMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, max(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, max(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, max(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CIntMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix matMin(const CIntMatrix& inMatrix, const int& inValue)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMin(const int& inValue, const CIntMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMin(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix1.m_pOutIntMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, min(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, min(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, min(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CIntMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Andere CIntMatrix Funktionen
CIntMatrix transp(const CIntMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(CSize(inMatrix.GetSize().cy, inMatrix.GetSize().cx));
	int sizeX = inMatrix.GetSize().cx;
	int sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			outMatrix(j,i) = inMatrix(i,j);

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	if (inMatrix.GetTotalSize() == nNewSizeX*nNewSizeY)
	{
		outMatrix = inMatrix;
		outMatrix.SetSize(CSize(nNewSizeX, nNewSizeY));
	}
	else
		((class CIntMatrix *)&inMatrix)->DimensionErrorMessage(8, inMatrix.GetSize(), CSize(nNewSizeX, nNewSizeY));

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix, const CSize& nNewSize)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	if (inMatrix.GetTotalSize() == nNewSize.cx*nNewSize.cy)
	{
		outMatrix = inMatrix;
		outMatrix.SetSize(nNewSize);
	}
	else
		((class CIntMatrix *)&inMatrix)->DimensionErrorMessage(8, inMatrix.GetSize(), nNewSize);

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix = inMatrix;
	// make it a column vector
	outMatrix.SetSize(inMatrix.GetTotalSize());

	return outMatrix;
}

CIntMatrix concatHor(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix1.m_pOutIntMatrix);
	int i, j, sizeX, sizeY;

	if (inMatrix1.GetSize().cx == inMatrix2.GetSize().cx)
	{
		outMatrix.SetSize(CSize(inMatrix1.GetSize().cx, inMatrix1.GetSize().cy + inMatrix2.GetSize().cy));
		sizeX = inMatrix1.GetSize().cx;
		sizeY = inMatrix1.GetSize().cy;
		for (i = 1; i <= sizeX; i++)
			for (j = 1; j <= sizeY; j++)
				outMatrix(i,j) = inMatrix1(i,j);
		sizeX = inMatrix2.GetSize().cx;
		sizeY = inMatrix2.GetSize().cy;
		for (i = 1; i <= sizeX; i++)
			for (j = 1; j <= sizeY; j++)
				outMatrix(i,inMatrix1.GetSize().cy+j) = inMatrix2(i,j);
	}
	else if (inMatrix1.GetTotalSize() == 0)
		outMatrix = inMatrix2;
	else if (inMatrix2.GetTotalSize() == 0)
		outMatrix = inMatrix1;
	else
		((class CIntMatrix *)&inMatrix1)->DimensionErrorMessage(3, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix concatVert(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix1.m_pOutIntMatrix);
	int i, j, sizeX, sizeY;

	if (inMatrix1.GetSize().cy == inMatrix2.GetSize().cy)
	{
		outMatrix.SetSize(CSize(inMatrix1.GetSize().cx + inMatrix2.GetSize().cx, inMatrix1.GetSize().cy));
		sizeX = inMatrix1.GetSize().cx;
		sizeY = inMatrix1.GetSize().cy;
		for (i = 1; i <= sizeX; i++)
			for (j = 1; j <= sizeY; j++)
				outMatrix(i,j) = inMatrix1(i,j);
		sizeX = inMatrix2.GetSize().cx;
		sizeY = inMatrix2.GetSize().cy;
		for (i = 1; i <= sizeX; i++)
			for (j = 1; j <= sizeY; j++)
				outMatrix(inMatrix1.GetSize().cx+i,j) = inMatrix2(i,j);
	}
	else if (inMatrix1.GetTotalSize() == 0)
		outMatrix = inMatrix2;
	else if (inMatrix2.GetTotalSize() == 0)
		outMatrix = inMatrix1;
	else
		((class CIntMatrix *)&inMatrix1)->DimensionErrorMessage(4, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix rowSum(const CIntMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	int tmp;
	outMatrix.SetSize(CSize(inMatrix.GetSize().cx, 1));
	int sizeX = inMatrix.GetSize().cx;
	int sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 0;
		for (int j = 1; j <= sizeY; j++)
			tmp += inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CIntMatrix colSum(const CIntMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	int tmp;
	outMatrix.SetSize(CSize(1, inMatrix.GetSize().cy));
	int sizeY = inMatrix.GetSize().cy;
	int sizeX = inMatrix.GetSize().cx;
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 0;
		for (int i = 1; i <= sizeX; i++)
			tmp += inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

int sum(const CIntMatrix& inMatrix)		
{
	int tmp = 0;
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		tmp += inMatrix[i];

	return tmp;
}
