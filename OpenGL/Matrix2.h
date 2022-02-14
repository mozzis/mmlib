/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// Matrix.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Matrix CMatrix 

class CIntMatrix;

class AFX_EXT_CLASS CMatrix : public CArray<double, double>
{
public:
	CMatrix* m_pOutMatrix;
	CIntMatrix* m_pOutIntMatrix;

	CMatrix(void* dummyPointer);
	CMatrix();
	CMatrix(const double& inValue);
	CMatrix(const double& firstValue, const double& lastValue, const double& stepSize = 1.);
	CMatrix(const CMatrix& inMatrix);
	CMatrix(const CIntMatrix& inMatrix);
	CMatrix(const CSize& inSize, const double& initValue = 0.);

	void PrepareOutMatrices();
	int GetTotalSize() const;
	void Empty();
	BOOL IsEmpty();
	BOOL IsVector();
	void DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo);
	void DimensionErrorMessage(int i, CSize sizeOne, CSize sizeTwo, CSize sizeThree);

	double operator()(const int& nSubscriptX, const int& nSubscriptY) const;
	double& operator()(const int& nSubscriptX, const int& nSubscriptY);
	CMatrix operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const;

	CMatrix& operator=(const CMatrix& inMatrix);
	CMatrix& operator=(const CIntMatrix& inMatrix);

	void operator+=(const double& inValue);
	void operator-=(const double& inValue);
	void operator*=(const double& inValue);
	void operator/=(const double& inValue);

	void operator+=(const CMatrix& inMatrix);
	void operator-=(const CMatrix& inMatrix);
	void DotTimesAssign(const CMatrix& inMatrix);
	void DotOverAssign(const CMatrix& inMatrix);

	void operator+=(const CIntMatrix& inMatrix);
	void operator-=(const CIntMatrix& inMatrix);
	void DotTimesAssign(const CIntMatrix& inMatrix);
	void DotOverAssign(const CIntMatrix& inMatrix);

	CMatrix operator+(const int& inValue) const;
	CMatrix operator-(const int& inValue) const;
	CMatrix operator*(const int& inValue) const;
	CMatrix operator/(const int& inValue) const;
	CIntMatrix operator<(const int& inValue) const;
	CIntMatrix operator>(const int& inValue) const;
	CIntMatrix operator<=(const int& inValue) const;
	CIntMatrix operator>=(const int& inValue) const;

	friend CMatrix AFX_EXT_API operator+(const int& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator-(const int& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator*(const int& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator/(const int& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<(const int& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>(const int& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<=(const int& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>=(const int& inValue, const CMatrix& inMatrix);

	CMatrix operator+(const double& inValue) const;
	CMatrix operator-(const double& inValue) const;
	CMatrix operator*(const double& inValue) const;
	CMatrix operator/(const double& inValue) const;
	CIntMatrix operator<(const double& inValue) const;
	CIntMatrix operator>(const double& inValue) const;
	CIntMatrix operator<=(const double& inValue) const;
	CIntMatrix operator>=(const double& inValue) const;

	friend CMatrix AFX_EXT_API operator+(const double& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator-(const double& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator*(const double& inValue, const CMatrix& inMatrix);
	friend CMatrix AFX_EXT_API operator/(const double& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<(const double& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>(const double& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator<=(const double& inValue, const CMatrix& inMatrix);
	friend CIntMatrix AFX_EXT_API operator>=(const double& inValue, const CMatrix& inMatrix);

	CMatrix operator+(const CMatrix& inMatrix) const;
	CMatrix operator-(const CMatrix& inMatrix) const;
	CMatrix DotTimes(const CMatrix& inMatrix) const;
	CMatrix DotOver(const CMatrix& inMatrix) const;
	CIntMatrix operator<(const CMatrix& inMatrix) const;
	CIntMatrix operator>(const CMatrix& inMatrix) const;
	CIntMatrix operator<=(const CMatrix& inMatrix) const;
	CIntMatrix operator>=(const CMatrix& inMatrix) const;

	CMatrix operator+(const CIntMatrix& inMatrix) const;
	CMatrix operator-(const CIntMatrix& inMatrix) const;
	CMatrix DotTimes(const CIntMatrix& inMatrix) const;
	CMatrix DotOver(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<=(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>=(const CIntMatrix& inMatrix) const;

	CMatrix operator-() const;
	CMatrix operator*(const CMatrix& inMatrix) const;
	CMatrix operator*(const CIntMatrix& inMatrix) const;

	void reshape(const int& nNewSizeX, const int& nNewSizeY);
	void reshape(const CSize& nNewSize);
	void reshape();

	CMatrix GetRows(const int& nSubscriptX) const;
	CMatrix GetColumns(const int& nSubscriptY) const;
	CMatrix GetRows(const CIntMatrix& nSubscriptMatrixX) const;
	CMatrix GetColumns(const CIntMatrix& nSubscriptMatrixY) const;

protected:
	int m_nXSize, m_nYSize;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMatrix)
	public:
	virtual void SetSize(int nNewSize, int nGrowBy = -1);
	virtual void SetSize(CSize nNewSize, int nGrowBy = -1);
	virtual CSize GetSize() const;
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMatrix();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CMatrix)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////
// Mathematische CMatrix Funktionen
CMatrix AFX_EXT_API acos(const CMatrix& inMatrix);
CMatrix AFX_EXT_API asin(const CMatrix& inMatrix);
CMatrix AFX_EXT_API atan(const CMatrix& inMatrix);
CMatrix AFX_EXT_API atan2(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API atan2(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API atan2(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API ceil(const CMatrix& inMatrix);
CMatrix AFX_EXT_API cos(const CMatrix& inMatrix);
CMatrix AFX_EXT_API cosh(const CMatrix& inMatrix);
CMatrix AFX_EXT_API exp(const CMatrix& inMatrix);
CMatrix AFX_EXT_API fabs(const CMatrix& inMatrix);
CMatrix AFX_EXT_API fix(const CMatrix& inMatrix);
CMatrix AFX_EXT_API floor(const CMatrix& inMatrix);
CMatrix AFX_EXT_API fmod(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API fmod(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API fmod(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API frem(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API frem(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API frem(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API log(const CMatrix& inMatrix);
CMatrix AFX_EXT_API log10(const CMatrix& inMatrix);
CMatrix AFX_EXT_API pow(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API pow(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API pow(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API round(const CMatrix& inMatrix);
CMatrix AFX_EXT_API sign(const CMatrix& inMatrix);
CMatrix AFX_EXT_API sin(const CMatrix& inMatrix);
CMatrix AFX_EXT_API sinh(const CMatrix& inMatrix);
CMatrix AFX_EXT_API sqrt(const CMatrix& inMatrix);
CMatrix AFX_EXT_API tan(const CMatrix& inMatrix);
CMatrix AFX_EXT_API tanh(const CMatrix& inMatrix);
CMatrix AFX_EXT_API wkreset(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API wkreset(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API wkreset(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
double AFX_EXT_API matMax(const CMatrix& inMatrix);
double AFX_EXT_API matMin(const CMatrix& inMatrix);
CMatrix AFX_EXT_API matMax(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API matMax(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API matMax(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API matMin(const CMatrix& inMatrix, const double& inValue);
CMatrix AFX_EXT_API matMin(const double& inValue, const CMatrix& inMatrix);
CMatrix AFX_EXT_API matMin(const CMatrix& inMatrix1, const CMatrix& inMatrix2);

/////////////////////////////////////////////////////////////////////////////
// Andere CMatrix Funktionen
CMatrix AFX_EXT_API transp(const CMatrix& inMatrix);
CMatrix AFX_EXT_API reshape(const CMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY);
CMatrix AFX_EXT_API reshape(const CMatrix& inMatrix, const CSize& nNewSize);
CMatrix AFX_EXT_API reshape(const CMatrix& inMatrix);
CMatrix AFX_EXT_API concatHor(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API concatVert(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
CMatrix AFX_EXT_API rowSum(const CMatrix& inMatrix);
CMatrix AFX_EXT_API colSum(const CMatrix& inMatrix);
double AFX_EXT_API sum(const CMatrix& inMatrix);

void AFX_EXT_API cart2pol(const CMatrix& X, const CMatrix& Y, CMatrix& Theta, CMatrix& Radius);
void AFX_EXT_API cart2sph(const CMatrix& X, const CMatrix& Y, const CMatrix& Z, CMatrix& Alpha, CMatrix& Theta, CMatrix& Radius);
void AFX_EXT_API pol2cart(const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y);
void AFX_EXT_API sph2cart(const CMatrix& Alpha, const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y, CMatrix& Z);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
