// FrameBase.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "FrameBase.h"

#include "LocationRef.h"
#include "enums.h"
#include "GCStrings.h"
#include "GCAyanamsha.h"
#include "GCDisplaySettings.h"
#include "GCGlobal.h"
#include "GCTextParser.h"

/////////////////////////////////////////////////////////////////////////////
// CFrameBase

//IMPLEMENT_DYNCREATE(CFrameBase, CFrameWnd)

CFrameBase::CFrameBase()
{
	m_rectPrintMargins.SetRect(15,15,15,15);
	m_bClosed = FALSE;
}

CFrameBase::~CFrameBase()
{
}


BEGIN_MESSAGE_MAP(CFrameBase, CFrameWnd)
	ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CFrameBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameBase message handlers

void CFrameBase::InitBaseAttributes()
{
	m_fontList.CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0, "Lucida Console");
	m_fontInfo.CreateFont(32,0,0,0,0,0,0,0,0,0,0,0,0, "Tahoma");

}

int CFrameBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitBaseAttributes();
	
	return 0;
}

int CFrameBase::AfxPrintCalendarText(CDC &dc, TResultCalendar &daybuff, CLocationRef & earth, VCTIME date
									 , int nDaysCount, CRect & rcPrint)
{
	int k;
	CString str, str2, str3, dayText;

//	char * spaces = "                                                                                ";
	VAISNAVADAY * pvd, * prevd, *nextd;
	int lastmasa = -1;
	int lastmonth = -1;
	//int nFestClass;
	bool bStart = true;
	bool bStarted = false;
//	double rate;
//	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);
	CPoint midPoint;
//	CRect rcPrint;
	int yCurr = rcPrint.top;
	int nWidth;
	int yLine = 0;
	CString strLine, strPage, strHdr;
	TString strt, strt2;
	int nPage = 0, nLineCount = 0;
	int nColumn[7];
//	int xPaper, yPaper;

//	rcPrint.SetRect(0, 0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
//	xPaper = dc.GetDeviceCaps(HORZSIZE);
//	yPaper = dc.GetDeviceCaps(VERTSIZE);
	yCurr = rcPrint.top;
	midPoint = rcPrint.CenterPoint();
	CSize sz;

	double xRes = dc.GetDeviceCaps(LOGPIXELSX);
	nColumn[0] = 0;
	nColumn[1] = int(xRes * 1.5);
	nColumn[2] = int(xRes * 4.85);
	nColumn[3] = int(xRes * 5);
	nColumn[4] = int(xRes * 6);
	nColumn[5] = int(xRes * 7);

	sz = dc.GetTextExtent("M", 1);
	nColumn[6] = sz.cx;

	for (k = 0; k < nDaysCount; k++)
	{
		date.shour = 0.0;
		date.tzone = earth.m_fTimezone;

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (pvd)
		{
			// zistuje kolko riadkov zaberie nasledujuci den
			nLineCount = pvd->GetTextLineCount();
			
			if ((yLine * nLineCount + yCurr) > rcPrint.bottom)
				bStart = true;

			if ((GCDisplaySettings::getValue(18) == 1) && (pvd->astrodata.nMasa != lastmasa))
			{
				strHdr.Format("%s %s, Gaurabda %d", GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str(), pvd->astrodata.nGaurabdaYear);
//				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
//					GCStringBuilder::AppendTwoColumnText(m_text, GCStrings::getString(128]);
				}
				lastmasa = pvd->astrodata.nMasa;
				bStart = true;
			}

			if ((GCDisplaySettings::getValue(19) == 1) && (pvd->date.month != lastmonth))
			{
				strHdr.Format("%s %d", GCStrings::getString(759 + pvd->date.month).c_str(), pvd->date.year);

				//m_text += " DATE            TITHI                         PAKSA YOGA      NAKSATRA       FAST\r\n";
				lastmonth = pvd->date.month;
				bStart = true;
			}

			if (bStart)
			{
				if (bStarted == true)
					dc.EndPage();

				dc.StartPage();
				yCurr = rcPrint.top;
				bStarted = true;

				dc.SelectObject(&m_fontPrintHdr);
				// tlaci hlavicku
				sz = dc.GetTextExtent(strHdr);
				dc.TextOut(midPoint.x - sz.cx / 2, yCurr, strHdr);
				yCurr += sz.cy + 8;

				dc.SelectObject(&m_fontPrintText);

				// location
				str = this->earth.m_strName.c_str();
				sz = dc.GetTextExtent(str);
				dc.TextOut(midPoint.x - sz.cx / 2, yCurr + 10, str);

				// version
				sz = dc.GetTextExtent(GCStrings::getString(130).c_str(), GCStrings::getString(130).GetLength());
				dc.TextOut(rcPrint.right - sz.cx, yCurr - sz.cy - 10, GCStrings::getString(130).c_str(), GCStrings::getString(130).GetLength());

				// page
				str.Format("%s %d", GCStrings::getString(162).c_str(), nPage);
				nPage++;
				sz = dc.GetTextExtent(str);
				dc.TextOut(rcPrint.right - sz.cx, yCurr + 10, str);
				
				// line
				dc.MoveTo(rcPrint.left, yCurr - 4);
				dc.LineTo(rcPrint.right, yCurr - 4);
				
				
				yLine = sz.cy * 6 / 5;
				yCurr += 3 * sz.cy;

				str = GCStrings::getString(157);
				dc.TextOut(rcPrint.left + nColumn[0] + 3*nColumn[6], yCurr, str);//DATE

				str = GCStrings::getString(13);
				str.MakeUpper();
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);//TITHI

				str = GCStrings::getString(163);
				sz = dc.GetTextExtent(str);
				dc.TextOut(rcPrint.left + nColumn[2] - sz.cx, yCurr, str);

				str = GCStrings::getString(104);
				str.MakeUpper();
				dc.TextOut(rcPrint.left + nColumn[3], yCurr, str);

				str = GCStrings::getString(15);
				str.MakeUpper();
				dc.TextOut(rcPrint.left + nColumn[4], yCurr, str);

				dc.TextOut(rcPrint.left + nColumn[5] - 2*nColumn[6], yCurr, "FAST", 4);
				
				yCurr += yLine + 8;
				dc.MoveTo(rcPrint.left, yCurr);
				dc.LineTo(rcPrint.right, yCurr);

				yCurr += 8;
				bStart = false;
			}

			if (pvd->astrodata.sun.longitude_deg < 0.0)
			{
				continue;
			}

			str.Format("%d %s %d", pvd->date.day, GCStrings::GetMonthAbreviation(pvd->date.month), pvd->date.year);
			sz = dc.GetTextExtent(str);
			dc.TextOut(rcPrint.left + nColumn[0] + 11*nColumn[6] - nColumn[6]/2 - sz.cx, yCurr, str);

			dc.TextOut(rcPrint.left + nColumn[0] + 11*nColumn[6] + nColumn[6]/2
				, yCurr, LPCTSTR(GCStrings::getString(pvd->date.dayOfWeek)), 2);

			str = pvd->GetFullTithiName().c_str();
			dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);

			dc.TextOut(rcPrint.left + nColumn[2] - nColumn[6], yCurr, GCStrings::GetPaksaName(pvd->astrodata.nPaksa), 1);

			str = GCStrings::GetYogaName(pvd->astrodata.nYoga);
			dc.TextOut(rcPrint.left + nColumn[3], yCurr, str);

			str = GCStrings::GetNaksatraName(pvd->astrodata.nNaksatra);
			dc.TextOut(rcPrint.left + nColumn[4], yCurr, str);

			if (pvd->nFastType >= EV_NULL)
				dc.TextOut(rcPrint.left + nColumn[5], yCurr, "*", 1);

			yCurr += yLine;

			for(int i = 0; i < pvd->dayEvents.Count(); i++)
			{
				GCMutableDictionary * ed = pvd->dayEvents.ObjectAtIndex(i);
				int disp = ed->intForKey("disp");
				str = ed->stringForKey("text");
				if (!ed->containsKey("disp") || GCDisplaySettings::getValue(disp))
				{
					if (ed->containsKey("spec"))
					{
						sz = dc.GetTextExtent(str);
						nWidth = sz.cx / 2;

						dc.TextOut(midPoint.x - nWidth, yCurr, str);
						dc.MoveTo(rcPrint.left, yCurr + sz.cy / 2);
						dc.LineTo(midPoint.x - nWidth, yCurr + sz.cy / 2);
						dc.MoveTo(midPoint.x + nWidth, yCurr + sz.cy / 2);
						dc.LineTo(rcPrint.right, yCurr + sz.cy / 2);
					}
					else
					{
						dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
					}
					yCurr += yLine;
				}
			}

		}
		date.shour = 0;
		date.NextDay();
	}

	dc.EndPage();

	return 1;
}

int CFrameBase::PrintBlockText(CDC &pdc, TString &strTitle, TString &strText, int &nPage, CRect & rcPrint)
{
	CPoint midPoint;
//	CRect rcPrint;
	int yCurr = rcPrint.top;
	int xOffset = 0;
	int nPos = 0, nPrevPos = 0;
	int nMaxWidth = 0;
	TString strLine, strPage;
	BOOL bStart = TRUE;
	CString str;

//	rcPrint.SetRect(0, 0, pdc.GetDeviceCaps(HORZRES), pdc.GetDeviceCaps(VERTRES));
	yCurr = rcPrint.top;
	midPoint = rcPrint.CenterPoint();
	CSize sz;

	// location text
//	m_viewCond.m_edits[0].GetWindowText(strLocation);

	// measure dimension of text
	pdc.SelectObject(&m_fontPrintText);
	str = strText.c_str();
	sz = pdc.GetTextExtent(str);
	GCTextParser parser;
	parser.SetTarget(&strText);
	while(parser.GetNextLine(strLine))
	{
		str = strLine.c_str();
		sz = pdc.GetTextExtent(str);
		if (sz.cx > nMaxWidth)
			nMaxWidth = sz.cx;
	}

	xOffset = (rcPrint.Width() - nMaxWidth) / 2;
	parser.SetTarget(&strText);
	while(parser.GetNextLine(strLine))
	{
		if (strLine.IsEmpty())
			strLine = " ";
		if (bStart)
		{
			pdc.StartPage();

			pdc.SelectObject(&m_fontPrintHdr);

			// titul
			str = strTitle.c_str();
			sz = pdc.GetTextExtent(str);
			pdc.TextOut( midPoint.x - sz.cx / 2, yCurr, str);
			yCurr += sz.cy;

			pdc.SelectObject(&m_fontPrintText);

			// pozn o programe verzii
			str = GCStrings::getString(130).c_str();
			sz = pdc.GetTextExtent(str);
			pdc.TextOut(rcPrint.right - sz.cx, yCurr - sz.cy - 5, str);

			// cislo strany
			strPage.Format("%s %d", GCStrings::getString(162).c_str(), nPage);
			nPage++;
			str = strPage.c_str();
			sz = pdc.GetTextExtent(str);
			pdc.TextOut(rcPrint.right - sz.cx, yCurr + 10, str);

			// ciara
			pdc.MoveTo( rcPrint.left, yCurr);
			pdc.LineTo( rcPrint.right, yCurr);
			yCurr += 10;

			// lokacia
			str = this->earth.m_strName.c_str();
			sz = pdc.GetTextExtent(str);
			pdc.TextOut( midPoint.x - sz.cx / 2, yCurr, str);
			yCurr += 3 * sz.cy;

			bStart = FALSE;
		}

		str = strLine.c_str();
		sz = pdc.GetTextExtent(str);

		if (sz.cy + yCurr > rcPrint.bottom)
		{
			nPos = nPrevPos;
			bStart = TRUE;
			pdc.EndPage();
			yCurr = rcPrint.top;
		}
		else
		{
			pdc.TextOut(xOffset, yCurr, str);
			yCurr += sz.cy;
		}

		nPrevPos = nPos;
	}

	pdc.EndPage();

	return 0;
}

void CFrameBase::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	m_bClosed = TRUE;	
}

void CFrameBase::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	GetWindowRect(m_rectWindow);
}

void CFrameBase::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);

	GetWindowRect(m_rectWindow);
}

void CFrameBase::OnInitMenu(CMenu* pMenu)
{
   CFrameWnd::OnInitMenu(pMenu);
  
	// CG: This block added by 'Tip of the Day' component.
	{
		// TODO: This code adds the "Tip of the Day" menu item
		// on the fly.  It may be removed after adding the menu
		// item to all applicable menu items using the resource
		// editor.

		// Add Tip of the Day menu item on the fly!
		static CMenu* pSubMenu = NULL;

		CString strHelp; strHelp.LoadString(CG_IDS_TIPOFTHEDAYHELP);
		CString strMenu;
		int nMenuCount = pMenu->GetMenuItemCount();
		BOOL bFound = FALSE;
		for (int i=0; i < nMenuCount; i++) 
		{
			pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
			if (strMenu == strHelp)
			{ 
				pSubMenu = pMenu->GetSubMenu(i);
				bFound = TRUE;
				ASSERT(pSubMenu != NULL);
			}
		}

		CString strTipMenu;
		strTipMenu.LoadString(CG_IDS_TIPOFTHEDAYMENU);
		if (!bFound)
		{
			// Help menu is not available. Please add it!
			if (pSubMenu == NULL) 
			{
				// The same pop-up menu is shared between mainfrm and frame 
				// with the doc.
				static CMenu popUpMenu;
				pSubMenu = &popUpMenu;
				pSubMenu->CreatePopupMenu();
				pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 
					CG_IDS_TIPOFTHEDAY, strTipMenu);
			} 
			pMenu->AppendMenu(MF_STRING|MF_BYPOSITION|MF_ENABLED|MF_POPUP, 
				(UINT)pSubMenu->m_hMenu, strHelp);
			DrawMenuBar();
		} 
		else
		{      
			// Check to see if the Tip of the Day menu has already been added.
			pSubMenu->GetMenuString(0, strMenu, MF_BYPOSITION);

			if (strMenu != strTipMenu) 
			{
				// Tip of the Day submenu has not been added to the 
				// first position, so add it.
				pSubMenu->InsertMenu(0, MF_BYPOSITION);  // Separator
				pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 
					CG_IDS_TIPOFTHEDAY, strTipMenu);
			}
		}
	}

}
