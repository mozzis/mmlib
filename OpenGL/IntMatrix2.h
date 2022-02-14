/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// IntMatrix.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Matrix CIntMatrix 

class CMatrix;

class AFX_EXT_CLASS CIntMatrix : public CArray<int, int>
{
public:
	CMatrix* m_pOutMatrix;
	CIntMatrix* m_pOutIntMatrix;

	CIntMatrix(void* dummyPointer);
	CIntMatrix();
	CIntMatrix(const int& inValue);
	CIntMatrix(const int& firstValue, const int& lastValue, const double& stepSize = 1.);
	CIntMatrix(const CIntMatrix& inMatrix);
	CIntMatrix(const CMatrix& inMatrix);
	CIntMatrix(const CSize& inSize, const int& initValue = 0.);

	void PrepareOutMatrices();
	int GetTotalSize() const;
	void Empty();
	BOOL IsEmpty();
	BOOL IsVector();
	void DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo);

	int operator()(const int& nSubscriptX, const int& nSubscriptY) const;
	int& operator()(const int& nSubscriptX, const int& nSubscriptY);
	CIntMatrix operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const;

	CIntMatrix& operator=(const CIntMatrix& inMatrix);
	CIntMatrix& operator=(const CMatrix& inMatrix);

	void operator+=(const int& inValue);
	void operator-=(const int& inValue);
	void operator*=(const int& inValue);
	void operator/=(const int& inValue);

	void operator+=(const CIntMatrix& inMatrix);
	void operator-=(const CIntMatrix& inMatrix);
	void DotTimesAssign(const CIntMatrix& inMatrix);
	void DotOverAssign(const CIntMatrix& inMatrix);

	CIntMatrix operator+(const int& inValue) const;
	CIntMatrix operator-(const int& inValue) const;
	CIntMatrix operator*(const int& inValue) const;
	CIntMatrix operator/(const int& inValue) const;
	CIntMatrix operator<(const int& inValue) const;
	CIntMatrix operator>(const int& inValue) const;
	CIntMatrix operator<=(const int& inValue) const;
	CIntMatrix operator>=(const int& inValue) const;

	friend CIntMatrix AFX_EXT_API operator+(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator-(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator*(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator/(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<=(const int& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>=(const int& inValue, const CIntMatrix& inMatrix);

	CMatrix operator+(const double& inValue) const;
	CMatrix operator-(const double& inValue) const;
	CMatrix operator*(const double& inValue) const;
	CMatrix operator/(const double& inValue) const;
	CIntMatrix operator<(const double& inValue) const;
	CIntMatrix operator>(const double& inValue) const;
	CIntMatrix operator<=(const double& inValue) const;
	CIntMatrix operator>=(const double& inValue) const;

	friend CMatrix AFX_EXT_API operator+(const double& inValue, const CIntMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator-(const double& inValue, const CIntMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator*(const double& inValue, const CIntMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator/(const double& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<(const double& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>(const double& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<=(const double& inValue, const CIntMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>=(const double& inValue, const CIntMatrix& inMatrix);

	CIntMatrix operator+(const CIntMatrix& inMatrix) const;
	CIntMatrix operator-(const CIntMatrix& inMatrix) const;
	CIntMatrix DotTimes(const CIntMatrix& inMatrix) const;
	CIntMatrix DotOver(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<=(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>=(const CIntMatrix& inMatrix) const;

	CMatrix operator+(const CMatrix& inMatrix) const;
	CMatrix operator-(const CMatrix& inMatrix) const;
	CMatrix DotTimes(const CMatrix& inMatrix) const;
	CMatrix DotOver(const CMatrix& inMatrix) const;
	CIntMatrix operator<(const CMatrix& inMatrix) const;
	CIntMatrix operator>(const CMatrix& inMatrix) const;
	CIntMatrix operator<=(const CMatrix& inMatrix) const;
	CIntMatrix operator>=(const CMatrix& inMatrix) const;

	CIntMatrix operator-() const;
	CIntMatrix operator*(const CIntMatrix& inMatrix) const;
	CMatrix operator*(const CMatrix& inMatrix) const;

	void reshape(const int& nNewSizeX, const int& nNewSizeY);
	void reshape(const CSize& nNewSize);
	void reshape();

	CIntMatrix GetRows(const int& nSubscriptX) const;
	CIntMatrix GetColumns(const int& nSubscriptY) const;
	CIntMatrix GetRows(const CIntMatrix& nSubscriptMatrixX) const;
	CIntMatrix GetColumns(const CIntMatrix& nSubscriptMatrixY) const;

protected:
	int m_nXSize, m_nYSize;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CIntMatrix)
	public:
	virtual void SetSize(int nNewSize, int nGrowBy = -1);
	virtual void SetSize(CSize nNewSize, int nGrowBy = -1);
	virtual CSize GetSize() const;
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CIntMatrix();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CIntMatrix)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////
// Mathematische CIntMatrix Funktionen
CIntMatrix AFX_EXT_API abs(const CIntMatrix& inMatrix);
int AFX_EXT_API matMax(const CIntMatrix& inMatrix);
int AFX_EXT_API matMin(const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API matMax(const CIntMatrix& inMatrix, const int& inValue);
CIntMatrix AFX_EXT_API matMax(const int& inValue, const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API matMax(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
CIntMatrix AFX_EXT_API matMin(const CIntMatrix& inMatrix, const int& inValue);
CIntMatrix AFX_EXT_API matMin(const int& inValue, const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API matMin(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);

/////////////////////////////////////////////////////////////////////////////
// Andere CIntMatrix Funktionen
CIntMatrix AFX_EXT_API transp(const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API reshape(const CIntMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY);
CIntMatrix AFX_EXT_API reshape(const CIntMatrix& inMatrix, const CSize& nNewSize);
CIntMatrix AFX_EXT_API reshape(const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API concatHor(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
CIntMatrix AFX_EXT_API concatVert(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
CIntMatrix AFX_EXT_API rowSum(const CIntMatrix& inMatrix);
CIntMatrix AFX_EXT_API colSum(const CIntMatrix& inMatrix);
int AFX_EXT_API sum(const CIntMatrix& inMatrix);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
