/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: July 1998
//
// based on a sample of
// Oleg G. Galkin
//
/////////////////////////////////////////////////////////////////////////////
// SplitterWndEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class AFX_EXT_CLASS CSplitterWndEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterWndEx)
// Konstruktion
public:
	CSplitterWndEx();
	// Create a single view type splitter with multiple splits
	BOOL Create(CWnd* pParentWnd,
				int nMaxRows, int nMaxCols, SIZE sizeMin,
				CCreateContext* pContext,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE |
					WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,
				UINT nID = AFX_IDW_PANE_FIRST);

	// Create a multiple view type splitter with static layout
	BOOL CreateStatic(CWnd* pParentWnd,
				int nRows, int nCols,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE,
				UINT nID = AFX_IDW_PANE_FIRST);

	struct CRowColInfoEx
	{
		int nRowColNumber;	// number of row/column to be hidden/shown ( zerobased; -1 => nonexistant )
		BOOL bIsHidden;		// current state
		CRowColInfoEx& operator=(const CRowColInfoEx& otherRowColInfo) {nRowColNumber = otherRowColInfo.nRowColNumber; bIsHidden = otherRowColInfo.bIsHidden; return *this;}
	};

// Attribute
protected:
	CArray<CRowColInfoEx,CRowColInfoEx> m_ColInfoEx;
	CArray<CRowColInfoEx,CRowColInfoEx> m_RowInfoEx;
	BOOL m_bEnableResizing;

// Operationen
public:
	void ShowColumn(int colShow);
	void ShowRow(int rowShow);
	void HideColumn(int colHide);
	void HideRow(int rowHide);
	BOOL IsColHidden(int col);
	BOOL IsRowHidden(int row);
	int GetHiddenColumnCount();
	int GetHiddenRowCount();
	int GetSplitterBarWidth();
	int GetSplitterBarHeight();

// Überschreibungen
	// determining active pane from focus or active view in frame
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSplitterWndEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CSplitterWndEx();

// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
