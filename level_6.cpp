#include "stdafx.h"
#include "platform.h"
#include "level_6.h"
#include "vedic_ui.h"
#include "strings.h"
#include "enums.h"
#include "resource.h"
#include "TTimeZone.h"
#include "TResultApp.h"
#include "TResultMasaList.h"
#include "DlgCalcProgress.h"
// PORTABLE 
#include "GCStrings.h"
#include "GCRasi.h"
#include "GCTithi.h"
#include "GCNaksatra.h"
#include "GCAyanamsha.h"
#include "GCSankranti.h"
#include "GCConjunction.h"
#include "GCCalendar.h"
#include "GCSunData.h"
#include "GCDisplaySettings.h"
#include "GCMoonData.h"
#include "GCStringBuilder.h"
#include "GCLayoutData.h"

extern TString gAyaName;
extern const char * gpszSeparator;
void time_print(TString &str, DAYTIME dt);
void AddTextLine(TString &, const char *);
void AddTextLineRtf(TString &str, const char * pt);
void AddNoteRtf(TString &);

int g_ShowMode = 1; // 0=text,1=rtf

void AddNoteText(TString &str)
{
	GCStringBuilder sb;
	sb.Target = &str;
	sb.Format = SBTF_TEXT;
	sb.AppendNote();
}

void time_print(TString &str, DAYTIME dt)
{
	str.Format("%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
}

void AddNoteRtf(TString &str)
{
	GCStringBuilder sb;
	sb.Target = &str;
	sb.Format = SBTF_RTF;
	sb.AppendNote();
}

void AddListText(TString &text, const char *pText)
{
	TString str;
	str.Format("                 %s\r\n", pText);
	str.TrimRight();
	str+= "\r\n";
	text += str;
}

void AddListRtf(TString &text, const char *pText)
{
	text += "\\par\\tab ";
	text += pText;
}

void AddListText(TString &text, const char *pText, const char * pText2)
{
	TString str;
	str.Format("%-17s%s", pText, pText2);
	str.TrimRight();
	str+= "\r\n";
	text += str;
}

void AddListRtf(TString &text, const char *pText, const char * pText2)
{
	TString str;
	str.Format("\\par %s\\tab %s", pText, pText2);
	text += str;
}

DlgCalcProgress dcp;

int AvcProgressWindowCreate()
{
	dcp.Create(IDD_CALC_PROGRESS, AfxGetMainWnd());
	dcp.CenterWindow();
	dcp.ShowWindow(SW_SHOW);
	dcp.UpdateWindow();

	return 0;
}

int AvcProgressWindowSetRange(int nMin, int nMax)
{
	dcp.m_p1.SetRange32(nMin, nMax);

	return 0;
}

int AvcProgressWindowSetPos(int nPos)
{
	dcp.m_p1.SetPos(nPos);

	return 0;
}

int AvcProgressWindowClose()
{
	dcp.DestroyWindow();

	return 0;
}

int CalcCalendar(TResultCalendar &daybuff, CLocationRef & loc, VCTIME date, int nDaysCount) 
{
	TString str, str2, str3, dayText;

	char * spaces = "                                                                                ";
	int lastmasa = -1;
	int lastmonth = -1;
	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);

	DlgCalcProgress dcp;

	dcp.Create(IDD_CALC_PROGRESS, AfxGetMainWnd());
	dcp.CenterWindow();
	dcp.ShowWindow(SW_SHOW);
	dcp.UpdateWindow();

	daybuff.m_pProgress = &(dcp.m_p1);

	if (daybuff.CalculateCalendar(loc, date, nDaysCount) == 0)
		return 0;

	daybuff.m_pProgress = NULL;

//	AvcGetOldCalendarText(daybuff, m_text);


/*	for (k = 0; k < nDaysCount; k++)
	{
		date.shour = 0.0;
		date.TimeZone = earth.tzone;

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (bCalcMoon)
		{
			dcp.m_p1.SetPos(int(90.8 + 9.2 * k / nDaysCount));
		}
		else
		{
			rate = double(k) / nDaysCount;
			dcp.m_p1.SetPos(int(58.8 + 41.2 * rate * rate));
		}

		if (pvd)
		{
			if ((GCDisplaySettings::getValue(18) == 1) && (pvd->astrodata.nMasa != lastmasa))
			{
				m_text += "\r\n";
				str.Format("%s %s, Gaurabda %d", GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22], pvd->astrodata.nGaurabdaYear);
				tp1 = (80 - str.GetLength())/2;
				str.Insert(0, (spaces + (79 - tp1)));
				str += spaces;
				str.Insert(80 - _tcslen(AvcGetVersionText()), AvcGetVersionText());
				m_text += str.Left(80);
				m_text += "\r\n";
				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
					AddListText(m_text, GCStrings::getString(128]);
				}
				m_text += "\r\n";
				m_text += (spaces + (79 - (80 - location_info.GetLength())/2));
				m_text += location_info;

				m_text += "\r\n\r\n";
				m_text += " DATE            TITHI                         PAKSA YOGA      NAKSATRA       FAST\r\n";
				m_text += gpszSeparator;
				m_text += "\r\n";
				lastmasa = pvd->astrodata.nMasa;
			}

			if ((GCDisplaySettings::getValue(19) == 1) && (pvd->date.month != lastmonth))
			{
				m_text += "\r\n";
				str.Format("%s %d", GCStrings::getString(759 + pvd->date.month], pvd->date.year);
				tp1 = (80 - str.GetLength())/2;
				str.Insert(0, (spaces + (79 - tp1)));
				str += spaces;
				str.Insert(80 - _tcslen(AvcGetVersionText()), AvcGetVersionText());
				m_text += str.Left(80);
				m_text += "\r\n";
				m_text += (spaces + (79 - (80 - location_info.GetLength())/2));
				m_text += location_info;

				m_text += "\r\n\r\n";
				m_text += " DATE            TITHI                         PAKSA YOGA      NAKSATRA       FAST\r\n";
				m_text += gpszSeparator;
				m_text += "\r\n";
				lastmonth = pvd->date.month;
			}

			dayText.Empty();
			pvd->GetTextA(str);
			str2 = str.Mid(16);
			str = str.Left(15);
			if (pvd->astrodata.sun.longitude_deg < 0.0)
			{
				AddListText(dayText, str, "No rise and no set of the sun. No calendar information.");
				goto _resolve_text;
			}
			AddListText(dayText, str, str2);

			if (GCDisplaySettings::getValue(17) == 1)
			{
				if (pvd->ekadasi_parana)
				{
					pvd->GetTextEP(str);
					str.Delete(0, 17);
					AddListText(dayText, str);
				}
			}

			if (GCDisplaySettings::getValue(6) == 1)
			{
				if (pvd->festivals)
				{
					int i = pvd->GetHeadFestival();
					while(pvd->GetNextFestival(i, str2))
					{
						if (str2.GetLength() > 1)
							AddListText(dayText, str2);
					}
				}
			}

			if (GCDisplaySettings::getValue(16) == 1 && pvd->sankranti_zodiac >= 0)
			{
				double h1, m1;
				m1 = modf(pvd->sankranti_day.shour*24, &h1);
				str.Format(" %s %s (%s %s %s %d %s, %02d:%02d %s) "
					, GetSankrantiName(pvd->sankranti_zodiac)
					, GCStrings::getString(56]
					, GCStrings::getString(111], GetSankrantiNameEn(pvd->sankranti_zodiac)
					, GCStrings::getString(852]
					, pvd->sankranti_day.day, GCStrings::GetMonthAbreviation(pvd->sankranti_day.month)
					, int(h1), int(m1*60)
					, GCStrings::GetDSTSignature(pvd->nDST));
				int length = str.GetLength();
				int seplen = _tcslen(gpszSeparator);
				length = (seplen - length) / 2;
				dayText += (gpszSeparator + (seplen - length));
				dayText += str;
				dayText += (gpszSeparator + (seplen - length));
				dayText += "\r\n";
			}

			if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
			{
				double h, m;
				VAISNAVADAY * p;


				if (pvd->ksaya_time1 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time1)*24, &h);
				else
					m = modf(pvd->ksaya_time1*24, &h);
				p = (pvd->ksaya_time1 < 0.0) ? prevd : pvd;
				str2.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				if (pvd->ksaya_time2 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
				else
					m = modf(pvd->ksaya_time2*24, &h);
				p = (pvd->ksaya_time2 < 0.0) ? prevd : pvd;
				str3.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				str.Format("%s %s %s %s %s (%s)", GCStrings::getString(89], GCStrings::getString(850], str2, GCStrings::getString(851], str3, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}

			if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (pvd->is_vriddhi)
					AddListText(dayText, GCStrings::getString(90]);
			}

			if (pvd->nCaturmasya & CMASYA_CONT_MASK)
			{
				int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
				AddListText(dayText, GCStrings::getString( 111 + n ]);
			}

			if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				str.Format("%s [PURNIMA SYSTEM]"
					, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					           + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)]
					);
				AddListText(dayText, str);
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)] );
				}
			}

			if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				str.Format("%s [PRATIPAT SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					           + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)]
					);
				AddListText(dayText, str);
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)] );
				}
			}

			if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				str.Format("%s [EKADASI SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					           + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)]
					);
				AddListText(dayText, str);
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)] );
				}
			}

			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str.Format("%s: %s", GCStrings::getString(98], GetTithiName(pvd->astrodata.nTithiArunodaya));
				AddListText(dayText, str);
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(99], pvd->astrodata.sun.arunodaya.hour
					, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}
			//"Sunrise Time",//2
			//"Sunset Time",//3
			if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(51], pvd->astrodata.sun.rise.hour
					, pvd->astrodata.sun.rise.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}
			if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(52], pvd->astrodata.sun.set.hour
					, pvd->astrodata.sun.set.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);

			}
			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(136];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(53], pvd->moonrise.hour
						, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(137];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(54], pvd->moonset.hour
						, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(100], pvd->astrodata.sun.longitude_deg);
				AddListText(dayText, str);
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(101], pvd->astrodata.moon.longitude_deg);
				AddListText(dayText, str);
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (%s)", GCStrings::getString(102], pvd->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
				AddListText(dayText, str);
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f", GCStrings::getString(103], pvd->astrodata.jdate);
				AddListText(dayText, str);
			}

			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780], GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22]);
						AddListText(dayText, str);
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781], GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22]);
						AddListText(dayText, str);
					}
				}
			}

		_resolve_text:
			if (GCDisplaySettings::getValue(20) == 0)
				m_text += dayText;
			else if (dayText.GetLength() > 90)
				m_text += dayText;


		}
		date.shour = 0;
		date.NextDay();
	}
*/
	dcp.DestroyWindow();

	return 1;
}

int AvcGetOldCalendarDayText(VAISNAVADAY * pvd, TString &dayText) 
{
	TString str, str2, str3;
	int nFestClass;

		dayText.Empty();
		/* BEGIN GCAL 1.4.3 */
		pvd->GetTextA(str, GCDisplaySettings::getValue(39), GCDisplaySettings::getValue(36), GCDisplaySettings::getValue(37), GCDisplaySettings::getValue(38), GCDisplaySettings::getValue(41));
		/* END GCAL 1.4.3 */
		str.Mid(16, str.GetLength(), str2);
		str.Left(15, str3);
		str = str3;
		if (pvd->astrodata.sun.longitude_deg < 0.0)
		{
			AddListText(dayText, str, "No rise and no set of the sun. No calendar information.");
			return 1;
		}
		AddListText(dayText, str, str2);

		if (GCDisplaySettings::getValue(17) == 1)
		{
			if (pvd->ekadasi_parana)
			{
				pvd->GetTextEP(str);
				str.Delete(0, 17);
				AddListText(dayText, str);
			}
		}

		if (GCDisplaySettings::getValue(6) == 1)
		{
			if (pvd->festivals.GetLength() > 0)
			{
				int i = pvd->GetHeadFestival();
				while(pvd->GetNextFestival(i, str2))
				{
					if (str2.GetLength() > 1)
					{
						nFestClass = pvd->GetFestivalClass(str2);
						if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
							AddListText(dayText, str2);
					}
				}
			}
		}

		if (GCDisplaySettings::getValue(16) == 1 && pvd->sankranti_zodiac >= 0)
		{
			//double h1, m1;
			//m1 = modf(pvd->sankranti_day.shour*24, &h1);
			str.Format(" %s %s (%s %s %s %d %s, %02d:%02d %s) "
				, GCStrings::GetSankrantiName(pvd->sankranti_zodiac)
				, GCStrings::getString(56).c_str()
				, GCStrings::getString(111).c_str(), GCStrings::GetSankrantiNameEn(pvd->sankranti_zodiac)
				, GCStrings::getString(852).c_str()
				, pvd->sankranti_day.day, GCStrings::GetMonthAbreviation(pvd->sankranti_day.month)
				, pvd->sankranti_day.GetHour(), pvd->sankranti_day.GetMinuteRound()
				, GCStrings::GetDSTSignature(pvd->nDST));
			int length = str.GetLength();
			int seplen = _tcslen(gpszSeparator);
			length = (seplen - length) / 2;
			dayText += (gpszSeparator + (seplen - length));
			dayText += str;
			dayText += (gpszSeparator + (seplen - length));
			dayText += "\r\n";
		}

		if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
		{
			double h, m;
			VCTIME ksayaDate;

			// zaciatok ksaya tithi
			m = modf(pvd->ksaya_time1*24, &h);
			ksayaDate = pvd->date;
			if (pvd->ksaya_day1 < 0.0)
				ksayaDate.PreviousDay();
			str2.Format("%d %s %02d:%02d", ksayaDate.day, GCStrings::GetMonthAbreviation(ksayaDate.month), int(h), int(m*60));

			// end of ksaya tithi
			m = modf(pvd->ksaya_time2*24, &h);
			ksayaDate = pvd->date;
			if (pvd->ksaya_day2 < 0.0)
				ksayaDate.PreviousDay();
			str3.Format("%d %s %02d:%02d", ksayaDate.day, GCStrings::GetMonthAbreviation(ksayaDate.month), int(h), int(m*60));

			// print info
			str.Format("%s: %s -- %s %s %s (%s)", GCStrings::getString(89).c_str(), GCStrings::GetTithiName((pvd->astrodata.nTithi + 29)%30), str2.c_str(), 
				GCStrings::getString(851).c_str(), str3.c_str(), GCStrings::GetDSTSignature(pvd->nDST));
			//str.Format("%s %s %s %s %s (%s)", GCStrings::getString(89).c_str(), GCStrings::getString(850).c_str(), str2.c_str(), GCStrings::getString(851).c_str(), str3.c_str(), GCStrings::GetDSTSignature(pvd->nDST));
			AddListText(dayText, str);
		}

		if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
		{
			if (pvd->is_vriddhi)
				AddListText(dayText, GCStrings::getString(90));
		}

		if (pvd->nCaturmasya & CMASYA_CONT_MASK)
		{
			int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
			AddListText(dayText, GCStrings::getString( 111 + n ));
		}

		if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
		{
			str.Format("%s [PURNIMA SYSTEM]"
				, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					       + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
				);
			AddListText(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
			{
				AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str() );
			}
		}

		if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
		{
			str.Format("%s [PRATIPAT SYSTEM]"
				, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					       + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
				);
			AddListText(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
			{
				AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str() );
			}
		}

		if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
		{
			str.Format("%s [EKADASI SYSTEM]"
				, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					       + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
				);
			AddListText(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
			{
				AddListText(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str() );
			}
		}

		// tithi at arunodaya
		if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
		{
			str.Format("%s: %s", GCStrings::getString(98).c_str(), GCStrings::GetTithiName(pvd->astrodata.nTithiArunodaya));
			AddListText(dayText, str);
		}

		//"Arunodaya Time",//1
		if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(99).c_str(), pvd->astrodata.sun.arunodaya.hour
				, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListText(dayText, str);
		}
		//"Sunrise Time",//2
		//"Sunset Time",//3
		if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(51).c_str(), pvd->astrodata.sun.rise.hour
				, pvd->astrodata.sun.rise.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListText(dayText, str);
		}
		/* BEGIN GCAL 1.4.3 */
		// Noon time
		if (GCDisplaySettings::getValue(34) == 1)//m_dshow.m_sunset)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(857).c_str(), pvd->astrodata.sun.noon.hour
				, pvd->astrodata.sun.noon.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListText(dayText, str);
		}
		/* END GCAL 1.4.3 */
		if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(52).c_str(), pvd->astrodata.sun.set.hour
				, pvd->astrodata.sun.set.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListText(dayText, str);
		}
		//"Moonrise Time",//4
		if (GCDisplaySettings::getValue(4) == 1)
		{
			if (pvd->moonrise.hour < 0)
				str = GCStrings::getString(136).c_str();
			else
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(53).c_str(), pvd->moonrise.hour
					, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
			}
			AddListText(dayText, str);
		}
		//"Moonset Time",//5
		if (GCDisplaySettings::getValue(5) == 1)
		{
			if (pvd->moonrise.hour < 0)
				str = GCStrings::getString(137);
			else
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(54).c_str(), pvd->moonset.hour
					, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
			}
			AddListText(dayText, str);
		}
		///"Sun Longitude",//9
		if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s: %f (*)", GCStrings::getString(100).c_str(), pvd->astrodata.sun.longitude_deg);
			AddListText(dayText, str);
		}
		//"Moon Longitude",//10
		if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s: %f (*)", GCStrings::getString(101).c_str(), pvd->astrodata.moon.longitude_deg);
			AddListText(dayText, str);
		}
		//"Ayanamsha value",//11
		if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s %f (%s) (*)", GCStrings::getString(102).c_str(), pvd->astrodata.msAyanamsa, GCAyanamsha::GetAyanamsaName(GCAyanamsha::GetAyanamsaType()));
			AddListText(dayText, str);
		}
		//"Julian Day",//12
		if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s %f (*)", GCStrings::getString(103).c_str(), pvd->astrodata.jdate);
			AddListText(dayText, str);
		}

	return 0;
}

int FormatCalendarOld(TResultCalendar &daybuff, TString &m_text) 
{
	int k, nMasaHeader;
	TString str, str2, str3, dayText;

	char * spaces = "                                                                                ";
	VAISNAVADAY * pvd, * prevd, *nextd;
	int lastmasa = -1;
	int lastmonth = -1;
	int tp1;
	double rate;
	DlgCalcProgress dcp;
	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);

	m_text.Empty();

	for (k = 0; k < daybuff.m_vcCount; k++)
	{

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (daybuff.m_pProgress != NULL)
		{
			if (bCalcMoon)
			{
				daybuff.m_pProgress->SetPos(int(90.8 + 9.2 * k / daybuff.m_vcCount));
			}
			else
			{
				rate = double(k) / daybuff.m_vcCount;
				daybuff.m_pProgress->SetPos(int(58.8 + 41.2 * rate * rate));
			}
		}

		if (pvd)
		{
			nMasaHeader = 0;
			if ((GCDisplaySettings::getValue(18) == 1) && (pvd->astrodata.nMasa != lastmasa))
			{
				nMasaHeader = 1;
				m_text += "\r\n";
				str.Format("%s %s, Gaurabda %d", GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str(), pvd->astrodata.nGaurabdaYear);
				tp1 = (80 - str.GetLength())/2;
				str.Insert(0, (spaces + (79 - tp1)));
				str += spaces;
				str.Insert(80 - _tcslen(GCStrings::GetVersionText()), GCStrings::GetVersionText());
				str.Left(80, str3);
				m_text += str3;
				m_text += "\r\n";
				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
					AddListText(m_text, GCStrings::getString(128));
				}
				m_text += "\r\n";
				lastmasa = pvd->astrodata.nMasa;
			}

			if ((GCDisplaySettings::getValue(19) == 1) && (pvd->date.month != lastmonth))
			{
				nMasaHeader = 1;
				m_text += "\r\n";
				str.Format("%s %d", GCStrings::getString(759 + pvd->date.month).c_str(), pvd->date.year);
				tp1 = (80 - str.GetLength())/2;
				str.Insert(0, (spaces + (79 - tp1)));
				str += spaces;
				str.Insert(80 - _tcslen(GCStrings::GetVersionText()), GCStrings::GetVersionText());
				str.Left(80, str3);
				m_text += str3;
				m_text += "\r\n";
				lastmonth = pvd->date.month;
			}
			if (nMasaHeader)
			{
				m_text += (spaces + (79 - (80 - daybuff.m_Location.m_strFullName.GetLength())/2));
				m_text += daybuff.m_Location.m_strFullName;

				m_text += "\r\n\r\n";
				/* BEGIN GCAL 1.4.3 */
				nMasaHeader = m_text.GetLength();
				m_text += " DATE            TITHI                         ";
				if (GCDisplaySettings::getValue(39)) m_text += "PAKSA ";
				else m_text += "      ";
				if (GCDisplaySettings::getValue(37)) m_text += "YOGA      ";
				if (GCDisplaySettings::getValue(36)) m_text += "NAKSATRA       ";
				if (GCDisplaySettings::getValue(38)) m_text += "FAST ";
				if (GCDisplaySettings::getValue(41)) m_text += "RASI           ";
				nMasaHeader = m_text.GetLength() - nMasaHeader;
				m_text += "\r\n";
				/* END GCAL 1.4.3 */
				while(nMasaHeader > 0)
				{
					m_text += "----------";
					nMasaHeader -= 10;
				}
				nMasaHeader = 0;
				m_text += "\r\n";
			}

			if (AvcGetOldCalendarDayText(pvd, dayText) > 0)
				goto _resolve_text;

			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						AddListText(dayText, str);
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						AddListText(dayText, str);
					}
				}
			}
			/* BEGIN GCAL 1.4.3 */
			if (GCDisplaySettings::getValue(35) && prevd && nextd)
			{
				if (prevd->nDST == 0 && pvd->nDST==1)
					AddListText(dayText, GCStrings::getString(855).c_str());
				else if (pvd->nDST==1 && nextd->nDST==0)
					AddListText(dayText, GCStrings::getString(856).c_str());
			}

			/* END GCAL 1.4.3 */

		_resolve_text:
			if (GCDisplaySettings::getValue(20) == 0)
				m_text += dayText;
			else if (dayText.GetLength() > 90)
				m_text += dayText;


		}
//		date.shour = 0;
//		date.NextDay();
	}

//	dcp.DestroyWindow();

	return 1;
}

int FormatCalendarICAL(TResultCalendar &daybuff, TString &m_text)
{
	int k, nFestClass;
	int initialLength = 0;
	int lastmasa = -1;
	int lastmonth = -1;
	int tzoffset = 0, tzoff;
	TString str, dayText, str2, str3;
	VAISNAVADAY * pvd, * prevd, * nextd;
	char * SPACE_BEFORE_LINE = " , ";
	VCTIME vc;
	CLocationRef &loc = daybuff.m_Location;

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_text = "BEGIN:VCALENDAR\nVERSION:2.0\nX-WR-CALNAME:VAISNAVA\nPRODID:-//GBC Calendar Comitee//GCAL//EN\n";
	m_text += "X-WR-RELCALID:";
	str2.Format("%08d-%04d-%04d-%04d-%04d%08d", st.wYear + st.wMilliseconds, st.wDay * st.wDayOfWeek, st.wMonth,
		st.wHour, st.wMinute + st.wMilliseconds);
	m_text += str2;
	m_text += "\nX-WR-TIMEZONE:";

	m_text += TTimeZone::GetTimeZoneName(loc.m_nDST);
	m_text += "\n";

	m_text += "CALSCALE:GREGORIAN\nMETHOD:PUBLISH\n";
	m_text += "BEGIN:VTIMEZONE\nTZID:";
	m_text += TTimeZone::GetTimeZoneName(loc.m_nDST);
	str2.Format("\nLAST-MODIFIED:%04d%02d%02dT%02d%02d%02dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	m_text += str2;

	tzoffset = int(TTimeZone::GetTimeZoneOffset(loc.m_nDST) * 60.0);
	tzoff = tzoffset + TTimeZone::GetTimeZoneBias(loc.m_nDST);

	if (TTimeZone::GetTimeZoneBias(loc.m_nDST) > 0)
	{
		TTimeZone::GetDaylightTimeStartDate(loc.m_nDST, st.wYear, vc);
		str2.Format("\nBEGIN:DAYLIGHT\nDTSTART:%04d%02d%02dT%02d0000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str2.Format("\nTZOFFSETTO:%c%02d%02d", (tzoff > 0 ? '+' : '-'), abs(tzoff) / 60, abs(tzoff) % 60);
		m_text += str2;

		str2.Format("\nTZOFFSETFROM:%c%02d%02d", '+', 0, 0);
		m_text += str2;

		TTimeZone::GetNormalTimeStartDate(loc.m_nDST, st.wYear, vc);
		m_text += "\nEND:DAYLIGHT\nBEGIN:STANDARD\nDTSTART:";
		str2.Format("%04d%02d%02dT%02d0000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str2.Format("\nTZOFFSETTO:%c%02d%02d", (tzoffset > 0 ? '+' : '-'), abs(tzoffset)/60, abs(tzoffset) % 60);
		m_text += str2;
		str2.Format("\nTZOFFSETFROM:%c%02d%02d", (tzoff > 0 ? '+' : '-'), abs(tzoff) / 60, abs(tzoff) % 60);
		m_text += str2;
		m_text += "\nEND:STANDARD\n";
	}
	else
	{
		m_text += "\nBEGIN:STANDARD\nDTSTART:";
		str2.Format("%04d0101T000000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str.Format("\nTZOFFSETTO:%+02d%02d", tzoffset/60, abs(tzoffset) % 60);
		m_text += str2;
		str2.Format("\nTZOFFSETFROM:+0000");
		m_text += str2;
		m_text += "\nEND:STANDARD\n";
	}

	m_text += "END:VTIMEZONE\n";

	for (k = 0; k < daybuff.m_PureCount; k++)
	{
//		date.shour = 0.0;
//		date.TimeZone = earth.tzone;

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (pvd)
		{
			dayText.Empty();

			if (pvd->astrodata.nMasa != lastmasa)
			{
				str.Format("%s %s, Gaurabda %d", GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str(), pvd->astrodata.nGaurabdaYear);
				dayText += str;
				dayText += "\n";
				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
					if (dayText.IsEmpty() == FALSE)
						dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(128);
					dayText += "\n";
				}

				lastmasa = pvd->astrodata.nMasa;
				initialLength = -1;
			}
			else
			{
				initialLength = 0;
			}

			if (dayText.IsEmpty() == FALSE)
				dayText += SPACE_BEFORE_LINE;
			dayText += GCStrings::GetTithiName(pvd->astrodata.nTithi);

			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) || (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					dayText += " ";
					if (pvd->nMhdType == EV_NULL)
					{
						dayText += GCStrings::getString(58);
					}
					else
					{
						dayText += GCStrings::getString(59);
					}
				}
			}
			dayText += "\n";
			initialLength += dayText.GetLength();

			if (pvd->astrodata.sun.longitude_deg < 0.0)
			{
				goto _resolve_text;
			}

//			if (GCDisplaySettings::getValue(17) == 1)
			{
//				double h1, m1;
				if (pvd->ekadasi_parana)
				{
					pvd->GetTextEP(str);
					str.Delete(0, 17);
					dayText += SPACE_BEFORE_LINE;
					dayText += str;
					dayText += "\n";

/*					m_text += "BEGIN:VEVENT\n";
					if (pvd->eparana_time1 >= 0.0) {
						m1 = modf(pvd->eparana_time1 * 24.0, &h1) * 60;
						str2.Format("DTSTART;TZID=%s:%04d%02d%02dT%02d%02d00\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTSTART;TZID=%s:%04d%02d%02dT%000000\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day);
					}
					m_text += str2;
					if (pvd->eparana_time2 >= 0.0) {
						m1 = modf(pvd->eparana_time2 * 24.0, &h1) * 60;
						str2.Format("DTEND;TZID=%s:%04d%02d%02dT%02d%02d00\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTEND;TZID=%s:%04d%02d%02dT%235959\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day);
					}
					m_text += str2;
					m_text += "SUMMARY:";
					m_text += GCStrings::getString(60];
					m_text += "\nSEQUENCE:1\nEND:VEVENT\n";
*/
				}
			}

//			if (GCDisplaySettings::getValue(6) == 1)
			{
				if (pvd->festivals)
				{
					int i = pvd->GetHeadFestival();
					while(pvd->GetNextFestival(i, str2))
					{
						if (str2.GetLength() > 1)
						{
							nFestClass = pvd->GetFestivalClass(str2);
							if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
							{
								dayText += SPACE_BEFORE_LINE;
								dayText += str2;
								dayText += "\n";
							}
						}
					}
				}
			}

			if (/*GCDisplaySettings::getValue(16) == 1 &&*/ pvd->sankranti_zodiac >= 0)
			{
				str.Format(" %s %s", GCStrings::GetSankrantiName(pvd->sankranti_zodiac), GCStrings::getString(56).c_str());
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
			}

/*			if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
			{
				double h, m;
				VAISNAVADAY * p;


				if (pvd->ksaya_time1 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time1)*24, &h);
				else
					m = modf(pvd->ksaya_time1*24, &h);
				p = (pvd->ksaya_time1 < 0.0) ? prevd : pvd;
				str2.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				if (pvd->ksaya_time2 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
				else
					m = modf(pvd->ksaya_time2*24, &h);
				p = (pvd->ksaya_time2 < 0.0) ? prevd : pvd;
				str3.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				str.Format("%s %s %s %s %s (%s)", GCStrings::getString(89], GCStrings::getString(850], str2, GCStrings::getString(851], str3, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}*/

			/*if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (pvd->is_vriddhi)
					AddListText(dayText, GCStrings::getString(90]);
			}*/

			if (pvd->nCaturmasya & CMASYA_CONT_MASK)
			{
				int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
				dayText += SPACE_BEFORE_LINE;
				dayText += GCStrings::getString( 111 + n ).c_str();
				dayText += "\n";
			}

			if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				str.Format("%s [PURNIMA SYSTEM]"
					, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					           + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str();
					dayText += "\n";
				}
			}

			if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				str.Format("%s [PRATIPAT SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					           + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str();
					dayText += "\n";
				}
			}

			if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				str.Format("%s [EKADASI SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					           + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str();
					dayText += "\n";
				}
			}

/*			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str.Format("%s: %s", GCStrings::getString(98], GetTithiName(pvd->astrodata.nTithiArunodaya));
				AddListText(dayText, str);
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(99], pvd->astrodata.sun.arunodaya.hour
					, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}
*/			//"Sunrise Time",//2
			//"Sunset Time",//3
			if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(51).c_str(), pvd->astrodata.sun.rise.hour
					, pvd->astrodata.sun.rise.min, GCStrings::GetDSTSignature(pvd->nDST));
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
			}
			if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(52).c_str(), pvd->astrodata.sun.set.hour
					, pvd->astrodata.sun.set.min, GCStrings::GetDSTSignature(pvd->nDST));
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";

			}
/*			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(136];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(53], pvd->moonrise.hour
						, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(137];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(54], pvd->moonset.hour
						, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(100], pvd->astrodata.sun.longitude_deg);
				AddListText(dayText, str);
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(101], pvd->astrodata.moon.longitude_deg);
				AddListText(dayText, str);
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (%s)", GCStrings::getString(102], pvd->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
				AddListText(dayText, str);
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f", GCStrings::getString(103], pvd->astrodata.jdate);
				AddListText(dayText, str);
			}

*///			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += SPACE_BEFORE_LINE;
						dayText += str;
						dayText += "\n";
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += SPACE_BEFORE_LINE;
						dayText += str;
						dayText += "\n";
					}
				}
			}

		_resolve_text:
			if (dayText.GetLength() > initialLength)
			{
				m_text += "BEGIN:VEVENT\n";
				str2.Format("DTSTART;VALUE=DATE:%04d%02d%02d\n", pvd->date.year, pvd->date.month, pvd->date.day);
				m_text += str2;
				/*str2.Format("DTEND;VALUE=DATE:%04d%02d%02dT%02d%02d%02d\n", pvd->date.year, pvd->date.month, pvd->date.day,
					pvd->astrodata.sun.set.hour, pvd->astrodata.sun.set.min, pvd->astrodata.sun.set.sec);
				m_text += str2;*/
				str2.Format("LOCATION:%s\n", loc.m_strName.c_str());
				str2.Replace(",", "\\,");
				m_text += str2;
				m_text += "SUMMARY:";
				dayText.TrimLeft();
				dayText.Replace(",", "\\,");
				m_text += dayText;
				str2.Format("UID:%08d-%04d-%04d-%04d-%08d%04d\n", st.wYear, st.wMonth*30 + st.wDay, st.wHour*60 + st.wMinute, st.wSecond, st.wMilliseconds, k);
				m_text += str2;
				m_text += "DURATION:P1D\nSEQUENCE:1\nEND:VEVENT\n";
			}
		}
	}

	m_text += "END:VCALENDAR\n";
	return 1;
}

int FormatCalendarCSV(TResultCalendar &daybuff, TString &m_text)
{
	int k, nFestClass;
	int initialLength = 0;
	int lastmasa = -1;
	int lastmonth = -1;
	int tzoffset = 0;
	TString str, dayText, str2, str3;
	VAISNAVADAY * pvd, * prevd, * nextd;
	char * SPACE_BEFORE_LINE = " , ";
	VCTIME vc;

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_text = "\"Subject\",\"Begin Date\",\"Start\",\"End Date\",\"End\",\"WholeDay\",\"Alarm\"\n";

	for (k = 0; k < daybuff.m_PureCount; k++)
	{
//		date.shour = 0.0;
//		date.TimeZone = earth.tzone;

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (pvd)
		{
			dayText.Empty();

			if (pvd->astrodata.nMasa != lastmasa)
			{
				lastmasa = pvd->astrodata.nMasa;
				initialLength = -1;
			}
			else
			{
				initialLength = 0;
			}

			if (dayText.IsEmpty() == FALSE)
				dayText += SPACE_BEFORE_LINE;
			dayText += GCStrings::GetTithiName(pvd->astrodata.nTithi);

			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) || (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					dayText += " ";
					if (pvd->nMhdType == EV_NULL)
					{
						dayText += GCStrings::getString(58);
					}
					else
					{
						dayText += GCStrings::getString(59);
					}
				}
			}
			dayText += "; ";
			initialLength = dayText.GetLength();

			if (pvd->astrodata.sun.longitude_deg < 0.0)
			{
				goto _resolve_text;
			}

//			if (GCDisplaySettings::getValue(17) == 1)
			{
//				double h1, m1;
				if (pvd->ekadasi_parana)
				{
					pvd->GetTextEP(str);
					str.Delete(0, 17);
					dayText += str;
					dayText += "; ";

/*					m_text += "BEGIN:VEVENT\n";
					if (pvd->eparana_time1 >= 0.0) {
						m1 = modf(pvd->eparana_time1 * 24.0, &h1) * 60;
						str2.Format("DTSTART;TZID=%s:%04d%02d%02dT%02d%02d00\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTSTART;TZID=%s:%04d%02d%02dT%000000\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day);
					}
					m_text += str2;
					if (pvd->eparana_time2 >= 0.0) {
						m1 = modf(pvd->eparana_time2 * 24.0, &h1) * 60;
						str2.Format("DTEND;TZID=%s:%04d%02d%02dT%02d%02d00\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTEND;TZID=%s:%04d%02d%02dT%235959\n", TTimeZone::GetTimeZoneName(nDst),
							pvd->date.year, pvd->date.month, pvd->date.day);
					}
					m_text += str2;
					m_text += "SUMMARY:";
					m_text += GCStrings::getString(60];
					m_text += "\nSEQUENCE:1\nEND:VEVENT\n";
*/
				}
			}

//			if (GCDisplaySettings::getValue(6) == 1)
			{
				if (pvd->festivals)
				{
					int i = pvd->GetHeadFestival();
					while(pvd->GetNextFestival(i, str2))
					{
						if (str2.GetLength() > 1)
						{
							nFestClass = pvd->GetFestivalClass(str2);
							if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
							{
								dayText += str2;
								dayText += "; ";
							}
						}
					}
				}
			}

			if (/*GCDisplaySettings::getValue(16) == 1 &&*/ pvd->sankranti_zodiac >= 0)
			{
				str.Format("%s %s; ", GCStrings::GetSankrantiName(pvd->sankranti_zodiac), GCStrings::getString(56).c_str());
				dayText += str;
			}

/*			if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
			{
				double h, m;
				VAISNAVADAY * p;


				if (pvd->ksaya_time1 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time1)*24, &h);
				else
					m = modf(pvd->ksaya_time1*24, &h);
				p = (pvd->ksaya_time1 < 0.0) ? prevd : pvd;
				str2.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				if (pvd->ksaya_time2 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
				else
					m = modf(pvd->ksaya_time2*24, &h);
				p = (pvd->ksaya_time2 < 0.0) ? prevd : pvd;
				str3.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				str.Format("%s %s %s %s %s (%s)", GCStrings::getString(89], GCStrings::getString(850], str2, GCStrings::getString(851], str3, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}*/

			/*if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (pvd->is_vriddhi)
					AddListText(dayText, GCStrings::getString(90]);
			}*/

			if (pvd->nCaturmasya & CMASYA_CONT_MASK)
			{
				int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
				dayText += GCStrings::getString( 111 + n );
				dayText += "; ";
			}

			if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				str.Format("%s [PURNIMA SYSTEM]; "
					, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					           + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
					);
				dayText += str;
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str();
					dayText += "; ";
				}
			}

			if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				str.Format("%s [PRATIPAT SYSTEM]; "
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					           + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
					);
				dayText += str;
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str();
					dayText += "; ";
				}
			}

			if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				str.Format("%s [EKADASI SYSTEM]; "
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					           + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
					);
				dayText += str;
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str();
					dayText += "; ";
				}
			}

/*			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str.Format("%s: %s", GCStrings::getString(98], GetTithiName(pvd->astrodata.nTithiArunodaya));
				AddListText(dayText, str);
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(99], pvd->astrodata.sun.arunodaya.hour
					, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}
*/			//"Sunrise Time",//2
			//"Sunset Time",//3
			if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
			{
				str.Format("Sunrise %d:%02d; ", pvd->astrodata.sun.rise.hour
					, pvd->astrodata.sun.rise.min);
				dayText += str;
			}
			if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
			{
				str.Format("Sunset %d:%02d; ", pvd->astrodata.sun.set.hour, 
					pvd->astrodata.sun.set.min);
				dayText += str;
			}

/*			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(136];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(53], pvd->moonrise.hour
						, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(137];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(54], pvd->moonset.hour
						, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(100], pvd->astrodata.sun.longitude_deg);
				AddListText(dayText, str);
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(101], pvd->astrodata.moon.longitude_deg);
				AddListText(dayText, str);
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (%s)", GCStrings::getString(102], pvd->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
				AddListText(dayText, str);
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f", GCStrings::getString(103], pvd->astrodata.jdate);
				AddListText(dayText, str);
			}

*///			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += str;
						dayText += "; ";
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += str;
						dayText += "; ";
					}
				}
			}

		_resolve_text:
			if (dayText.GetLength() > initialLength || (GCDisplaySettings::getValue(20) == 0))
			{
				m_text += "\"";
				m_text += dayText;
				m_text += "\",";

				str2.Format("\"%d.%d.%d\",\"0:00:00\",\"%d.%d.%d\",\"0:00:00\",\"True\",\"False\"\n", 
					pvd->date.day, pvd->date.month, pvd->date.year, nextd->date.day,
					nextd->date.month, nextd->date.year);
				m_text += str2;
			}
		}
	}

	return 1;
}

int FormatCalendarVCAL(TResultCalendar &daybuff, TString &m_text)
{
	int k, nFestClass;
	int initialLength = 0;
	int lastmasa = -1;
	int lastmonth = -1;
	int tzoffset = 0;
	TString str, dayText, str2, str3;
	VAISNAVADAY * pvd, * prevd, * nextd;
	char * SPACE_BEFORE_LINE = " , ";
	VCTIME vc;

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_text = "BEGIN:VCALENDAR\nVERSION:1.0\nX-WR-CALNAME:VAISNAVA\nPRODID:-//GBC Calendar Comitee//GCAL//EN\n";
	m_text += "X-WR-RELCALID:";
	str2.Format("%08d-%04d-%04d-%04d-%04d%08d", st.wYear + st.wMilliseconds, st.wDay * st.wDayOfWeek, st.wMonth,
		st.wHour, st.wMinute + st.wMilliseconds);
	m_text += str2;
	m_text += "\nX-WR-TIMEZONE:";

	m_text += TTimeZone::GetTimeZoneName(daybuff.m_Location.m_nDST);
	m_text += "\n";

	m_text += "CALSCALE:GREGORIAN\nMETHOD:PUBLISH\n";
	/*m_text += "BEGIN:VTIMEZONE\nTZID:";
	m_text += TTimeZone::GetTimeZoneName(nDst);
	str2.Format("\nLAST-MODIFIED:%04d%02d%02dT%02d%02d%02dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	m_text += str2;

	tzoffset = int(TTimeZone::GetTimeZoneOffset(nDst) * 60.0);
	tzoff = tzoffset + TTimeZone::GetTimeZoneBias(nDst);

	if (TTimeZone::GetTimeZoneBias(nDst) > 0)
	{
		TTimeZone::GetDaylightTimeStartDate(nDst, st.wYear, vc);
		str2.Format("\nBEGIN:DAYLIGHT\nDTSTART:%04d%02d%02dT%02d0000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str2.Format("\nTZOFFSETTO:%c%02d%02d", (tzoff > 0 ? '+' : '-'), abs(tzoff) / 60, abs(tzoff) % 60);
		m_text += str2;

		str2.Format("\nTZOFFSETFROM:%c%02d%02d", '+', 0, 0);
		m_text += str2;

		TTimeZone::GetNormalTimeStartDate(nDst, st.wYear, vc);
		m_text += "\nEND:DAYLIGHT\nBEGIN:STANDARD\nDTSTART:";
		str2.Format("%04d%02d%02dT%02d0000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str2.Format("\nTZOFFSETTO:%c%02d%02d", (tzoffset > 0 ? '+' : '-'), abs(tzoffset)/60, abs(tzoffset) % 60);
		m_text += str2;
		str2.Format("\nTZOFFSETFROM:%c%02d%02d", (tzoff > 0 ? '+' : '-'), abs(tzoff) / 60, abs(tzoff) % 60);
		m_text += str2;
		m_text += "\nEND:STANDARD\n";
	}
	else
	{
		m_text += "\nBEGIN:STANDARD\nDTSTART:";
		str2.Format("%04d0101T000000", vc.year, vc.month, vc.day, vc.GetHour());
		m_text += str2;

		str.Format("\nTZOFFSETTO:%+02d%02d", tzoffset/60, abs(tzoffset) % 60);
		m_text += str2;
		str2.Format("\nTZOFFSETFROM:+0000");
		m_text += str2;
		m_text += "\nEND:STANDARD\n";
	}

	m_text += "END:VTIMEZONE\n";
*/
	for (k = 0; k < daybuff.m_PureCount; k++)
	{
//		date.shour = 0.0;
//		date.TimeZone = earth.tzone;

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (pvd)
		{
			dayText.Empty();

			if (pvd->astrodata.nMasa != lastmasa)
			{
				str.Format("%s %s, Gaurabda %d", GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str(), pvd->astrodata.nGaurabdaYear);
				dayText += str;
				dayText += "\n";
				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
					if (dayText.IsEmpty() == FALSE)
						dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(128);
					dayText += "\n";
				}

				lastmasa = pvd->astrodata.nMasa;
				initialLength = -1;
			}
			else
			{
				initialLength = 0;
			}

			if (dayText.IsEmpty() == FALSE)
				dayText += SPACE_BEFORE_LINE;
			dayText += GCStrings::GetTithiName(pvd->astrodata.nTithi);

			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) || (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					dayText += " ";
					if (pvd->nMhdType == EV_NULL)
					{
						dayText += GCStrings::getString(58);
					}
					else
					{
						dayText += GCStrings::getString(59);
					}
				}
			}
			dayText += "\n";
			initialLength += dayText.GetLength();

			if (pvd->astrodata.sun.longitude_deg < 0.0)
			{
				goto _resolve_text;
			}

//			if (GCDisplaySettings::getValue(17) == 1)
			{
				double h1, m1;
				if (pvd->ekadasi_parana)
				{
/*					pvd->GetTextEP(str);
					str.Delete(0, 17);
					dayText += SPACE_BEFORE_LINE;
					dayText += str;
					dayText += "\n";
*/
					m_text += "BEGIN:VEVENT\n";
					if (pvd->eparana_time1 >= 0.0) {
						m1 = modf(pvd->eparana_time1, &h1) * 60;
						str2.Format("DTSTART:%04d%02d%02dT%02d%02d00\n", pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTSTART:%04d%02d%02dT000000\n", pvd->date.year, pvd->date.month, pvd->date.day);
					}
					m_text += str2;
					if (pvd->eparana_time2 >= 0.0) {
						m1 = modf(pvd->eparana_time2, &h1) * 60;
						str2.Format("DTEND:%04d%02d%02dT%02d%02d00\n", pvd->date.year, pvd->date.month, pvd->date.day, int(h1), int(m1));
					} else {
						str2.Format("DTEND:%04d%02d%02dT%02d%02d%00\n", pvd->date.year, pvd->date.month, pvd->date.day, pvd->astrodata.sun.set.hour, pvd->astrodata.sun.set.min);
					}
					m_text += str2;
					m_text += "SUMMARY:";
					m_text += GCStrings::getString(60);
					m_text += "\nSEQUENCE:1\nEND:VEVENT\n";

				}
			}

//			if (GCDisplaySettings::getValue(6) == 1)
			{
				if (pvd->festivals)
				{
					int i = pvd->GetHeadFestival();
					while(pvd->GetNextFestival(i, str2))
					{
						if (str2.GetLength() > 1)
						{
							nFestClass = pvd->GetFestivalClass(str2);
							if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
							{
								dayText += SPACE_BEFORE_LINE;
								dayText += str2;
								dayText += "\n";
							}
						}
					}
				}
			}

			if (/*GCDisplaySettings::getValue(16) == 1 &&*/ pvd->sankranti_zodiac >= 0)
			{
				str.Format(" %s %s", GCStrings::GetSankrantiName(pvd->sankranti_zodiac), GCStrings::getString(56).c_str());
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
			}

/*			if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
			{
				double h, m;
				VAISNAVADAY * p;


				if (pvd->ksaya_time1 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time1)*24, &h);
				else
					m = modf(pvd->ksaya_time1*24, &h);
				p = (pvd->ksaya_time1 < 0.0) ? prevd : pvd;
				str2.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				if (pvd->ksaya_time2 < 0.0)
					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
				else
					m = modf(pvd->ksaya_time2*24, &h);
				p = (pvd->ksaya_time2 < 0.0) ? prevd : pvd;
				str3.Format("%d %s, %02d:%02d", p->date.day, GCStrings::GetMonthAbreviation(p->date.month), int(h), int(m*60));

				str.Format("%s %s %s %s %s (%s)", GCStrings::getString(89], GCStrings::getString(850], str2, GCStrings::getString(851], str3, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}*/

			/*if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (pvd->is_vriddhi)
					AddListText(dayText, GCStrings::getString(90]);
			}*/

			if (pvd->nCaturmasya & CMASYA_CONT_MASK)
			{
				int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
				dayText += SPACE_BEFORE_LINE;
				dayText += GCStrings::getString( 111 + n );
				dayText += "\n";
			}

			if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				str.Format("%s [PURNIMA SYSTEM]"
					, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					           + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str();
					dayText += "\n";
				}
			}

			if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				str.Format("%s [PRATIPAT SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					           + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str();
					dayText += "\n";
				}
			}

			if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				str.Format("%s [EKADASI SYSTEM]"
					, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					           + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
					);
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					dayText += SPACE_BEFORE_LINE;
					dayText += GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str();
					dayText += "\n";
				}
			}

/*			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str.Format("%s: %s", GCStrings::getString(98], GetTithiName(pvd->astrodata.nTithiArunodaya));
				AddListText(dayText, str);
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(99], pvd->astrodata.sun.arunodaya.hour
					, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
				AddListText(dayText, str);
			}
*/			//"Sunrise Time",//2
			//"Sunset Time",//3
			if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(51).c_str(), pvd->astrodata.sun.rise.hour
					, pvd->astrodata.sun.rise.min, GCStrings::GetDSTSignature(pvd->nDST));
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";
			}
			if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(52).c_str(), pvd->astrodata.sun.set.hour
					, pvd->astrodata.sun.set.min, GCStrings::GetDSTSignature(pvd->nDST));
				dayText += SPACE_BEFORE_LINE;
				dayText += str;
				dayText += "\n";

			}
/*			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(136];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(53], pvd->moonrise.hour
						, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (pvd->moonrise.hour < 0)
					str = GCStrings::getString(137];
				else
				{
					str.Format("%s %d:%02d (%s)", GCStrings::getString(54], pvd->moonset.hour
						, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
				}
				AddListText(dayText, str);
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(100], pvd->astrodata.sun.longitude_deg);
				AddListText(dayText, str);
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s: %f", GCStrings::getString(101], pvd->astrodata.moon.longitude_deg);
				AddListText(dayText, str);
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f (%s)", GCStrings::getString(102], pvd->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
				AddListText(dayText, str);
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str.Format("%s %f", GCStrings::getString(103], pvd->astrodata.jdate);
				AddListText(dayText, str);
			}

*///			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += SPACE_BEFORE_LINE;
						dayText += str;
						dayText += "\n";
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						dayText += SPACE_BEFORE_LINE;
						dayText += str;
						dayText += "\n";
					}
				}
			}

		_resolve_text:
			if (dayText.GetLength() > initialLength)
			{
				m_text += "BEGIN:VEVENT\n";
				str2.Format("DTSTART:%04d%02d%02dT%02d%02d%02d\n", pvd->date.year, pvd->date.month, pvd->date.day,
					pvd->astrodata.sun.rise.hour, pvd->astrodata.sun.rise.min, pvd->astrodata.sun.rise.sec);
				m_text += str2;
				str2.Format("DTEND:%04d%02d%02dT%02d%02d%02d\n", pvd->date.year, pvd->date.month, pvd->date.day,
					pvd->astrodata.sun.set.hour, pvd->astrodata.sun.set.min, pvd->astrodata.sun.set.sec);
				m_text += str2;
				str2.Format("LOCATION:%s\n", daybuff.m_Location.m_strName.c_str());
				str2.Replace(",", "\\,");
				m_text += str2;
				m_text += "SUMMARY:";
				dayText.TrimLeft();
				dayText.Replace(",", "\\,");
				m_text += dayText;
				str2.Format("UID:%08d-%04d-%04d-%04d-%08d%04d\n", st.wYear, st.wMonth*30 + st.wDay, st.wHour*60 + st.wMinute, st.wSecond, st.wMilliseconds, k);
				m_text += str2;
				m_text += "SEQUENCE:1\nEND:VEVENT\n";
			}
		}
	}

	m_text += "END:VCALENDAR\n";
	return 1;
}

void AvcGetTodayInfo(VCTIME vc, CLocationRef & loc, TString &str)
{
	TString str2, str3, str4;

	int k = 0, nFestClass;

	TResultCalendar db;

	VCTIME vc2 = vc;
	vc2.tzone = loc.m_fTimezone;
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	db.CalculateCalendar(loc, vc2, 9);

	int i = db.FindDate(vc);
	VAISNAVADAY * p = db.GetDay(i);

	if (p == NULL)
		return;

	vc.InitWeekDay();

	str2.Format("%s (%s, %s, Timezone: %s)\r\n\r\n[%d %s %d - %s]\r\n  %s, %s %s\r\n  %s %s, %d Gaurabda\r\n\r\n",
		loc.m_strName.c_str(), EARTHDATA::GetTextLatitude(loc.m_fLatitude), EARTHDATA::GetTextLongitude(loc.m_fLongitude),
		TTimeZone::GetTimeZoneName(loc.m_nDST),
		vc.day, GCStrings::GetMonthAbreviation(vc.month), vc.year, GCStrings::getString(vc.dayOfWeek).c_str(),
		GCStrings::GetTithiName(p->astrodata.nTithi), GCStrings::GetPaksaName(p->astrodata.nPaksa), GCStrings::getString(20).c_str(), 
		GCStrings::GetMasaName(p->astrodata.nMasa), GCStrings::getString(22).c_str(), p->astrodata.nGaurabdaYear);

	str = str2;

	if (p->ekadasi_parana)
	{
		p->GetTextEP(str2);
		str2.Delete(0, 14);
		str += str2;
		str += "\r\n";
	}

	// adding mahadvadasi
	// adding spec festivals

	if (p->festivals)
	{
		int i = p->GetHeadFestival();
		while(p->GetNextFestival(i, str2))
		{
			if (str2.GetLength() > 1)
			{
				nFestClass = p->GetFestivalClass(str2);
				if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
				{
					str += "   ";
					str += str2;
					str += "\r\n";
				}
			}
		}
	}

	str += "\r\n";

	if (GCDisplaySettings::getValue(16) == 1 && p->sankranti_zodiac >= 0)
	{
		//double h1, m1;
		//m1 = modf(p->sankranti_day.shour*24, &h1);
		str2.Format(" %s %s (%s %s %s %d %s, %02d:%02d %s) "
			, GCStrings::GetSankrantiName(p->sankranti_zodiac)
			, GCStrings::getString(56).c_str()
			, GCStrings::getString(111).c_str(), GCStrings::GetSankrantiNameEn(p->sankranti_zodiac)
			, GCStrings::getString(852).c_str()
			, p->sankranti_day.day, GCStrings::GetMonthAbreviation(p->sankranti_day.month)
			, p->sankranti_day.GetHour(), p->sankranti_day.GetMinuteRound()
			, GCStrings::GetDSTSignature(p->nDST));
		int length = str2.GetLength();
		int seplen = _tcslen(gpszSeparator);
		length = (seplen - length) / 2;
		str += (gpszSeparator + (seplen - length));
		str += str2;
		str += (gpszSeparator + (seplen - length));
		str += "\r\n";
	}

	if (GCDisplaySettings::getValue(7) == 1 && p->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
	{
		double h, m;
		VCTIME dd;
		m = modf(p->ksaya_time1*24, &h);
		dd = p->date;
		if (p->ksaya_day1 < 0.0)
			dd.PreviousDay();
		str2.Format("%d %s, %02d:%02d", dd.day, GCStrings::GetMonthAbreviation(dd.month), int(h), int(m*60));

//				if (pvd->ksaya_time2 < 0.0)
//					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
//				else
		m = modf(p->ksaya_time2*24, &h);
		dd = p->date;
		if (p->ksaya_day2 < 0.0)
			dd.PreviousDay();
		str3.Format("%d %s, %02d:%02d", dd.day, GCStrings::GetMonthAbreviation(dd.month), int(h), int(m*60));

		str4.Format("%s %s %s %s %s (%s)\r\n", GCStrings::getString(89).c_str(), GCStrings::getString(850).c_str(), str2.c_str(), GCStrings::getString(851).c_str(), str3.c_str(), GCStrings::GetDSTSignature(p->nDST));
		str += str4;
	}
	// adding fasting
			if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
			{
				if (p->is_vriddhi)
				{
					str += GCStrings::getString(90);
					str += "\r\n";
				}
			}


	if (p->nCaturmasya & CMASYA_PURN_MASK)
	{
		str2.Format("%s [PURNIMA SYSTEM]\r\n"
			, GCStrings::getString(107 + (p->nCaturmasya & CMASYA_PURN_MASK_DAY)
					   + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str();
		}
	}

	if (p->nCaturmasya & CMASYA_PRAT_MASK)
	{
		str2.Format("%s [PRATIPAT SYSTEM]\r\n"
			, GCStrings::getString(107 + ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					   + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str();
			str += "\r\n";
		}
	}

	if (p->nCaturmasya & CMASYA_EKAD_MASK)
	{
		str2.Format("%s [EKADASI SYSTEM]\r\n"
			, GCStrings::getString(107 + ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					   + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str();
			str += "\r\n";
		}
	}
			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str2.Format("%s: %s\r\n", GCStrings::getString(98).c_str(), GCStrings::GetTithiName(p->astrodata.nTithiArunodaya));
				str += str2;
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str2.Format("%s %d:%02d (%s)\r\n", GCStrings::getString(99).c_str(), p->astrodata.sun.arunodaya.hour
					, p->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(p->nDST));
				str += str2;
			}

			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (p->moonrise.hour < 0)
					str2 = GCStrings::getString(136);
				else
				{
					str2.Format("%s %d:%02d (%s)", GCStrings::getString(53).c_str(), p->moonrise.hour
						, p->moonrise.min, GCStrings::GetDSTSignature(p->nDST));
				}
				str += str2;
				str += "\r\n";
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (p->moonrise.hour < 0)
					str2 = GCStrings::getString(137);
				else
				{
					str2.Format("%s %d:%02d (%s)", GCStrings::getString(54).c_str(), p->moonset.hour
						, p->moonset.min, GCStrings::GetDSTSignature(p->nDST));
				}
				str += str2;
				str += "\r\n";
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s: %f (*)\r\n", GCStrings::getString(100).c_str(), p->astrodata.sun.longitude_deg);
				str += str2;
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s: %f (*)\r\n", GCStrings::getString(101).c_str(), p->astrodata.moon.longitude_deg);
				str += str2;
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s %f (%s) (*)\r\n", GCStrings::getString(102).c_str(), p->astrodata.msAyanamsa, GCAyanamsha::GetAyanamsaName(GCAyanamsha::GetAyanamsaType()));
				str += str2;
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s %f (*)\r\n", GCStrings::getString(103).c_str(), p->astrodata.jdate);
				str += str2;
			}

	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GCDisplaySettings::getValue(45))
	{
		tdA = p->astrodata.sun.rise;
		tdB = p->astrodata.sun.rise;
		tdA -= 96;
		tdB -= 48;
		str2.Format("\r\nBrahma Muhurta %2d:%02d - %2d:%02d (%s)", 
			tdA.hour, tdA.min, tdB.hour, tdB.min, GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(29))
	{
		str2.Format("\r\n%s %2d:%02d ",
			GCStrings::getString(51).c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.rise;
			tdB = p->astrodata.sun.rise;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(30))
	{
		str2.Format("%s    %2d:%02d ", GCStrings::getString(857).c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.noon;
			tdB = p->astrodata.sun.noon;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(31))
	{
		str2.Format("%s  %2d:%02d ", GCStrings::getString(52).c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.set;
			tdB = p->astrodata.sun.set;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(33))
	{
		str2.Format("\r\n%s info\r\n   Moon in %s %s",
			GCStrings::getString(51).c_str(), 
			GCStrings::GetNaksatraName(p->astrodata.nNaksatra), GCStrings::getString(15).c_str());
		str += str2;
		if (GCDisplaySettings::getValue(47))
		{
			str2.Format(", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, GCStrings::getString(811+int(p->astrodata.nNaksatraElapse/25)).c_str());
			str += str2;
		}
		if (GCDisplaySettings::getValue(46))
		{
			str2.Format(", Moon in %s %s", GCStrings::GetSankrantiName(p->astrodata.nMoonRasi), GCStrings::getString(105).c_str());
			str += str2;
		}
		str2.Format(", %s %s\r\n   Sun in %s %s.\r\n",
			GCStrings::GetYogaName(p->astrodata.nYoga), GCStrings::getString(104).c_str(),
			GCStrings::GetSankrantiName(p->astrodata.nSunRasi), GCStrings::getString(105).c_str());
		str += str2;
	}



	AddNoteText(str);

	/* END GCAL 1.4.3 */

}

void FormatTodayInfoRtf(VCTIME vc, CLocationRef & loc, TString &str)
{
	TString str2, str3, str4;
	TString str5;

	int k = 0, nFestClass;

	TResultCalendar db;

	VCTIME vc2 = vc;
	vc2.tzone = loc.m_fTimezone;
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	db.CalculateCalendar(loc, vc2, 9);

	int i = db.FindDate(vc);
	VAISNAVADAY * p = db.GetDay(i);

	if (p == NULL)
		return;

	str = "";
	GCStringBuilder::AppendRtfHeader(&str);
	VCTIME::GetDateTextWithTodayExt(str3, vc);
	str2.Format("\\f2\\fs%d %s ", GCLayoutData::textSizeH1, str3.c_str());
	str += str2;

	str2.Format("\\par\\f2\\fs%d {\\fs%d %s}\\line %s (%s, %s, Timezone: %s)\\par\r\n\\par\r\n  %s, %s %s\\par\r\n  %s %s, %d Gaurabda\\par\r\n\\par\r\n",
		GCLayoutData::textSizeText, GCLayoutData::textSizeText+4, GCStrings::getString(p->date.dayOfWeek).c_str(),
		loc.m_strName.c_str(), EARTHDATA::GetTextLatitude(loc.m_fLatitude), EARTHDATA::GetTextLongitude(loc.m_fLongitude),
		TTimeZone::GetTimeZoneName(loc.m_nDST),
		GCStrings::GetTithiName(p->astrodata.nTithi), GCStrings::GetPaksaName(p->astrodata.nPaksa), GCStrings::getString(20).c_str(), 
		GCStrings::GetMasaName(p->astrodata.nMasa), GCStrings::getString(22).c_str(), p->astrodata.nGaurabdaYear);

	str += str2;

	if (p->ekadasi_parana)
	{
		p->GetTextEP(str2);
		str2.Delete(0, 14);
		str += str2;
		str += "\\par\r\n";
	}

	// adding mahadvadasi
	// adding spec festivals

	if (p->festivals)
	{
		int i = p->GetHeadFestival();
		while(p->GetNextFestival(i, str2))
		{
			if (str2.GetLength() > 1)
			{
				nFestClass = p->GetFestivalClass(str2);
				if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
				{
					str += "\\tab ";
					str += str2;
					str += "\\par\r\n";
				}
			}
		}
	}

	str += "\\par\r\n";

	if (GCDisplaySettings::getValue(16) == 1 && p->sankranti_zodiac >= 0)
	{
		//double h1, m1;
		//m1 = modf(p->sankranti_day.shour*24, &h1);
		str2.Format(" %s %s (%s %s %s %d %s, %02d:%02d %s) "
			, GCStrings::GetSankrantiName(p->sankranti_zodiac)
			, GCStrings::getString(56).c_str()
			, GCStrings::getString(111).c_str(), GCStrings::GetSankrantiNameEn(p->sankranti_zodiac)
			, GCStrings::getString(852).c_str()
			, p->sankranti_day.day, GCStrings::GetMonthAbreviation(p->sankranti_day.month)
			, p->sankranti_day.GetHour(), p->sankranti_day.GetMinuteRound()
			, GCStrings::GetDSTSignature(p->nDST));
		int length = str2.GetLength();
		int seplen = _tcslen(gpszSeparator);
		length = (seplen - length) / 2;
		str += (gpszSeparator + (seplen - length));
		str += str2;
		str += (gpszSeparator + (seplen - length));
		str += "\\par\r\n";
	}

	if (GCDisplaySettings::getValue(7) == 1 && p->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
	{
		double h, m;
		VCTIME dd;
		m = modf(p->ksaya_time1*24, &h);
		dd = p->date;
		if (p->ksaya_day1 < 0.0)
			dd.PreviousDay();
		str2.Format("%d %s, %02d:%02d", dd.day, GCStrings::GetMonthAbreviation(dd.month), int(h), int(m*60));

//				if (pvd->ksaya_time2 < 0.0)
//					m = modf(fabs(1.0 + pvd->ksaya_time2)*24, &h);
//				else
		m = modf(p->ksaya_time2*24, &h);
		dd = p->date;
		if (p->ksaya_day2 < 0.0)
			dd.PreviousDay();
		str3.Format("%d %s, %02d:%02d", dd.day, GCStrings::GetMonthAbreviation(dd.month), int(h), int(m*60));

		str4.Format("%s %s %s %s %s (%s)\\par\r\n", GCStrings::getString(89).c_str(), GCStrings::getString(850).c_str(), str2.c_str(), GCStrings::getString(851).c_str(), str3.c_str(), GCStrings::GetDSTSignature(p->nDST));
		str += str4;
	}
	// adding fasting
	if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
	{
		if (p->is_vriddhi)
		{
			str += GCStrings::getString(90);
			str += "\\par\r\n";
		}
	}


	if (p->nCaturmasya & CMASYA_PURN_MASK)
	{
		str2.Format("%s [PURNIMA SYSTEM]\\par\r\n"
			, GCStrings::getString(107 + (p->nCaturmasya & CMASYA_PURN_MASK_DAY)
					   + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str();
		}
	}

	if (p->nCaturmasya & CMASYA_PRAT_MASK)
	{
		str2.Format("%s [PRATIPAT SYSTEM]\\par\r\n"
			, GCStrings::getString(107 + ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					   + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str();
			str += "\\par\r\n";
		}
	}

	if (p->nCaturmasya & CMASYA_EKAD_MASK)
	{
		str2.Format("%s [EKADASI SYSTEM]\\par\r\n"
			, GCStrings::getString(107 + ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					   + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
			);
		str += str2;
		if ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
		{
			str += GCStrings::getString(110 + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str();
			str += "\\par\r\n";
		}
	}
			// tithi at arunodaya
			if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
			{
				str2.Format("%s: %s\\par\r\n", GCStrings::getString(98).c_str(), GCStrings::GetTithiName(p->astrodata.nTithiArunodaya));
				str += str2;
			}

			//"Arunodaya Time",//1
			if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
			{
				str2.Format("%s %d:%02d (%s)\\par\r\n", GCStrings::getString(99).c_str(), p->astrodata.sun.arunodaya.hour
					, p->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(p->nDST));
				str += str2;
			}

			//"Moonrise Time",//4
			if (GCDisplaySettings::getValue(4) == 1)
			{
				if (p->moonrise.hour < 0)
					str2 = GCStrings::getString(136);
				else
				{
					str2.Format("%s %d:%02d (%s)", GCStrings::getString(53).c_str(), p->moonrise.hour
						, p->moonrise.min, GCStrings::GetDSTSignature(p->nDST));
				}
				str += str2;
				str += "\\par\r\n";
			}
			//"Moonset Time",//5
			if (GCDisplaySettings::getValue(5) == 1)
			{
				if (p->moonrise.hour < 0)
					str2 = GCStrings::getString(137);
				else
				{
					str2.Format("%s %d:%02d (%s)", GCStrings::getString(54).c_str(), p->moonset.hour
						, p->moonset.min, GCStrings::GetDSTSignature(p->nDST));
				}
				str += str2;
				str += "\\par\r\n";
			}
			///"Sun Longitude",//9
			if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s: %f (*)\\par\r\n", GCStrings::getString(100).c_str(), p->astrodata.sun.longitude_deg);
				str += str2;
			}
			//"Moon Longitude",//10
			if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s: %f (*)\\par\r\n", GCStrings::getString(101).c_str(), p->astrodata.moon.longitude_deg);
				str += str2;
			}
			//"Ayanamsha value",//11
			if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s %f (%s) (*)\\par\r\n", GCStrings::getString(102).c_str(), p->astrodata.msAyanamsa, GCAyanamsha::GetAyanamsaName(GCAyanamsha::GetAyanamsaType()));
				str += str2;
			}
			//"Julian Day",//12
			if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
			{
				str2.Format("%s %f (*)\\par\r\n", GCStrings::getString(103).c_str(), p->astrodata.jdate);
				str += str2;
			}

	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GCDisplaySettings::getValue(45))
	{
		tdA = p->astrodata.sun.rise;
		tdB = p->astrodata.sun.rise;
		tdA -= 96;
		tdB -= 48;
		str2.Format("\\par\r\nBrahma Muhurta %2d:%02d - %2d:%02d (%s)", 
			tdA.hour, tdA.min, tdB.hour, tdB.min, GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(29))
	{
		str2.Format("\\par\r\n%s %2d:%02d ",
			GCStrings::getString(51).c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.rise;
			tdB = p->astrodata.sun.rise;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(30))
	{
		str2.Format("%s    %2d:%02d ", GCStrings::getString(857).c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.noon;
			tdB = p->astrodata.sun.noon;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(31))
	{
		str2.Format("%s  %2d:%02d ", GCStrings::getString(52).c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA = p->astrodata.sun.set;
			tdB = p->astrodata.sun.set;
			tdA -= 24;
			tdB += 24;
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(33))
	{
		str2.Format("\\par\r\n%s info\\par\r\n   Moon in %s %s",
			GCStrings::getString(51).c_str(), 
			GCStrings::GetNaksatraName(p->astrodata.nNaksatra), GCStrings::getString(15).c_str());
		str += str2;
		if (GCDisplaySettings::getValue(47))
		{
			str2.Format(", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, GCStrings::getString(811+int(p->astrodata.nNaksatraElapse/25)).c_str());
			str += str2;
		}
		if (GCDisplaySettings::getValue(46))
		{
			str2.Format(", Moon in %s %s", GCStrings::GetSankrantiName(p->astrodata.nMoonRasi), GCStrings::getString(105).c_str());
			str += str2;
		}
		str2.Format(", %s %s\\par\r\n   Sun in %s %s,\\par\r\n",
			GCStrings::GetYogaName(p->astrodata.nYoga), GCStrings::getString(104).c_str(),
			GCStrings::GetSankrantiName(p->astrodata.nSunRasi), GCStrings::getString(105).c_str());
		str += str2;
	}
	/* END GCAL 1.4.3 */
		MOONDATA md;
	VCTIME vct;
	vct.shour = 0.0;
	EARTHDATA ed = (EARTHDATA)loc;
	double jd;
	str += "\\par \n--------moon---------------\n\n \\par\\par ";
	for(int i = 0; i <= 48; i++)
	{
		vct.shour = i / 48.0;
		jd = vct.GetJulianComplete();
		md.Calculate(vct.GetJulianComplete(), ed);
		str2.Format("Hour %.1f      rektas: %.3f  long: %.3f  st:%.3f\n horz: %.3f\\par ", i/2.0, md.rektaszension, md.longitude_deg,
			EARTHDATA::star_time(jd), put_in_360(EARTHDATA::star_time(jd) - ed.longitude_deg - 90)/30);
		str += str2;
	}
	str += "\n\n \\par";

	AddNoteRtf(str);
}





int FormatCalendarDayRtf(VAISNAVADAY * pvd, TString &dayText) 
{
	TString str, str2, str3;
	int nFestClass;

		dayText.Empty();
		/* BEGIN GCAL 1.4.3 */
		pvd->GetTextRtf(str, GCDisplaySettings::getValue(39), GCDisplaySettings::getValue(36), GCDisplaySettings::getValue(37), GCDisplaySettings::getValue(38), GCDisplaySettings::getValue(41));
		/* END GCAL 1.4.3 */
		//str.Mid(16, str.GetLength(), str2);
		//str.Left(15, str3);
		//str = str3;
		if (pvd->astrodata.sun.longitude_deg < 0.0)
		{
			dayText += "\\par\\tab No rise and no set of the sun. No calendar information.";
			return 1;
		}
		dayText += str;
//		AddListRtf(dayText, str, str2);

		if (GCDisplaySettings::getValue(17) == 1)
		{
			if (pvd->ekadasi_parana)
			{
				pvd->GetTextEP(str);
				str.Delete(0, 17);
				AddListRtf(dayText, str);
			}
		}

		if (GCDisplaySettings::getValue(6) == 1)
		{
			if (pvd->festivals.GetLength() > 0)
			{
				int i = pvd->GetHeadFestival();
				while(pvd->GetNextFestival(i, str2))
				{
					if (str2.GetLength() > 1)
					{
						nFestClass = pvd->GetFestivalClass(str2);
						if (nFestClass < 0 || GCDisplaySettings::getValue(22 + nFestClass) == 1)
							AddListRtf(dayText, str2);
					}
				}
			}
		}

		if (GCDisplaySettings::getValue(16) == 1 && pvd->sankranti_zodiac >= 0)
		{
			//double h1, m1;
			//m1 = modf(pvd->sankranti_day.shour*24, &h1);
			str.Format("{\\i %s %s (%s %s %s %d %s, %02d:%02d %s) }"
				, GCStrings::GetSankrantiName(pvd->sankranti_zodiac)
				, GCStrings::getString(56).c_str()
				, GCStrings::getString(111).c_str(), GCStrings::GetSankrantiNameEn(pvd->sankranti_zodiac)
				, GCStrings::getString(852).c_str()
				, pvd->sankranti_day.day, GCStrings::GetMonthAbreviation(pvd->sankranti_day.month)
				, pvd->sankranti_day.GetHour(), pvd->sankranti_day.GetMinuteRound()
				, GCStrings::GetDSTSignature(pvd->nDST));
			int length = str.GetLength();
			int seplen = _tcslen(gpszSeparator);
			length = (seplen - length) / 2;
			dayText += "\\par ";
			dayText += (gpszSeparator + (seplen - length));
			dayText += str;
			dayText += (gpszSeparator + (seplen - length));
			//dayText += "\\par\r\n";
		}

		if (GCDisplaySettings::getValue(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
		{
			double h, m;
			VCTIME ksayaDate;

			// zaciatok ksaya tithi
			m = modf(pvd->ksaya_time1*24, &h);
			ksayaDate = pvd->date;
			if (pvd->ksaya_day1 < 0.0)
				ksayaDate.PreviousDay();
			str2.Format("%d %s %02d:%02d", ksayaDate.day, GCStrings::GetMonthAbreviation(ksayaDate.month), int(h), int(m*60));

			// end of ksaya tithi
			m = modf(pvd->ksaya_time2*24, &h);
			ksayaDate = pvd->date;
			if (pvd->ksaya_day2 < 0.0)
				ksayaDate.PreviousDay();
			str3.Format("%d %s %02d:%02d", ksayaDate.day, GCStrings::GetMonthAbreviation(ksayaDate.month), int(h), int(m*60));

			// print info
			str.Format("%s: %s -- %s %s %s (%s)", GCStrings::getString(89).c_str(), GCStrings::GetTithiName((pvd->astrodata.nTithi + 29)%30), str2.c_str(), GCStrings::getString(851).c_str(), str3.c_str(), GCStrings::GetDSTSignature(pvd->nDST));
			AddListRtf(dayText, str);
		}

		if (GCDisplaySettings::getValue(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
		{
			if (pvd->is_vriddhi)
				AddListRtf(dayText, GCStrings::getString(90));
		}

		if (pvd->nCaturmasya & CMASYA_CONT_MASK)
		{
			int n = ((pvd->nCaturmasya & CMASYA_CONT_MASK) >> 22);
			AddListRtf(dayText, GCStrings::getString( 111 + n ));
		}

		if ((GCDisplaySettings::getValue(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
		{
			str.Format("%s [PURNIMA SYSTEM]"
				, GCStrings::getString(107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
					       + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str()
				);
			AddListRtf(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
			{
				AddListRtf(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)).c_str() );
			}
		}

		if ((GCDisplaySettings::getValue(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
		{
			str.Format("%s [PRATIPAT SYSTEM]"
				, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					       + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str()
				);
			AddListRtf(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
			{
				AddListRtf(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)).c_str() );
			}
		}

		if ((GCDisplaySettings::getValue(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
		{
			str.Format("%s [EKADASI SYSTEM]"
				, GCStrings::getString(107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					       + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str()
				);
			AddListRtf(dayText, str);
			if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
			{
				AddListRtf(dayText, GCStrings::getString(110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)).c_str() );
			}
		}

		// tithi at arunodaya
		if (GCDisplaySettings::getValue(0) == 1)//m_dshow.m_tithi_arun)
		{
			str.Format("%s: %s", GCStrings::getString(98).c_str(), GCStrings::GetTithiName(pvd->astrodata.nTithiArunodaya));
			AddListRtf(dayText, str);
		}

		//"Arunodaya Time",//1
		if (GCDisplaySettings::getValue(1) == 1)//m_dshow.m_arunodaya)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(99).c_str(), pvd->astrodata.sun.arunodaya.hour
				, pvd->astrodata.sun.arunodaya.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListRtf(dayText, str);
		}
		//"Sunrise Time",//2
		//"Sunset Time",//3
		if (GCDisplaySettings::getValue(2) == 1)//m_dshow.m_sunrise)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(51).c_str(), pvd->astrodata.sun.rise.hour
				, pvd->astrodata.sun.rise.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListRtf(dayText, str);
		}
		/* BEGIN GCAL 1.4.3 */
		// Noon time
		if (GCDisplaySettings::getValue(34) == 1)//m_dshow.m_sunset)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(857).c_str(), pvd->astrodata.sun.noon.hour
				, pvd->astrodata.sun.noon.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListRtf(dayText, str);
		}
		/* END GCAL 1.4.3 */
		if (GCDisplaySettings::getValue(3) == 1)//m_dshow.m_sunset)
		{
			str.Format("%s %d:%02d (%s)", GCStrings::getString(52).c_str(), pvd->astrodata.sun.set.hour
				, pvd->astrodata.sun.set.min, GCStrings::GetDSTSignature(pvd->nDST));
			AddListRtf(dayText, str);
		}
		//"Moonrise Time",//4
		if (GCDisplaySettings::getValue(4) == 1)
		{
			if (pvd->moonrise.hour < 0)
				str = GCStrings::getString(136).c_str();
			else
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(53).c_str(), pvd->moonrise.hour
					, pvd->moonrise.min, GCStrings::GetDSTSignature(pvd->nDST));
			}
			AddListRtf(dayText, str);
		}
		//"Moonset Time",//5
		if (GCDisplaySettings::getValue(5) == 1)
		{
			if (pvd->moonrise.hour < 0)
				str = GCStrings::getString(137);
			else
			{
				str.Format("%s %d:%02d (%s)", GCStrings::getString(54).c_str(), pvd->moonset.hour
					, pvd->moonset.min, GCStrings::GetDSTSignature(pvd->nDST));
			}
			AddListRtf(dayText, str);
		}
		///"Sun Longitude",//9
		if (GCDisplaySettings::getValue(9) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s: %f (*)", GCStrings::getString(100).c_str(), pvd->astrodata.sun.longitude_deg);
			AddListRtf(dayText, str);
		}
		//"Moon Longitude",//10
		if (GCDisplaySettings::getValue(10) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s: %f (*)", GCStrings::getString(101).c_str(), pvd->astrodata.moon.longitude_deg);
			AddListRtf(dayText, str);
		}
		//"Ayanamsha value",//11
		if (GCDisplaySettings::getValue(11) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s %f (%s) (*)", GCStrings::getString(102).c_str(), pvd->astrodata.msAyanamsa, GCAyanamsha::GetAyanamsaName(GCAyanamsha::GetAyanamsaType()));
			AddListRtf(dayText, str);
		}
		//"Julian Day",//12
		if (GCDisplaySettings::getValue(12) == 1)//m_dshow.m_sun_long)
		{
			str.Format("%s %f (*)", GCStrings::getString(103).c_str(), pvd->astrodata.jdate);
			AddListRtf(dayText, str);
		}

	return 0;
}



/****************************************************************************
*
*
*
*****************************************************************************/

int FormatCalendarRtf(TResultCalendar &daybuff, TString &m_text) 
{
	int k;
	int bShowColumnHeaders = 0;
	TString str, str2, str3, dayText;

	char * spaces = "                                                                                ";
	VAISNAVADAY * pvd, * prevd, *nextd;
	int lastmasa = -1;
	int lastmonth = -1;
//	int tp1;
//	double rate;
	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);

	m_text.Empty();

	GCStringBuilder::AppendRtfHeader(&m_text);

	for (k = 0; k < daybuff.m_vcCount; k++)
	{

		prevd = daybuff.GetDay(k - 1);
		pvd = daybuff.GetDay(k);
		nextd = daybuff.GetDay(k + 1);

		if (pvd)
		{
			bShowColumnHeaders = 0;
			if ((GCDisplaySettings::getValue(18) == 1) && (pvd->astrodata.nMasa != lastmasa))
			{
				if (bShowColumnHeaders == 0)
					m_text += "\\par ";
				bShowColumnHeaders = 1;
//				m_text += "\\par\r\n";
				str.Format("\\par \\pard\\f2\\fs%d\\qc %s %s, Gaurabda %d", GCLayoutData::textSizeH2
					, GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str()
					, pvd->astrodata.nGaurabdaYear);
				if ((pvd->astrodata.nMasa == ADHIKA_MASA) && ((lastmasa >= SRIDHARA_MASA) && (lastmasa <= DAMODARA_MASA)))
				{
					str += "\\line ";
					str += GCStrings::getString(128).c_str();
				}
//				m_text += "\\par ";
//				tp1 = (80 - str.GetLength())/2;
//				str.Insert(0, (spaces + (79 - tp1)));
//				str += spaces;
//				str.Insert(80 - _tcslen(AvcGetVersionText()), );
//				str.Left(80, str3);
				m_text += str;

//				m_text += "\\par\r\n\\par\r\n";
				/* BEGIN GCAL 1.4.3 */
				lastmasa = pvd->astrodata.nMasa;
			}

			if ((GCDisplaySettings::getValue(19) == 1) && (pvd->date.month != lastmonth))
			{
				if (bShowColumnHeaders == 0)
					m_text += "\\par ";
				bShowColumnHeaders = 1;
				str.Format("\\par\\pard\\f2\\qc\\fs%d\r\n", GCLayoutData::textSizeH2);
				m_text += str;
				str.Format("%s %d", GCStrings::getString(759 + pvd->date.month).c_str(), pvd->date.year);
				//tp1 = (80 - str.GetLength())/2;
				//str.Insert(0, (spaces + (79 - tp1)));
				//str += spaces;
				//str.Insert(80 - _tcslen(AvcGetVersionText()), AvcGetVersionText());
				//str.Left(80, str3);
				m_text += str;
//				m_text += "\\par\r\n";
//				m_text += (spaces + (79 - (80 - daybuff.m_Location.m_strFullName.GetLength())/2));
//				m_text += daybuff.m_Location.m_strFullName;
				lastmonth = pvd->date.month;
			}

			// print location text
			if (bShowColumnHeaders)
			{
//				m_text += "\\par\r\n";
				m_text += "\\par\\pard\\qc\\cf2\\fs22 ";
//				m_text += (spaces + (79 - (80 - daybuff.m_Location.m_strFullName.GetLength())/2));
				m_text += daybuff.m_Location.m_strFullName;
//				m_text += "\\par\r\n";
			}

			if (bShowColumnHeaders)
			{
				str.Format("\\par\\pard\\fs%d\\qc %s", GCLayoutData::textSizeNote, GCStrings::GetVersionText());
				m_text += str;
				m_text += "\\par\\par\r\n";
			}


			if (bShowColumnHeaders)
			{
				int tabStop = 5760*GCLayoutData::textSizeText/24;
				str.Format("\\pard\\tx%d\\tx%d ",2000*GCLayoutData::textSizeText/24, tabStop);
				m_text += str;
				if (GCDisplaySettings::getValue(39))
				{
					tabStop += 990*GCLayoutData::textSizeText/24;
					str.Format("\\tx%d", tabStop);
					m_text += str;
				}
				if (GCDisplaySettings::getValue(37))
				{
					tabStop += 1720*GCLayoutData::textSizeText/24;
					str.Format("\\tx%d", tabStop);
					m_text += str;
				}
				if (GCDisplaySettings::getValue(36))
				{
					tabStop += 1800*GCLayoutData::textSizeText/24;
					str.Format("\\tx%d", tabStop);
					m_text += str;
				}
				if (GCDisplaySettings::getValue(38))
				{
					tabStop += 750*GCLayoutData::textSizeText/24;
					str.Format("\\tx%d", tabStop);
					m_text += str;
				}
				if (GCDisplaySettings::getValue(41))
				{
					tabStop += 1850*GCLayoutData::textSizeText/24;
					str.Format("\\tx%d", tabStop);
					m_text += str;
				}
				// paksa width 990
				// yoga width 1720
				// naks width 1800
				// fast width 990
				// rasi width 1850
				m_text += str;
				str.Format("{\\highlight15\\cf7\\fs%d\\b DATE\\tab TITHI", GCLayoutData::textSizeNote);
				m_text += str;
				if (GCDisplaySettings::getValue(39)) {
					m_text += "\\tab PAKSA";
				}
				if (GCDisplaySettings::getValue(37)) {
					m_text += "\\tab YOGA";
				}
				if (GCDisplaySettings::getValue(36)) {
					m_text += "\\tab NAKSATRA";
				}
				if (GCDisplaySettings::getValue(38))
				{
					m_text += "\\tab FAST";
				}
				if (GCDisplaySettings::getValue(41))
				{
					m_text += "\\tab RASI";
				}
				m_text += "}";
				//m_text += "\\par\r\n";
				//m_text += gpszSeparator;
			}
			str.Format("\\fs%d ", GCLayoutData::textSizeText);
			m_text += str;
//			m_text += "\\fs24 ";

			FormatCalendarDayRtf(pvd, dayText);

			if (GCDisplaySettings::getValue(21) == 1)
			{
				if (prevd != NULL)
				{
					if (prevd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(780).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						AddListRtf(dayText, str);
					}
				}
				if (nextd != NULL)
				{
					if (nextd->astrodata.nMasa != pvd->astrodata.nMasa)
					{
						str.Format("%s %s %s", GCStrings::getString(781).c_str(), GCStrings::GetMasaName(pvd->astrodata.nMasa), GCStrings::getString(22).c_str());
						AddListRtf(dayText, str);
					}
				}
			}
			/* BEGIN GCAL 1.4.3 */
			if (GCDisplaySettings::getValue(35) && prevd && nextd)
			{
				if (prevd->nDST == 0 && pvd->nDST==1)
					AddListRtf(dayText, GCStrings::getString(855).c_str());
				else if (pvd->nDST==1 && nextd->nDST==0)
					AddListRtf(dayText, GCStrings::getString(856).c_str());
			}

			/* END GCAL 1.4.3 */

			if (GCDisplaySettings::getValue(20) == 0)
				m_text += dayText;
			else if (dayText.GetLength() > 90)
				m_text += dayText;


		}
	}

	AddNoteRtf(m_text);

	m_text += "\r\n}\r\n";

	return 1;
}

