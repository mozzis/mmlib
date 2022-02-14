#pragma once


// CGridCellItem command target

class CGridCellItem : public CObject
{
public:
	CGridCellItem();
	virtual ~CGridCellItem();

  union {
    double m_dValue;
    int    m_nValue;
    bool   m_bValue;
    TCHAR  m_sValue[80];
  } m_Value;
  enum eDataType m_eType;
  int m_nFieldWidth;
};


