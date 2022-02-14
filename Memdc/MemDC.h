#ifndef _MEMDC__H
#define _MEMDC__H

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a memory Device Context

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//
// This class implements a memory Device Context

class CMemDC : public CDC
{
public:

  // constructor sets up the memory DC
  CMemDC(CDC* pDC);
  
  CMemDC(CDC* pDC, const CRect& rcBounds);

  // Destructor copies the contents of the mem DC to the original DC
  ~CMemDC();

  // Allow usage as a pointer
  CMemDC* operator->() {return this;}
      
  // Allow usage as a pointer
  operator CMemDC*() {return this;}

private:
  CBitmap  m_bitmap;      // Offscreen bitmap
  CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
  CDC*     m_pDC;         // Saves CDC passed in constructor
  CRect    m_rect;        // Rectangle of drawing area.
  BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

#endif
