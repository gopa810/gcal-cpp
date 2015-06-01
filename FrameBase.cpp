// FrameBase.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "FrameBase.h"
#include "strings.h"
#include "vedic_ui.h"
#include "LocationRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameBase

int GetShowSetVal(int);
int AvcGetNextLine(TString &, TString &, int &);
int AvcGetTextLineCount(VAISNAVADAY * pvd);

int WriteCalendarXml(TResultCalendar &daybuff, FILE * fout);
int CalcCalendar(TResultCalendar &calendar, CLocationRef &loc, VCTIME date, int nDaysCount);
int FormatCalendarICAL(TResultCalendar &calendar, TString &str);
int FormatCalendarVCAL(TResultCalendar &calendar, TString &str);
int FormatCalendarOld(TResultCalendar &, TString &);

extern EARTHDATA gMyLocation;
extern VCTIME gTodayAct;
extern const char * gpszSeparator;

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

int AvcGetTextLineCount(VAISNAVADAY * pd);

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
//	bool bCalcMoon = (GetShowSetVal(4) > 0 || GetShowSetVal(5) > 0);
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
			nLineCount = AvcGetTextLineCount(pvd);
			
			if (GetShowSetVal(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						nLineCount++;
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						nLineCount++;
					}
				}
			}

			if ((yLine * nLineCount + yCurr) > rcPrint.bottom)
				bStart = true;

			if ((GetShowSetVal(18) == 1) && (pvd->astrodata.nMasa != lastmasa))
			{
				strHdr.Format("%s %s, Gaurabda %d", GetMasaName(pvd->astrodata.nMasa), gstr[22].c_str(), pvd->astrodata.nGaurabdaYear);
//				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
//					AddListText(m_text, gstr[128]);
				}
				lastmasa = pvd->astrodata.nMasa;
				bStart = true;
			}

			if ((GetShowSetVal(19) == 1) && (pvd->date.month != lastmonth))
			{
				strHdr.Format("%s %d", gstr[759 + pvd->date.month].c_str(), pvd->date.year);

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
				sz = dc.GetTextExtent(gstr[130].c_str(), gstr[130].GetLength());
				dc.TextOut(rcPrint.right - sz.cx, yCurr - sz.cy - 10, gstr[130].c_str(), gstr[130].GetLength());

				// page
				str.Format("%s %d", gstr[162].c_str(), nPage);
				nPage++;
				sz = dc.GetTextExtent(str);
				dc.TextOut(rcPrint.right - sz.cx, yCurr + 10, str);
				
				// line
				dc.MoveTo(rcPrint.left, yCurr - 4);
				dc.LineTo(rcPrint.right, yCurr - 4);
				
				
				yLine = sz.cy * 6 / 5;
				yCurr += 3 * sz.cy;

				str = gstr[157];
				dc.TextOut(rcPrint.left + nColumn[0] + 3*nColumn[6], yCurr, str);//DATE

				str = gstr[13];
				str.MakeUpper();
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);//TITHI

				str = gstr[163];
				sz = dc.GetTextExtent(str);
				dc.TextOut(rcPrint.left + nColumn[2] - sz.cx, yCurr, str);

				str = gstr[104];
				str.MakeUpper();
				dc.TextOut(rcPrint.left + nColumn[3], yCurr, str);

				str = gstr[15];
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

			str.Format("%d %s %d", pvd->date.day, AvcGetMonthAbr(pvd->date.month), pvd->date.year);
			sz = dc.GetTextExtent(str);
			dc.TextOut(rcPrint.left + nColumn[0] + 11*nColumn[6] - nColumn[6]/2 - sz.cx, yCurr, str);

			dc.TextOut(rcPrint.left + nColumn[0] + 11*nColumn[6] + nColumn[6]/2
				, yCurr, LPCTSTR(gstr[pvd->date.dayOfWeek]), 2);

			str = GetTithiName(pvd->astrodata.nTithi);

			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) || (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					if (pvd->nMhdType == EV_NULL)
					{
						str += " ";
						str += gstr[58];
					}
					else
					{
						str += " ";
						str += gstr[59];
					}
				}
			}
			dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);

			dc.TextOut(rcPrint.left + nColumn[2] - nColumn[6], yCurr, GetPaksaName(pvd->astrodata.nPaksa), 1);

			str = GetYogaName(pvd->astrodata.nYoga);
			dc.TextOut(rcPrint.left + nColumn[3], yCurr, str);

			str = GetNaksatraName(pvd->astrodata.nNaksatra);
			dc.TextOut(rcPrint.left + nColumn[4], yCurr, str);

			if (pvd->nFastType >= EV_NULL)
				dc.TextOut(rcPrint.left + nColumn[5], yCurr, "*", 1);

			yCurr += yLine;

			if (GetShowSetVal(17) == 1)
			{
				if (pvd->ekadasi_parana)
				{
					pvd->GetTextEP(strt);
					str = strt.c_str();
					str.Delete(0, 17);
					dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
					yCurr += yLine;
				}
			}

			if (GetShowSetVal(6) == 1)
			{
				if (pvd->festivals.GetLength() > 0)
				{
					int i = pvd->GetHeadFestival();
					int nFestClass;
					TString str2;
					while(pvd->GetNextFestival(i, str2))
					{
						if (str2.GetLength() > 1)
						{
							nFestClass = pvd->GetFestivalClass(str2);
							str.Format("%s<br>\n", str2.c_str());
							if (nFestClass < 0 || GetShowSetVal(22 + nFestClass) == 1)
							{
								const char * cstr2 = str2.c_str();
								dc.TextOut(rcPrint.left + nColumn[1], yCurr, cstr2, strlen(cstr2));
								yCurr += yLine;
							}
						}
					}
				}
				/*if (pvd->festivals)
				{
					int i = pvd->GetHeadFestival();
					while(pvd->GetNextFestival(i, strt))
					{
						str2 = strt.c_str();
						if (str2.GetLength() > 1)
						{
							nFestClass = pvd->GetFestivalClass(strt2);
							str2 = strt2.c_str();
							if (nFestClass < 0 || GetShowSetVal(22 + nFestClass) == 1)
							{
								dc.TextOut(rcPrint.left + nColumn[1], yCurr, str2);
								yCurr += yLine;
							}
						}
					}
				}*/
			}

			if (GetShowSetVal(16) == 1 && pvd->sankranti_zodiac >= 0)
			{
//				double h1, m1;
//				m1 = modf(pvd->sankranti_day.shour*24, &h1);
				str.Format(" %s %s (%s %s %s %d %s, %02d:%02d %s) "
					, GetSankrantiName(pvd->sankranti_zodiac)
					, gstr[56].c_str()
					, gstr[111].c_str(), GetSankrantiNameEn(pvd->sankranti_zodiac)
					, gstr[852].c_str()
					, pvd->sankranti_day.day, AvcGetMonthAbr(pvd->sankranti_day.month)
					, pvd->sankranti_day.GetHour(), pvd->sankranti_day.GetMinuteRound()
					, AvcGetDSTSignature(pvd->nDST));
	
				sz = dc.GetTextExtent(str);
				nWidth = sz.cx / 2;

				dc.TextOut(midPoint.x - nWidth, yCurr, str);
				dc.MoveTo(rcPrint.left, yCurr + sz.cy / 2);
				dc.LineTo(midPoint.x - nWidth, yCurr + sz.cy / 2);
				dc.MoveTo(midPoint.x + nWidth, yCurr + sz.cy / 2);
				dc.LineTo(rcPrint.right, yCurr + sz.cy / 2);
				yCurr += yLine;
				/*dayText += (gpszSeparator + (seplen - length));
				dayText += str;
				dayText += (gpszSeparator + (seplen - length));
				dayText += "\r\n";*/
			}

			if (GetShowSetVal(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
			{
				double h, m;
				VCTIME ksayaDate;

				// zaciatok ksaya tithi
				m = modf(pvd->ksaya_time1*24, &h);
				ksayaDate = pvd->date;
				if (pvd->ksaya_day1 < 0.0)
					ksayaDate.PreviousDay();
				str2.Format("%d %s, %02d:%02d", ksayaDate.day, AvcGetMonthAbr(ksayaDate.month), int(h), int(m*60));

				// end of ksaya tithi
				m = modf(pvd->ksaya_time2*24, &h);
				ksayaDate = pvd->date;
				if (pvd->ksaya_day2 < 0.0)
					ksayaDate.PreviousDay();
				str3.Format("%d %s, %02d:%02d", ksayaDate.day, AvcGetMonthAbr(ksayaDate.month), int(h), int(m*60));

				// print info
				str.Format("%s %s %s %s %s (%s)", gstr[89].c_str(), gstr[850].c_str(), str2, gstr[851].c_str(), str3, AvcGetDSTSignature(pvd->nDST));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}

			if (GetShowSetVal(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (pvd->is_vriddhi)
				{
					str = gstr[90];
					dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
					yCurr += yLine;
				}
			}

			if (pvd->nCaturmasya & CMASYA_CONT_MASK)
			{
				int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
				str = gstr[111+n];
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
//				AddListText(dayText, gstr[ 111 + n ]);
			}

			if ((GetShowSetVal(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				str.Format("%s [PURNIMA SYSTEM]"
					, gstr[107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					           + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str()
					);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					str = gstr[110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str();
					dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
					yCurr += yLine;
				}
			}

			if ((GetShowSetVal(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				str.Format("%s [PRATIPAT SYSTEM]"
					, gstr[107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					           + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str()
					);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					str = gstr[110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str();
					dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
					yCurr += yLine;
				}
			}

			if ((GetShowSetVal(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				str.Format("%s [EKADASI SYSTEM]"
					, gstr[107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					           + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str()
					);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					str = gstr[110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str();
					dc.TextOut(rcPrint.left  + nColumn[1], yCurr, str);
					yCurr += yLine;
				}
			}

			// tithi at arunodaya
			if (GetShowSetVal(0) == 1)//m_dshow.m_tithi_arun)
			{
				str.Format("%s: %s", gstr[98].c_str(), GetTithiName(pvd->astrodata.nTithiArunodaya));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}

			//"Arunodaya Time",//1
			if (GetShowSetVal(1) == 1)//m_dshow.m_arunodaya)
			{
				str.Format("%s %d:%02d (%s)", gstr[99].c_str(), pvd->astrodata.sun.arunodaya.hour
					, pvd->astrodata.sun.arunodaya.min, AvcGetDSTSignature(pvd->nDST));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			//"Sunrise Time",//2
			//"Sunset Time",//3
			if (GetShowSetVal(2) == 1)//m_dshow.m_sunrise)
			{
				str.Format("%s %d:%02d (%s)", gstr[51].c_str(), pvd->astrodata.sun.rise.hour
					, pvd->astrodata.sun.rise.min, AvcGetDSTSignature(pvd->nDST));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			if (GetShowSetVal(3) == 1)//m_dshow.m_sunset)
			{
				str.Format("%s %d:%02d (%s)", gstr[52].c_str(), pvd->astrodata.sun.set.hour
					, pvd->astrodata.sun.set.min, AvcGetDSTSignature(pvd->nDST));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;

			}
			//"Moonrise Time",//4
			if (GetShowSetVal(4) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = gstr[136];
				else
				{
					str.Format("%s %d:%02d (%s)", gstr[53].c_str(), pvd->moonrise.hour
						, pvd->moonrise.min, AvcGetDSTSignature(pvd->nDST));
				}
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			//"Moonset Time",//5
			if (GetShowSetVal(5) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = gstr[137];
				else
				{
					str.Format("%s %d:%02d (%s)", gstr[54].c_str(), pvd->moonset.hour
						, pvd->moonset.min, AvcGetDSTSignature(pvd->nDST));
				}
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			///"Sun Longitude",//9
			if (GetShowSetVal(9) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f (*)", gstr[100].c_str(), pvd->astrodata.sun.longitude_deg);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			//"Moon Longitude",//10
			if (GetShowSetVal(10) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f (*)", gstr[101].c_str(), pvd->astrodata.moon.longitude_deg);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			//"Ayanamsha value",//11
			if (GetShowSetVal(11) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (%s) (*)", gstr[102].c_str(), pvd->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}
			//"Julian Day",//12
			if (GetShowSetVal(12) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (*)", gstr[103].c_str(), pvd->astrodata.jdate);
				dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
				yCurr += yLine;
			}

			if (GetShowSetVal(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", gstr[780].c_str(), GetMasaName(pvd->astrodata.nMasa), gstr[22].c_str());
						dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
						yCurr += yLine;
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", gstr[781].c_str(), GetMasaName(pvd->astrodata.nMasa), gstr[22].c_str());
						dc.TextOut(rcPrint.left + nColumn[1], yCurr, str);
						yCurr += yLine;
					}
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
	nPos = 0;
	while(AvcGetNextLine(strText, strLine, nPos))
	{
		str = strLine.c_str();
		sz = pdc.GetTextExtent(str);
		if (sz.cx > nMaxWidth)
			nMaxWidth = sz.cx;
	}

	xOffset = (rcPrint.Width() - nMaxWidth) / 2;
	nPos = 0;

	while(AvcGetNextLine(strText, strLine, nPos))
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
			str = gstr[130].c_str();
			sz = pdc.GetTextExtent(str);
			pdc.TextOut(rcPrint.right - sz.cx, yCurr - sz.cy - 5, str);

			// cislo strany
			strPage.Format("%s %d", gstr[162].c_str(), nPage);
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
