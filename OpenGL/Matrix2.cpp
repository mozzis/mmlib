/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// Matrix.cpp: Implementierungsdatei
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
// CMatrix

CMatrix::CMatrix(void* /*dummyPointer*/)
{
	m_pOutMatrix = NULL;
	m_pOutIntMatrix = NULL;

	// this is an empty matrix
	SetSize(0);					
}

CMatrix::CMatrix()
{
	PrepareOutMatrices();

	// this is an empty matrix
	SetSize(0);					
}

CMatrix::CMatrix(const double& inValue)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(1);
	SetAt(0, inValue);
}

CMatrix::CMatrix(const double& firstValue, const double& lastValue, const double& stepSize)
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
			SetAt(k, firstValue + k*stepSize);
		SetAt(GetTotalSize()-1, lastValue);
	}
}

CMatrix::CMatrix(const CMatrix& inMatrix)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, inMatrix.GetAt(i));
}

CMatrix::CMatrix(const CIntMatrix& inMatrix)
{
	PrepareOutMatrices();

	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, double(inMatrix.GetAt(i)));
}

CMatrix::CMatrix(const CSize& inSize, const double& initValue)
{
	PrepareOutMatrices();

	// initialize with initValue
	SetSize(inSize);
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, initValue);
}

CMatrix::~CMatrix()
{
	if (m_pOutMatrix)
		delete m_pOutMatrix;
	if (m_pOutIntMatrix)
		delete m_pOutIntMatrix;
}

//{{AFX_MSG_MAP(CMatrix)
	// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// Diagnose CMatrix

#ifdef _DEBUG
void CMatrix::AssertValid() const
{
	CArray<double, double>::AssertValid();
}

void CMatrix::Dump(CDumpContext& dc) const
{
	CArray<double, double>::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Überladungen CMatrix

void CMatrix::SetSize(int nNewSize, int nGrowBy)
{
	// format it as a column vector
	int xSize = nNewSize;
	int ySize = 1;
	if (xSize <= 0)
		xSize = ySize = 0;
	m_nXSize = xSize;
	m_nYSize = ySize;
	CArray<double, double>::SetSize(xSize*ySize, nGrowBy);
}

void CMatrix::SetSize(CSize nNewSize, int nGrowBy)
{
	int xSize = nNewSize.cx;
	int ySize = nNewSize.cy;
	if (xSize <= 0 || ySize <= 0)
		xSize = ySize = 0;
	m_nXSize = xSize;
	m_nYSize = ySize;
	CArray<double, double>::SetSize(xSize*ySize, nGrowBy);
}

CSize CMatrix::GetSize() const
{
	return CSize(m_nXSize, m_nYSize);
}

int CMatrix::GetTotalSize() const
{
	CSize mSize = GetSize();
	return mSize.cx * mSize.cy;
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CMatrix 
void CMatrix::PrepareOutMatrices()
{
	m_pOutMatrix = new CMatrix(this);
	m_pOutIntMatrix = new CIntMatrix(this);
	ASSERT(m_pOutMatrix);
	ASSERT(m_pOutIntMatrix);
}

void CMatrix::Empty()
{
	SetSize(0);
}

BOOL CMatrix::IsEmpty()
{
	return(GetTotalSize() == 0);
}

BOOL CMatrix::IsVector()
{
	return(GetSize().cx == 1 || GetSize().cy == 1);
}

void CMatrix::DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo)
{
	CString mMsg;
	switch (i)
	{
	case 1:
		mMsg.Format(IDS_ERR_DIMMISMATCH, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 2:
		mMsg.Format(IDS_ERR_DIMMULT, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 3:
		mMsg.Format(IDS_ERR_DIMHCAT, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 4:
		mMsg.Format(IDS_ERR_DIMVCAT, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 5:
		mMsg.Format(IDS_ERR_2INDEXBAD, sizeTwo.cx, sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 6:
		mMsg.Format(IDS_ERR_ROWINDEX, sizeTwo.cx, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 7:
		mMsg.Format(IDS_ERR_COLINDEX, sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case 8:
		mMsg.Format(IDS_ERR_BADMATSIZE, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	default:
		{}
	}
}

void CMatrix::DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo, CSize sizeThree)
{
	CString mMsg;
	switch (i)
	{
	case 1:
		mMsg.Format("Die Matrizen (%dx%d, %dx%d und %dx%d) müssen die gleiche Dimension haben.\n", sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy, sizeThree.cx, sizeThree.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	default:
		{}
	}
}

double CMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY) const
{
	if (nSubscriptX >= 1 && nSubscriptX <= GetSize().cx && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
		return (*this)[(nSubscriptY-1)*GetSize().cx+nSubscriptX-1];
	else
	{
		((class CMatrix *)this)->DimensionErrorMessage(5, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return 0.;
	}
}

double& CMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY)
{
	static double ret;

	if (nSubscriptX >= 1 && nSubscriptX <= GetSize().cx && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
		return (*this)[(nSubscriptY-1)*GetSize().cx+nSubscriptX-1];
	else
	{
		DimensionErrorMessage(5, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return ret = 0.;
	}
}

CMatrix CMatrix::operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
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

CMatrix& CMatrix::operator=(const CMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, inMatrix.GetAt(i));

	return *this;
}

CMatrix& CMatrix::operator=(const CIntMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, double(inMatrix.GetAt(i)));

	return *this;
}

void CMatrix::operator+=(const double& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) + inValue);
}

void CMatrix::operator-=(const double& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) - inValue);
}

void CMatrix::operator*=(const double& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) * inValue);
}

void CMatrix::operator/=(const double& inValue)
{
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, GetAt(i) / inValue);
}

void CMatrix::operator+=(const CMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::operator-=(const CMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::DotTimesAssign(const CMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::DotOverAssign(const CMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::operator+=(const CIntMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::operator-=(const CIntMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::DotTimesAssign(const CIntMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

void CMatrix::DotOverAssign(const CIntMatrix& inMatrix)
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
		double tmp = GetAt(0);
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

CMatrix CMatrix::operator+(const int& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator-(const int& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator*(const int& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix *= double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator/(const int& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix /= double(inValue);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const int& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CMatrix operator+(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix += double(inValue);

	return outMatrix;
}

CMatrix operator-(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix -= double(inValue);

	return -outMatrix;
}

CMatrix operator*(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix *= double(inValue);

	return outMatrix;
}

CMatrix operator/(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = CMatrix(inMatrix.GetSize(), double(inValue));
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix operator<(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix CMatrix::operator+(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix CMatrix::operator-(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= inValue;

	return outMatrix;
}

CMatrix CMatrix::operator*(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix CMatrix::operator/(const double& inValue) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const double& inValue) const
{
	ASSERT(m_pOutIntMatrix);
	CIntMatrix& outMatrix = *m_pOutIntMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CMatrix operator+(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix operator-(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix -= inValue;

	return -outMatrix;
}

CMatrix operator*(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix operator/(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = CMatrix(inMatrix.GetSize(), inValue);
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix operator<(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	ASSERT(inMatrix.m_pOutIntMatrix);
	CIntMatrix& outMatrix = *(inMatrix.m_pOutIntMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix CMatrix::operator+(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CMatrix::operator-(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return outMatrix;
}

CMatrix CMatrix::DotTimes(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CMatrix::DotOver(const CMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const CMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const CMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const CMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const CMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator+(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CMatrix::operator-(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return outMatrix;
}

CMatrix CMatrix::DotTimes(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CMatrix::DotOver(const CIntMatrix& inMatrix) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const CIntMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const CIntMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const CIntMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const CIntMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator-() const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, -GetAt(i));

	return outMatrix;
}

CMatrix CMatrix::operator*(const CMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(2, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator*(const CIntMatrix& inMatrix) const
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
		((class CMatrix *)this)->DimensionErrorMessage(2, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

void CMatrix::reshape(const int& nNewSizeX, const int& nNewSizeY)
{
	if (GetTotalSize() == nNewSizeX*nNewSizeY)
		SetSize(CSize(nNewSizeX, nNewSizeY));
	else
		((class CMatrix *)this)->DimensionErrorMessage(8, GetSize(), CSize(nNewSizeX, nNewSizeY));
}

void CMatrix::reshape(const CSize& nNewSize)
{
	if (GetTotalSize() == nNewSize.cx*nNewSize.cy)
		SetSize(nNewSize);
	else
		((class CMatrix *)this)->DimensionErrorMessage(8, GetSize(), nNewSize);
}

void CMatrix::reshape()
{
	// make it a column vector
	SetSize(GetTotalSize());
}

CMatrix CMatrix::GetRows(const int& nSubscriptX) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	if (nSubscriptX >=1 && nSubscriptX <= GetSize().cx)
	{
		outMatrix.SetSize(CSize(1, GetSize().cy));
		int sizeY = GetSize().cy;
		for (int j = 1; j <= sizeY; j++)
			outMatrix(1,j) = (*this)(nSubscriptX,j);
	}
	else
		((class CMatrix *)this)->DimensionErrorMessage(6, GetSize(), CSize(nSubscriptX, 1));

	return outMatrix;
}

CMatrix CMatrix::GetColumns(const int& nSubscriptY) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	if (nSubscriptY >=1 && nSubscriptY <= GetSize().cy)
	{
		outMatrix.SetSize(CSize(GetSize().cx, 1));
		int sizeX = GetSize().cx;
		for (int i = 1; i <= sizeX; i++)
			outMatrix(i,1) = (*this)(i,nSubscriptY);
	}
	else
		((class CMatrix *)this)->DimensionErrorMessage(7, GetSize(), CSize(1, nSubscriptY));

	return outMatrix;
}

CMatrix CMatrix::GetRows(const CIntMatrix& nSubscriptMatrixX) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	if (nSubscriptMatrixX.GetTotalSize() > 0)
		outMatrix = GetRows(nSubscriptMatrixX[0]);
	int size = nSubscriptMatrixX.GetTotalSize();
	for (int i = 1; i < size; i++)
		outMatrix = concatVert(outMatrix, GetRows(nSubscriptMatrixX[i]));

	return outMatrix;
}

CMatrix CMatrix::GetColumns(const CIntMatrix& nSubscriptMatrixY) const
{
	ASSERT(m_pOutMatrix);
	CMatrix& outMatrix = *m_pOutMatrix;
	if (nSubscriptMatrixY.GetTotalSize() > 0)
		outMatrix = GetColumns(nSubscriptMatrixY[0]);
	int size = nSubscriptMatrixY.GetTotalSize();
	for (int i = 1; i < size; i++)
		outMatrix = concatHor(outMatrix, GetColumns(nSubscriptMatrixY[i]));

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Mathematische CMatrix Funktionen
CMatrix acos(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, acos(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix asin(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, asin(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix atan(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, atan(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix atan2(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, atan2(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix atan2(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, atan2(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix atan2(const CMatrix& inMatrix1, const CMatrix& inMatrix2)		
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, atan2(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, atan2(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, atan2(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix ceil(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, ceil(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix cos(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, cos(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix cosh(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, cosh(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix exp(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, exp(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix fabs(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, fabs(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix fix(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, fix(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix floor(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, floor(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix fmod(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, fmod(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix fmod(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, fmod(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix fmod(const CMatrix& inMatrix1, const CMatrix& inMatrix2)		
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, fmod(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, fmod(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, fmod(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix frem(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, frem(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix frem(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, frem(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix frem(const CMatrix& inMatrix1, const CMatrix& inMatrix2)		
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, frem(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, frem(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, frem(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix log(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, log(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix log10(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, log10(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix pow(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, pow(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix pow(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, pow(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix pow(const CMatrix& inMatrix1, const CMatrix& inMatrix2)		
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, pow(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, pow(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, pow(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix round(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, round(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix sign(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, sign(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix sin(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, sin(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix sinh(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, sinh(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix sqrt(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, sqrt(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix tan(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, tan(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix tanh(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, tanh(inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix wkreset(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, wkreset(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix wkreset(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, wkreset(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix wkreset(const CMatrix& inMatrix1, const CMatrix& inMatrix2)		
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, wkreset(inMatrix1.GetAt(i), inMatrix2.GetAt(i)));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, wkreset(inMatrix1.GetAt(i), inMatrix2.GetAt(0)));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, wkreset(inMatrix1.GetAt(0), inMatrix2.GetAt(i)));
	}
	else
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

double matMax(const CMatrix& inMatrix)		
{
	double outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int i = 1; i < size; i++)
			outValue = max(outValue, inMatrix.GetAt(i));
	}
	else
		outValue = 0.;	

	return outValue;
}

double matMin(const CMatrix& inMatrix)		
{
	double outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int i = 1; i < size; i++)
			outValue = min(outValue, inMatrix.GetAt(i));
	}
	else
		outValue = 0.;	

	return outValue;
}

CMatrix matMax(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix matMax(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix matMax(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
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
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix matMin(const CMatrix& inMatrix, const double& inValue)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inMatrix.GetAt(i), inValue));

	return outMatrix;
}

CMatrix matMin(const double& inValue, const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix matMin(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
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
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(1, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Andere CMatrix Funktionen
CMatrix transp(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix.SetSize(CSize(inMatrix.GetSize().cy, inMatrix.GetSize().cx));
	int sizeX = inMatrix.GetSize().cx;
	int sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			outMatrix(j,i) = inMatrix(i,j);

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	if (inMatrix.GetTotalSize() == nNewSizeX*nNewSizeY)
	{
		outMatrix = inMatrix;
		outMatrix.SetSize(CSize(nNewSizeX, nNewSizeY));
	}
	else
		((class CMatrix *)&inMatrix)->DimensionErrorMessage(8, inMatrix.GetSize(), CSize(nNewSizeX, nNewSizeY));

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix, const CSize& nNewSize)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	if (inMatrix.GetTotalSize() == nNewSize.cx*nNewSize.cy)
	{
		outMatrix = inMatrix;
		outMatrix.SetSize(nNewSize);
	}
	else
		((class CMatrix *)&inMatrix)->DimensionErrorMessage(8, inMatrix.GetSize(), nNewSize);

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix)
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	outMatrix = inMatrix;
	// make it a column vector
	outMatrix.SetSize(inMatrix.GetTotalSize());

	return outMatrix;
}

CMatrix concatHor(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
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
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(3, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix concatVert(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	ASSERT(inMatrix1.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix1.m_pOutMatrix);
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
		((class CMatrix *)&inMatrix1)->DimensionErrorMessage(4, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix rowSum(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	double tmp;
	outMatrix.SetSize(CSize(inMatrix.GetSize().cx, 1));
	int sizeX = inMatrix.GetSize().cx;
	int sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 0.;
		for (int j = 1; j <= sizeY; j++)
			tmp += inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CMatrix colSum(const CMatrix& inMatrix)		
{
	ASSERT(inMatrix.m_pOutMatrix);
	CMatrix& outMatrix = *(inMatrix.m_pOutMatrix);
	double tmp;
	outMatrix.SetSize(CSize(1, inMatrix.GetSize().cy));
	int sizeY = inMatrix.GetSize().cy;
	int sizeX = inMatrix.GetSize().cx;
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 0.;
		for (int i = 1; i <= sizeX; i++)
			tmp += inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

double sum(const CMatrix& inMatrix)		
{
	double tmp = 0.;
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		tmp += inMatrix[i];

	return tmp;
}

void cart2pol(const CMatrix& X, const CMatrix& Y, CMatrix& Theta, CMatrix& Radius)
{
	if (X.GetSize() == Y.GetSize())
	{
		Theta = atan2(Y,X);
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y));
	}
	else if (X.GetTotalSize() > 0 && Y.GetTotalSize() == 1)
	{
		Theta = atan2(Y,X);
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y));
	}
	else if (X.GetTotalSize() == 1 && Y.GetTotalSize() > 0)
	{
		Theta = atan2(Y,X);
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y));
	}
	else
		((class CMatrix *)&X)->DimensionErrorMessage(1, X.GetSize(), Y.GetSize());
}

void cart2sph(const CMatrix& X, const CMatrix& Y, const CMatrix& Z, CMatrix& Alpha, CMatrix& Theta, CMatrix& Radius)
{
	if (X.GetSize() == Y.GetSize() && X.GetSize() == Z.GetSize())
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (X.GetSize() == Z.GetSize() && X.GetTotalSize() > 0 && Y.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (Y.GetSize() == X.GetSize() && Y.GetTotalSize() > 0 && Z.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (Z.GetSize() == Y.GetSize() && Z.GetTotalSize() > 0 && X.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (X.GetSize() == Z.GetSize() && X.GetTotalSize() == 1 && Y.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (Y.GetSize() == X.GetSize() && Y.GetTotalSize() == 1 && Z.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,CMatrix(Z.GetSize(), X[0]));
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (Z.GetSize() == Y.GetSize() && Z.GetTotalSize() == 1 && X.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else
		((class CMatrix *)&X)->DimensionErrorMessage(1, X.GetSize(), Y.GetSize(), Z.GetSize());
}

void pol2cart(const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y)
{
	if (Theta.GetSize() == Radius.GetSize())
	{
		X = Radius.DotTimes(cos(Theta));
		Y = Radius.DotTimes(sin(Theta));
	}
	else if (Theta.GetTotalSize() > 0 && Radius.GetTotalSize() == 1)
	{
		X = cos(Theta)*Radius;
		Y = sin(Theta)*Radius;
	}
	else if (Theta.GetTotalSize() == 1 && Radius.GetTotalSize() > 0)
	{
		X = Radius*cos(Theta[0]);
		Y = Radius*sin(Theta[0]);
	}
	else
		((class CMatrix *)&Theta)->DimensionErrorMessage(1, Theta.GetSize(), Radius.GetSize());
}

void sph2cart(const CMatrix& Alpha, const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y, CMatrix& Z)
{
	if (Alpha.GetSize() == Theta.GetSize() && Alpha.GetSize() == Radius.GetSize())
	{
		X = Radius.DotTimes(cos(Theta)).DotTimes(cos(Alpha));
		Y = Radius.DotTimes(cos(Theta)).DotTimes(sin(Alpha));
		Z = Radius.DotTimes(sin(Theta));
	}
	else if (Alpha.GetSize() == Radius.GetSize() && Alpha.GetTotalSize() > 0 && Theta.GetTotalSize() == 1)
	{
		X = Radius.DotTimes(cos(Alpha))*cos(Theta[0]);
		Y = Radius.DotTimes(sin(Alpha))*cos(Theta[0]);
		Z = Radius*sin(Theta[0]);
	}
	else if (Theta.GetSize() == Alpha.GetSize() && Theta.GetTotalSize() > 0 && Radius.GetTotalSize() == 1)
	{
		X = cos(Theta).DotTimes(cos(Alpha))*Radius;
		Y = cos(Theta).DotTimes(sin(Alpha))*Radius;
		Z = sin(Theta)*Radius;
	}
	else if (Radius.GetSize() == Theta.GetSize() && Radius.GetTotalSize() > 0 && Alpha.GetTotalSize() == 1)
	{
		X = Radius.DotTimes(cos(Theta))*cos(Alpha[0]);
		Y = Radius.DotTimes(cos(Theta))*sin(Alpha[0]);
		Z = Radius.DotTimes(sin(Theta));
	}
	else if (Alpha.GetSize() == Radius.GetSize() && Alpha.GetTotalSize() == 1 && Theta.GetTotalSize() > 0)
	{
		X = cos(Theta)*Radius[0]*cos(Alpha[0]);
		Y = cos(Theta)*Radius[0]*sin(Alpha[0]);
		Z = sin(Theta)*Radius[0];
	}
	else if (Theta.GetSize() == Alpha.GetSize() && Theta.GetTotalSize() == 1 && Radius.GetTotalSize() > 0)
	{
		X = Radius*cos(Theta[0])*cos(Alpha[0]);
		Y = Radius*cos(Theta[0])*sin(Alpha[0]);
		Z = Radius*sin(Theta[0]);
	}
	else if (Radius.GetSize() == Theta.GetSize() && Radius.GetTotalSize() == 1 && Alpha.GetTotalSize() > 0)
	{
		X = cos(Alpha)*Radius[0]*cos(Theta[0]);
		Y = sin(Alpha)*Radius[0]*cos(Theta[0]);
		Z = CMatrix(Alpha.GetSize(),Radius[0])*sin(Theta[0]);
	}
	else
		((class CMatrix *)&Alpha)->DimensionErrorMessage(1, Alpha.GetSize(), Theta.GetSize(), Radius.GetSize());
}
