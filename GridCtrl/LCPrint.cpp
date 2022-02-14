// LCPrint.cpp.cpp: implement an list control printing class
//
// Copyright © 2001-2002, Mayn Idea, Inc.
// Copyright © 2001-2002, Metorex USA, Inc.
// Original code by Gregory Goeppel
//////////////////////////////////////////////////////////////////////////
/*   $History: LCPrint.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Morris       Date: 11/11/03   Time: 10:09a
 * Created in $/mmlib/GridCtrl
*  
*/
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LCPrint.h"
#include "GridCtrl.h"


#define LEFT_MARGIN 2
#define RIGHT_MARGIN 4
#define HEADER_HEIGHT 1
#define FOOTER_HEIGHT 3

//Set it all to 0
//********************************CONSTRUCTOR************************************
LCPrinting::LCPrinting()
{
  m_pLC = 0;
  pOldFont = 0;
  TitleStr = "";
  DateStr = "";
  
  page_rc.SetRect(0,0,0,0);	
  m_nRowHeight = 0;
  m_nRowsPerPage = 0;
  m_nMaxRowCount = 0;
  m_ratiox = 0;
  m_ratioy = 0;
  hc_items = 0;
  return;
}

//Using default for printer guess at # of pages.
//If no printer exists return FALSE;
//************************ONPREPAREPRINTING*******************************
BOOL LCPrinting::OnPreparePrinting(CPrintInfo* pInfo, CView * cview, CGridCtrl * t_lc) 
{
  if(t_lc==NULL || cview==NULL || pInfo == NULL)          return FALSE;
    m_pLC = t_lc;//Set Pointer to list Control
  
  //Lets make a guess as to how many pages there are based on the default printer.
  PRINTDLG PrtDlg;
  if (!AfxGetApp()->GetPrinterDeviceDefaults(&PrtDlg)) 
    return FALSE;//If no defaults then no printer!!
  CPrintDialog pdlg(FALSE);
  pdlg.m_pd.hDevMode = PrtDlg.hDevMode;
  pdlg.m_pd.hDevNames = PrtDlg.hDevNames;
  // pdlg.GetDevMode()->dmOrientation =//if you want, print only in this orientation, see DEVMODE in online-help
  // pdlg.GetDevMode()->dmPaperSize =  //if you want, print only in this papersize, see DEVMODE in online-help
  HDC hDC = pdlg.CreatePrinterDC();
  CDC* t_pDC = CDC::FromHandle(hDC);
  compute_metrics(t_pDC);

  m_nMaxRowCount = m_pLC->GetRowCount(); 
  if(!m_nMaxRowCount) 
    return FALSE;//Get the number of rows
  int nMaxPage = m_nMaxRowCount/m_nRowsPerPage + 1;
  pInfo->SetMaxPage(nMaxPage);
  pInfo->m_nCurPage = 1;  // start printing at page# 1
  
  //If you want to be able to do this remove it.
  pInfo->m_pPD->m_pd.Flags |=PD_HIDEPRINTTOFILE; 
  
  return cview->DoPreparePrinting(pInfo);
}

//Call this from your dialog class OnBeingPrinting function
//*************************ONBEGINGPRINTING*************************
void LCPrinting::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo, CString& t_title, CString& t_date)
{
  if(pDC == NULL || pInfo==NULL)          
    return;
  
  TitleStr = t_title;
  DateStr = t_date;
  
  //create lc font, and Bold lc Font
  LOGFONT  lf;
  CFont * lcfont_ptr = m_pLC->GetFont();
  lcfont_ptr->GetLogFont(&lf);
  lf.lfHeight = -10;
  lcFont.CreateFontIndirect(&lf);
  lf.lfWeight = FW_BOLD;
  if (lf.lfHeight > 0)
    lf.lfHeight += 22;//Make it a little bigger
  else
    lf.lfHeight -= 2;//Make it a little bigger
  
  lf.lfWidth = 0;
  BoldFont.CreateFontIndirect(&lf);
  
  compute_metrics(pDC);
  //Compute this again in case user changed printer
  int nMaxPage = m_nMaxRowCount/m_nRowsPerPage + 1;
  pInfo->SetMaxPage(nMaxPage);
  pInfo->m_nCurPage = 1;  // start printing at page# 1
  
  return;
}

//***********************ONPRINT*************************
void LCPrinting::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  if(NULL == pDC || NULL == pInfo)           
    return;
  
  //This has to be in OnPrint() or else PrintPreview goes screwy
  pOldFont = pDC->GetCurrentFont();
  
  //Fit all columns to 1 page, regardless of column number.
  pDC->SetMapMode(MM_ANISOTROPIC);
  
  //For every 1 List Control pixel
  pDC->SetWindowExt(1, 1);
  
  //The printer has ratio more dots
  pDC->SetViewportExt(m_ratiox, m_ratioy);
  
  int nStartRow = (pInfo->m_nCurPage - 1)*m_nRowsPerPage;
  int nEndRow = nStartRow+m_nRowsPerPage;
  if(nEndRow > m_nMaxRowCount)       
    nEndRow = m_nMaxRowCount;
  
  PrintHeader(pDC, pInfo);     //print the header
  
  PrintFooter(pDC, pInfo);	  //Print the footer

  pDC->SetWindowOrg(-1*page_rc.left, -1*HEADER_HEIGHT*m_nRowHeight);

  hc_items = m_pLC->GetColumnCount();                       // add this, from PrintHeaderControl

  PrintHeaderControl(pDC, pInfo);//Print the header Control, Manually

  pDC->SelectObject(&lcFont);//Use the LC normal font
  pDC->SetTextColor(RGB(0,0,0));//Black text on
  pDC->SetBkColor(RGB(255,255,255));//White paper
  
  CRect rcBounds;
  for (int i = 0; i < m_pLC->GetColumnCount(); i++)
    {
    CRect rect;
    m_pLC->GetCellRect(nStartRow, i, &rect);
    if (0 == i)
      {
      rcBounds.top = rect.top;
      rcBounds.bottom = rect.bottom;
      rcBounds.left = rect.left;
      }
    rcBounds.right = rect.right;
    }

  //offset top margin of rcBounds by ListControl header
  rcBounds.OffsetRect(0, -m_pLC->GetRowHeight(0));
  pDC->OffsetWindowOrg(rcBounds.left, rcBounds.top);
  
  //start printing rows
  for(i = nStartRow; i < nEndRow; i++)
    DrawRow(pDC, i, nEndRow - 1);
  
  //SetWindowOrg back for next page
  pDC->SetWindowOrg(0,0);
  pDC->SelectObject(pOldFont);//Put the old font back
  
  return;
}

//Set the extents after calling this function because it uses printer extents
//He is using a list in here have to figure out what to do.
//********************************PRINT_HEADER************************************
void LCPrinting::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
  pDC->SelectObject(&BoldFont);
  pDC->SetTextColor(RGB(0,0,0));//Black text on
  pDC->SetBkColor(RGB(255,255,255));//White paper
  
  CRect rc = page_rc;
  rc.bottom = rc.top+m_nRowHeight;
  
  //print App title on top right margin
  pDC->DrawText(TitleStr, &rc,  DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_RIGHT  | DT_NOCLIP);
  
  return;
}

//print footer with a line and date, and page number
//****************************PRINT_FOOTER****************************************
void LCPrinting::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
  CRect     rc = page_rc;
  rc.top = rc.bottom - FOOTER_HEIGHT*m_nRowHeight;
  rc.bottom = rc.top + m_nRowHeight;
  draw_line_at(pDC, rc.top);     //draw line
  
  //draw page number
  CString   sTemp ;
  rc.OffsetRect(0, m_nRowHeight/2);
  sTemp.Format("Page %d of %d", pInfo->m_nCurPage, pInfo->GetMaxPage());
  pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
  
  //Now draw the DateStr at bottom of page
  pDC->DrawText(DateStr,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
  
  return;
}

//Do the cleanup
//********************ONEND_PRINTING*****************
void LCPrinting::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
  lcFont.DeleteObject();
  BoldFont.DeleteObject();
  return;
}

//This function sets alls of the row and metric member vars
//************************COMPUTE_METRICS*********************
void LCPrinting::compute_metrics(CDC *pDC)
{
  //This includes width for all columns
  CRect row_rc(0, 0, 0, 0);
  for (int i = 0; i < m_pLC->GetColumnCount(); i++)
    {
    CRect rect;
    m_pLC->GetCellRect(0, i, &rect);
    if (0 == i)
      {
      row_rc.top = rect.top;
      row_rc.bottom = rect.bottom+1;
      row_rc.left = rect.left;
      }
    row_rc.right = rect.right+1;
    }

  //Get the list control window DC
  CDC  *pCtlDC = m_pLC->GetDC(); 
  if(NULL == pCtlDC) 
    return;
  
  //so we can get the avg character width
  TEXTMETRIC tm;   pCtlDC->GetTextMetrics(&tm);
  
  //Lets get the ratios for scaling to printer DC
  //Fit all columns to 1 page, regardless of column number.
  m_ratiox = pDC->GetDeviceCaps(HORZRES)/(row_rc.Width() + (LEFT_MARGIN+RIGHT_MARGIN)*tm.tmAveCharWidth);
  
  //width of pDC/whats got to fit into it in lcDC units
  m_ratioy = pDC->GetDeviceCaps(LOGPIXELSY)/pCtlDC->GetDeviceCaps(LOGPIXELSY);
  
  m_pLC->ReleaseDC(pCtlDC);
  
  //Set up a page rc in list control units that accounts for left and right margins
  page_rc.SetRect(0,0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
  page_rc.bottom = page_rc.bottom/m_ratioy;//Convert units to List Control
  page_rc.right = page_rc.right/m_ratiox;
  page_rc.left = LEFT_MARGIN*tm.tmAveCharWidth;//adjust sides for magins
  page_rc.right -= RIGHT_MARGIN*tm.tmAveCharWidth;
  
  m_nRowHeight = row_rc.Height();//Get the height of a row.
  int pRowHeight = (int)(m_nRowHeight*m_ratioy);//Get RowHeight in printer units.
  m_nRowsPerPage = pDC->GetDeviceCaps(VERTRES)/pRowHeight;//How many rows will fit on page?
  m_nRowsPerPage -= (HEADER_HEIGHT+FOOTER_HEIGHT);//After header and footer rows
  m_nRowsPerPage -= 1; //After header Control row
  
  return;
}

//You can't just have the header control print itself. 1st of all it looks crappy.
//2nd if part of header control is off screen does not print itself.
//So we will manually print it.
//************************PRINTHEADERCONTROL****************************
void LCPrinting::PrintHeaderControl(CDC *pDC, CPrintInfo *pInfo)
{
  UINT    dtFlags = DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER|DT_LEFT;//drawing flags
  
  CRect rc(0,0,0,m_nRowHeight);

  for (int j = 0; j < m_pLC->GetColumnCount(); j++)
    {
    CRect rect;
    m_pLC->GetCellRect(0, j, &rect);
    CString str = m_pLC->GetItemText(0, j);
    rc.right += rect.Width();
    pDC->DrawText(str, -1, rc, dtFlags);
    rc.left += rect.Width();
    }

 
  //Now draw the line below header control
  draw_line_at(pDC, rc.bottom);
  
  return;
}

//*************************************DRAWROW********************************************
void LCPrinting::DrawRow(CDC *pDC, int nItem, int nEnd)
{
  CString temp_str;

  CRect rc; m_pLC->GetCellRect(nItem, 0, &rc);
  int offset = pDC->GetTextExtent(" ", 1).cx;//Returns CSIZE so get cx member of CSIZE object.
  rc.left += offset/2;//This makes it so that label will be over a little bit
  rc.right -= offset;//Just keep this stuff it DOES look better.

  int iLeft;
  for (int i = 0; i < m_pLC->GetColumnCount(); i++)
    {
    CRect rect;
    m_pLC->GetCellRect(nItem, i, &rect);
    temp_str = m_pLC->GetItemText(nItem, i);
    rc.right += rect.Width();
    pDC->DrawText(temp_str, -1, rc, DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER|DT_LEFT);

    if (i == 0) 
      iLeft = rc.left;

    if (nItem < nEnd) //draw a line below each row
      {
      pDC->MoveTo(iLeft, rc.bottom);
      pDC->LineTo(iLeft + page_rc.right - page_rc.left - 1, rc.bottom);
      }

    // vertical lines, too
    if (i> 0)
      {
      pDC->MoveTo(rc.left - 5, rc.top);
      pDC->LineTo(rc.left - 5, rc.bottom);
      }
    rc.left += rect.Width();
    }
  return;
}

//Just pass this function a y position to draw the line at.
//*************************DRAW_LINE_AT************************************
void LCPrinting::draw_line_at(CDC *pDC, unsigned int y)
{
  pDC->MoveTo(0, y);
  pDC->LineTo(page_rc.right, y);//Use the page_rc to figure out the width of the line
  
  return;
}

