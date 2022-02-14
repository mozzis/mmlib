#include "stdafx.h"
#include "MemDC.h"


CMemDC::CMemDC(CDC* pDC) : CDC()
{
  ASSERT(pDC != NULL);

  m_pDC = pDC;
  m_pOldBitmap = NULL;
  m_bMemDC = !pDC->IsPrinting();
        
  if (m_bMemDC)    // Create a Memory DC
   {
   pDC->GetClipBox(&m_rect);
   CreateCompatibleDC(pDC);
   m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
   m_pOldBitmap = SelectObject(&m_bitmap);
   SetWindowOrg(m_rect.left, m_rect.top);
   }
  else        // Make a copy of the relevent parts of the current DC for printing
    {
    m_bPrinting = pDC->m_bPrinting;
    m_hDC       = pDC->m_hDC;
    m_hAttribDC = pDC->m_hAttribDC;
    }
}

CMemDC::CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
{
  m_bMemDC = !pDC->IsPrinting();
  if (m_bMemDC)    // Create a Memory DC
   {
   CreateCompatibleDC(pDC);
   m_bitmap.CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
   m_pOldBitmap = SelectObject(&m_bitmap);
   }
  else
   {
   m_bPrinting = pDC->m_bPrinting;
   m_hDC       = pDC->m_hDC;
   m_hAttribDC = pDC->m_hAttribDC;
   }
  m_pDC = pDC;
  m_rect = rcBounds;
}

CMemDC::~CMemDC()
{
if (m_bMemDC) 
  {    
  // Copy the offscreen bitmap onto the screen.
  m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);

  //Swap back the original bitmap.
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
