/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <kotyczka@bnm-gmbh.de>
// created: April 1999
//
// based on a sample of
// Keith Rule <keithr@europa.com>
//
/////////////////////////////////////////////////////////////////////////////
// MemDC.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMDC_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_MEMDC_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMemDC : public CDC
{
private:
	CBitmap  m_Bitmap;		// Offscreen bitmap
	CBitmap* m_pOldBitmap;	// bitmap originally found in CMemDC
	CDC*	 m_pDC;			// Saves CDC passed in constructor
	CRect    m_Rect;		// Rectangle of drawing area.
	BOOL     m_bMemDC;		// TRUE if CDC really is a Memory DC.

public:
	// constructor sets up the memory DC
	CMemDC(CDC* pDC) : CDC()
	{
		CommonConstruct(pDC);
	}

	// modified the constructor to take two parameters
	CMemDC(CDC* pDC, CRect &rect) : CDC()
	{
		m_Rect = rect;
		CommonConstruct(pDC);
	}

	void CommonConstruct(CDC* pDC)
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_pOldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();
			  
		if (m_bMemDC)	// Create a Memory DC
		{
			pDC->GetClipBox(&m_Rect);
		    CreateCompatibleDC(pDC);
		    m_Bitmap.CreateCompatibleBitmap(pDC, m_Rect.Width(), m_Rect.Height());
			m_pOldBitmap = SelectObject(&m_Bitmap);
		    SetWindowOrg(m_Rect.left, m_Rect.top);
		}
		else			// Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	// Destructor copies the contents of the mem DC to the original DC
	~CMemDC()
	{
		if (m_bMemDC) 
		{	
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(), 
					this, m_Rect.left, m_Rect.top, SRCCOPY);

			// Swap back the original bitmap.
			SelectObject(m_pOldBitmap);
		} 
		else 
		{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
		    m_hDC = m_hAttribDC = NULL;
		}
	}

	// Allow usage as a pointer
    CMemDC* operator->() {return this;}
		
	// Allow usage as a pointer
    operator CMemDC*() {return this;}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_MEMDC_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
