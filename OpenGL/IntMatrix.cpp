/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// IntMatrix.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Matrix.h"
#include "IntMatrix.h"
#include "MathAdd.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntMatrix

CIntMatrix::CIntMatrix()
{
	// this is an empty matrix
	SetSize(0);					
}

CIntMatrix::CIntMatrix(const int& inValue)
{
	// copy inMatrix
	SetSize(1);
	SetAt(0, inValue);
}

CIntMatrix::CIntMatrix(const int& firstValue, const int& lastValue, const double& stepSize)
{
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
	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, inMatrix.GetAt(i));
}

CIntMatrix::CIntMatrix(const CMatrix& inMatrix)
{
	// copy inMatrix
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, int(inMatrix.GetAt(i)));
}

CIntMatrix::CIntMatrix(const CSize& inSize, const int& initValue)
{
	// initialize with initValue
	SetSize(inSize);
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		SetAt(i, initValue);
}

CIntMatrix::~CIntMatrix()
{
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
// CIntMatrix Serialisierung

void CIntMatrix::Serialize(CArchive& ar, unsigned char numByte)
{
	ASSERT(numByte == 1 || numByte == 2 || numByte == 4);
	int i, j, sizeX, sizeY;
	int tmpInt, minVal = 0, maxVal = 0, fac = 1;
	UINT tmpUINT;
	char tmpChar;
	short tmpShort;

	if (ar.IsStoring())
	{
		// TODO: add storing code here

		sizeX = GetSize().cx;
		sizeY = GetSize().cy;
		ar << numByte;
		ar << sizeX;
		ar << sizeY;
		if (numByte < 4)
		{
			int maxDiff = (numByte == 1) ? 256 : 65536;

			minVal = matMin(*this);
			maxVal = matMax(*this);
			tmpUINT = maxVal - minVal;
			while (tmpUINT/fac >= (UINT)maxDiff)
				fac *= 2;
			ar << minVal;
			ar << fac;
		}
		for (j = 1; j <= sizeY; j++)
		{
			for (i = 1; i <= sizeX; i++)
			{
				tmpInt = (*this)(i,j);
				switch (numByte)
				{
				case 1:
					tmpUINT = tmpInt - minVal;
					tmpUINT /= fac;
					tmpChar = (char)(int(tmpUINT) - 128);
					ar << tmpChar;
					break;
				case 2:
					tmpUINT = tmpInt - minVal;
					tmpUINT /= fac;
					tmpShort = (short)(int(tmpUINT) - 32768);
					ar << tmpShort;
					break;
				case 4:
					ar << tmpInt;
					break;
				default:
					{}
				}
			}
		}
	}
	else
	{
		// TODO: add loading code here

		ar >> numByte;
		ar >> sizeX;
		ar >> sizeY;
		if (numByte < 4)
		{
			ar >> minVal;
			ar >> fac;
		}
		SetSize(CSize(sizeX,sizeY));
		sizeY = GetSize().cy;
		sizeX = GetSize().cx;
		for (j = 1; j <= sizeY; j++)
		{
			for (i = 1; i <= sizeX; i++)
			{
				switch (numByte)
				{
				case 1:
					ar >> tmpChar;
					tmpUINT = tmpChar + 128;
					tmpUINT *= fac;
					(*this)(i,j) = int(tmpUINT + minVal);
					break;
				case 2:
					ar >> tmpShort;
					tmpUINT = tmpShort + 32768;
					tmpUINT *= fac;
					(*this)(i,j) = int(tmpUINT + minVal);
					break;
				case 4:
					ar >> (*this)(i,j);
					break;
				default:
					{}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CIntMatrix 
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

void AFXAPI operator<<(CArchive& ar, CIntMatrix& matrix)	// friend function of CIntMatrix
{
	matrix.Serialize(ar);
}

void AFXAPI operator>>(CArchive& ar, CIntMatrix& matrix)	// friend function of CIntMatrix
{
	matrix.Serialize(ar);
}

int CIntMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY) const
{
	if (nSubscriptX >= 1 && nSubscriptX <= GetSize().cx && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
		return (*this)[(nSubscriptY-1)*GetSize().cx+nSubscriptX-1];
	else
	{
		((class CIntMatrix *)this)->DimensionErrorMessage(5, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return  0;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix -= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator/(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator==(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) == inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) != inValue));

	return outMatrix;
}

CIntMatrix operator+(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix operator-(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix -= inValue;

	return -outMatrix;
}

CIntMatrix operator*(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix operator/(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix = CMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator==(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue == inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator!=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue != inMatrix.GetAt(i)));

	return outMatrix;
}

CMatrix CIntMatrix::operator+(const double& inValue) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const double& inValue) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix -= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator*(const double& inValue) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator/(const double& inValue) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) >= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator==(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) == inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(GetAt(i) != inValue));

	return outMatrix;
}

CMatrix operator+(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix += inValue;

	return outMatrix;
}

CMatrix operator-(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix -= inValue;

	return -outMatrix;
}

CMatrix operator*(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix;
	outMatrix = inMatrix;
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix operator/(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix;
	outMatrix = CMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue < inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue > inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator<=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue <= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator>=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue >= inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator==(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue == inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix operator!=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, int(inValue != inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator+(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::DotTimes(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::DotOver(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CIntMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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

CIntMatrix CIntMatrix::operator==(const CIntMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) == inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) == inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) == inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const CIntMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) != inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) != inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) != inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CIntMatrix::operator+(const CMatrix& inMatrix) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const CMatrix& inMatrix) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix -= inMatrix;

	return -outMatrix;
}

CMatrix CIntMatrix::DotTimes(const CMatrix& inMatrix) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CIntMatrix::DotOver(const CMatrix& inMatrix) const
{
	CMatrix outMatrix;
	outMatrix = *this;
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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

CIntMatrix CIntMatrix::operator==(const CMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) == inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) == inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) == inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const CMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) != inMatrix.GetAt(i)));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(i) != inMatrix.GetAt(0)));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int i = 0; i < size; i++)
			outMatrix.SetAt(i, int(GetAt(0) != inMatrix.GetAt(i)));
	}
	else
		((class CIntMatrix *)this)->DimensionErrorMessage(1, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-() const
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, -GetAt(i));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const CIntMatrix& inMatrix) const
{	
	CIntMatrix outMatrix;
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
	CMatrix outMatrix;
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
	CIntMatrix outMatrix;
	
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
	if (nSubscriptMatrixX.GetTotalSize() > 0)
		outMatrix = GetRows(nSubscriptMatrixX[0]);
	int size = nSubscriptMatrixX.GetTotalSize();
	for (int i = 1; i < size; i++)
		outMatrix = concatVert(outMatrix, GetRows(nSubscriptMatrixX[i]));

	return outMatrix;
}

CIntMatrix CIntMatrix::GetColumns(const CIntMatrix& nSubscriptMatrixY) const
{
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMax(const int& inValue, const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, max(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMax(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMin(const int& inValue, const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int i = 0; i < size; i++)
		outMatrix.SetAt(i, min(inValue, inMatrix.GetAt(i)));

	return outMatrix;
}

CIntMatrix matMin(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
	outMatrix = inMatrix;
	// make it a column vector
	outMatrix.SetSize(inMatrix.GetTotalSize());

	return outMatrix;
}

CIntMatrix concatHor(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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
	CIntMatrix outMatrix;
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

void reduceSize(CIntMatrix& inMatrix, const int& maxTotal)
{
	int maxRows, maxColumns;

	if (inMatrix.IsEmpty())
		return;

	maxRows = (int)ceil(sqrt(maxTotal * double(inMatrix.GetSize().cx)/double(inMatrix.GetSize().cy)));
	maxColumns = (int)ceil(sqrt(maxTotal * double(inMatrix.GetSize().cy)/double(inMatrix.GetSize().cx)));
	reduceSize(inMatrix, maxRows, maxColumns);
}

void reduceSize(CIntMatrix& inMatrix, const int& maxRows, const int& maxColumns)
{
	ASSERT(maxRows > 0);
	ASSERT(maxColumns > 0);

	int rows, columns, sizeX, sizeY, rowFac = 1, columnFac = 1;

	rows = inMatrix.GetSize().cx;
	columns = inMatrix.GetSize().cy;

	while (rows > maxRows)
	{
		rows = ((rows+1)&~1)/2;	// rounding up
		rowFac *=2;
	}
	while (columns > maxColumns)
	{
		columns = ((columns+1)&~1)/2;	// rounding up
		columnFac *=2;
	}
	if (rowFac > 1 || columnFac > 1)
	{
		CIntMatrix tmpMatrix(inMatrix);
		int indexX, indexY;
		double value;
		inMatrix.SetSize(CSize(rows, columns));
		sizeX = tmpMatrix.GetSize().cx;
		sizeY = tmpMatrix.GetSize().cy;

		for (int i = 1; i <= rows; i++)
		{
			for (int j = 1; j <= columns; j++)
			{
				value = 0.;
				for (int k = 1; k <= rowFac; k++)
				{
					indexX = min(sizeX, (i-1)*rowFac + k); 
					for (int l = 1; l <= columnFac; l++)
					{
						indexY = min(sizeY, (j-1)*columnFac + l); 
						value += double(tmpMatrix(indexX, indexY));
					}
				}
				value /= double(rowFac*columnFac);
				inMatrix(i, j) = (int)value;
			}
		}
	}
}
