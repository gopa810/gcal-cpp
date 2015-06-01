#include "stdafx.h"

#include "LocationRef.h"
#include "XmlFile.h"
#include "level_4.h"
#include "strings.h"
#include "vedic_ui.h"
#include "level_5_days.h"
#include "TTimeZone.h"
#include "TResultApp.h"
#include "TResultEvent.h"
#include "TResultMasaList.h"
// PORTABLE

int WriteXML_FirstDay_Year(FILE * fout, CLocationRef & loc, VCTIME vcStart)
{
	TFileXml xml;

	xml = fout;

	vcStart = GetFirstDayOfYear((EARTHDATA)loc, vcStart.year);
	vcStart.InitWeekDay();

	// write
	xml << "<xml>\n";
	xml << "\t<request name=\"FirstDay\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"year\" val=\""<< vcStart.year << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"FirstDay_of_GaurabdaYear\">\n";
	xml << "\t\t<firstday date=\"" << vcStart 
		<< "\" dayweekid = \"" << vcStart.dayOfWeek << "\" dayweek=\"" << gstr[vcStart.dayOfWeek] << "\" />\n";
	xml << "\t</result>\n";
	xml << "</xml>\n";

	return 0;
}

int WriteXML_Sankrantis(FILE * fout, CLocationRef & loc, VCTIME vcStart, VCTIME vcEnd)
{
	VCTIME d;
	DAYTIME dt;
	int zodiac;

	TFileXml xml;

	// open file
	xml = fout;

	d = vcStart;

	xml << "<xml>\n";
	xml << "\t<request name=\"Sankranti\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << loc.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"location\" val=\"" << loc.m_strName << "\" />\n";
	xml << "\t\t<arg name=\"startdate\" val=\""<< vcStart << "\" />\n";
	xml << "\t\t<arg name=\"enddate\" val=\""<< vcEnd << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"SankrantiList\">\n";

	while(d.IsBeforeThis(vcEnd))
	{
		d = GetNextSankranti(d, zodiac);
		d.InitWeekDay();
		xml << "\t\t<sank date=\"" << d << "\" ";
		xml << "dayweekid=\"" << d.dayOfWeek << "\" dayweek=\"" << gstr[d.dayOfWeek] << "\" ";

		dt.SetDegTime( 360 * d.shour );

		xml << " time=\"" << dt << "\" >\n";
		xml << "\t\t<zodiac sans=\"" << GetSankrantiName(zodiac);
		xml << "\" eng=\"" << GetSankrantiNameEn(zodiac) << "\" id=\"" << zodiac << "\" />\n";
		xml << "\t\t</sank>\n\n";

		d.NextDay();
		d.NextDay();
	}

	xml << "\t</result>\n";
	xml << "</xml>";

	return 1;
}

int WriteCalendarXml(TResultCalendar &daybuff, FILE * fout)
{
	int k;
	TString str, st;
	VCTIME date;
	TFileXml xml;
	VAISNAVADAY * pvd;
	int nPrevMasa = -1;
	int nPrevPaksa = -1;

	xml = fout;

	xml << "<xml>\n";
	xml << "\t<request name=\"Calendar\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << daybuff.m_Location.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << daybuff.m_Location.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << daybuff.m_Location.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"startdate\" val=\"" << daybuff.m_vcStart << "\" />\n";
	xml << "\t\t<arg name=\"daycount\" val=\"" << daybuff.m_vcCount << "\" />\n";
	xml << "\t\t<arg name=\"dst\" val=\"" << daybuff.m_Location.m_nDST << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Calendar\">\n";
	if (daybuff.m_Location.m_nDST > 0)
		xml << "\t<dstsystem name=\"" << TTimeZone::GetTimeZoneName(daybuff.m_Location.m_nDST) << "\" />\n";

	for (k = 0; k < daybuff.m_vcCount; k++)
	{
		pvd = daybuff.GetDay(k);
		if (pvd)
		{
			if (nPrevMasa != pvd->astrodata.nMasa)
			{
				if (nPrevMasa != -1)
					xml << "\t</masa>\n";
				xml << "\t<masa name=\"" << GetMasaName(pvd->astrodata.nMasa) << " Masa";
				if (nPrevMasa == ADHIKA_MASA)
					xml << " " << gstr[109];
				xml << "\"";
				xml << " gyear=\"Gaurabda " << pvd->astrodata.nGaurabdaYear << "\"";
				xml << ">\n";
			}

			nPrevMasa = pvd->astrodata.nMasa;

			// date data
			xml << "\t<day date=\"" << pvd->date << "\" dayweekid=\"" << pvd->date.dayOfWeek;
			xml << "\" dayweek=\"";
			gstr[pvd->date.dayOfWeek].Left(2, st);
			xml << st.c_str() << "\">\n";

			// sunrise data
			xml << "\t\t<sunrise time=\"" << pvd->astrodata.sun.rise << "\">\n";

			xml << "\t\t\t<tithi name=\"";
			xml << GetTithiName(pvd->astrodata.nTithi);
			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) 
				|| (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					if (pvd->nMhdType == EV_NULL)
					{
						xml << " " << gstr[58];
					}
					else
					{
						xml << " " << gstr[59];
					}
				}
			}
			str.Format("\" elapse=\"%.1f\" index=\"%d\"/>\n"
				,pvd->astrodata.nTithiElapse, pvd->astrodata.nTithi % 30 + 1 );
			xml << str;

			str.Format("\t\t\t<naksatra name=\"%s\" elapse=\"%.1f\" />\n"
				,GetNaksatraName(pvd->astrodata.nNaksatra), pvd->astrodata.nNaksatraElapse );
			xml << str;

			str.Format("\t\t\t<yoga name=\"%s\" />\n", GetYogaName(pvd->astrodata.nYoga) );
			xml << str;

			str.Format("\t\t\t<paksa id=\"%c\" name=\"%s\"/>\n", GetPaksaChar(pvd->astrodata.nPaksa), GetPaksaName(pvd->astrodata.nPaksa) );
			xml << str;

			xml << "\t\t</sunrise>\n";

			xml << "\t\t<dst offset=\"" << pvd->nDST << "\" />\n";
			// arunodaya data
			xml << "\t\t<arunodaya time=\"" << pvd->astrodata.sun.arunodaya << "\">\n";
			xml << "\t\t\t<tithi name=\"" << GetTithiName(pvd->astrodata.nTithiArunodaya) << "\" />\n";
			xml << "\t\t</arunodaya>\n";

			str.Empty();

			xml << "\t\t<noon time=\"" << pvd->astrodata.sun.noon << "\" />\n";

			xml << "\t\t<sunset time=\"" << pvd->astrodata.sun.set << "\" />\n";

			// moon data
			xml << "\t\t<moon rise=\"" << pvd->moonrise << "\" set=\"" << pvd->moonset << "\" />\n";

			if (pvd->ekadasi_parana)
			{
				double h1, m1, h2, m2;
				m1 = modf(pvd->eparana_time1, &h1);
				if (pvd->eparana_time2 >= 0.0)
				{
					m2 = modf(pvd->eparana_time2, &h2);
					str.Format("\t\t<parana from=\"%02d:%02d\" to=\"%02d:%02d\" />\n", int(h1), int(m1*60), int(h2), int(m2*60));
				}
				else
				{
					str.Format("\t\t<parana after=\"%02d:%02d\" />\n", int(h1), int(m1*60) );
				}
				xml << str;
			}
			str.Empty();

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
						str.Format("\t\t<festival name=\"%s\" class=\"%d\"/>\n", str2.c_str(), nFestClass);
						xml << str;
					}
				}
			}

			if (pvd->nFastType != FAST_NULL)
			{
				if (pvd->nFastType == FAST_EKADASI)
				{
					xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
				}
				else
				{
					xml << "\t\t<fast type=\"\" mark=\"\" />\n";
				}
			}

			if (pvd->sankranti_zodiac >= 0)
			{
				//double h1, m1, s1;
				//m1 = modf(pvd->sankranti_day.shour*24, &h1);
//				s1 = modf(m1*60, &m1);
				str.Format("\t\t<sankranti rasi=\"%s\" time=\"%02d:%02d:%02d\" />\n"
					, GetSankrantiName(pvd->sankranti_zodiac), pvd->sankranti_day.GetHour()
					, pvd->sankranti_day.GetMinute(), pvd->sankranti_day.GetSecond());
				xml << str;
			}

			if (pvd->was_ksaya)
			{
				double h1, m1, h2, m2;
				m1 = modf(pvd->ksaya_time1*24, &h1);
				m2 = modf(pvd->ksaya_time2*24, &h2);
				str.Format("\t\t<ksaya from=\"%02d:%02d\" to=\"%02d:%02d\" />\n", int(h1), abs(int(m1*60)), int(h2), abs(int(m2*60)));
				xml << str;
			}

			if (pvd->is_vriddhi)
			{
				xml << "\t\t<vriddhi sd=\"yes\" />\n";
			}
			else
			{
				xml << "\t\t<vriddhi sd=\"no\" />\n";
			}

			if (pvd->nCaturmasya & CMASYA_PURN_MASK)
			{
				xml << "\t\t<caturmasya day=\"" 
					<< (((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)) > 1 ? "last" : "first")
					<< "\" month=\"" 
					<< int((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 4) 
					<< "\" system=\"PURNIMA\" />\n";
			}

			if (pvd->nCaturmasya & CMASYA_PRAT_MASK)
			{
				xml << "\t\t<caturmasya day=\"" 
					<< (((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8) > 1 ? "last" : "first")
					<< "\" month=\"" 
					<< int((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 12)
					<< "\" system=\"PRATIPAT\" />\n";
			}

			if (pvd->nCaturmasya & CMASYA_EKAD_MASK)
			{
				str.Format("\t<caturmasya day=\"%s\" month=\"%d\" system=\"EKADASI\" />\n"
					, ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16) > 1 ? "last" : "first"
					, int((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 20));
				xml << str;
			}

			xml << "\t</day>\n\n";

		}
		date.shour = 0;
		date.NextDay();
	}
	xml << "\t</masa>\n";

	
	xml << "</result>\n" << "</xml>\n";

	return 1;
}

int WriteXML_Naksatra(FILE * fout, CLocationRef &loc, VCTIME vc, int nDaysCount)
{
	TString str;
	VCTIME date;

	TFileXml xml;
	xml = fout;

	xml << "<xml>\n";
	xml << "\t<request name=\"Naksatra\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << loc.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"startdate\" val=\"" << vc << "\" />\n";
	xml << "\t\t<arg name=\"daycount\" val=\"" << nDaysCount << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Naksatra\">\n";

	VCTIME d = vc;
	d.tzone = loc.m_fTimezone;
	VCTIME dn;
	DAYTIME dt;
	SUNDATA sun;
	int nak;
	EARTHDATA earth = (EARTHDATA)loc;

	for(int i = 0; i < 30; i++)
	{
		nak = GetNextNaksatra(earth, d, dn);
		d = dn;
		xml << "\t\t<day date=\"" << d << "\">\n";
		//str.Format("%d.%d.%d", d.day, d.month, d.year);
		//n = m_list.InsertItem(50, GetNaksatraName(nak));
		//m_list.SetItemText(n, 1, str);
		xml << "\t\t\t<naksatra id=\"" << nak << "\" name=\"" << GetNaksatraName(nak) << "\"\n";
		dt.SetDegTime( d.shour * 360);
		//time_print(str, dt);
		xml << "\t\t\t\tstarttime=\"" << dt << "\" />\n";
		//m_list.SetItemText(n, 2, str);

		// sunrise time get
		SunCalc(d, earth, sun);
		//time_print(str, sun.rise);
		//m_list.SetItemText(n, 3, str);
		xml << "\t\t\t<sunrise time=\"" << sun.rise << "\" />\n";

		xml << "\t\t</day>\n";
		// increment for non-duplication of naksatra
		d = dn;
		d.shour += 1.0/8.0;
	}


	xml << "\t</result>\n";
	xml << "</xml>\n";


	return 1;
}

int WriteXML_Tithi(FILE * fout, CLocationRef &loc, VCTIME vc)
{
	TString str;
	VCTIME date;

	TFileXml xml;
	xml = fout;

	xml << "<xml>\n";
	xml << "\t<request name=\"Tithi\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << loc.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"startdate\" val=\"" << vc << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Tithi\">\n";

	VCTIME d = vc;
	VCTIME d1, d2;
	d.tzone = loc.m_fTimezone;
	VCTIME dn;
	DAYTIME dt;
	EARTHDATA earth = (EARTHDATA)loc;


	DAYDATA day;

	DayCalc(vc, earth, day);

	d.shour = day.sun.sunrise_deg/360.0 + loc.m_fTimezone/24.0;

	GetPrevTithiStart(earth, d, d1);
	GetNextTithiStart(earth, d, d2);

	{
		dt.SetDegTime( d1.shour * 360 );
		// start tithi at t[0]
		xml << "\t\t<tithi\n\t\t\tid=\"" << day.nTithi << "\"\n";
		xml << "\t\t\tname=\"" << GetTithiName(day.nTithi) << "\"\n";
		xml << "\t\t\tstartdate=\"" << d1 << "\"\n";
		xml << "\t\t\tstarttime=\"" << dt << "\"\n";
	
		dt.SetDegTime( d2.shour * 360 );
		xml << "\t\t\tenddate=\"" << d2 << "\"\n";
		xml << "\t\t\tendtime=\"" << dt << "\"\n />";
	}

	xml << "\t</result>\n";
	xml << "</xml>\n";

	return 1;
}

int WriteXML_GaurabdaTithi(FILE * fout, CLocationRef &loc, VATIME vaStart, VATIME vaEnd)
{
	int gyearA = vaStart.gyear;
	int gyearB = vaEnd.gyear;
	int gmasa = vaStart.masa;
	int gpaksa = vaStart.tithi / 15;
	int gtithi = vaStart.tithi % 15;
	
	if (gyearB < gyearA)
		gyearB = gyearA;

	TFileXml xml;

	xml = fout;

	xml << "<xml>\n";
	xml << "\t<request name=\"Tithi\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << loc.m_fTimezone << "\" />\n";
	if (gyearA > 1500)
	{
		xml << "\t\t<arg name=\"year-start\" val=\"" << gyearA << "\" />\n";
		xml << "\t\t<arg name=\"year-end\" val=\"" << gyearB << "\" />\n";
	}
	else
	{
		xml << "\t\t<arg name=\"gaurabdayear-start\" val=\"" << gyearA << "\" />\n";
		xml << "\t\t<arg name=\"gaurabdayear-end\" val=\"" << gyearB << "\" />\n";
	}
	xml << "\t\t<arg name=\"masa\" val=\"" << gmasa << "\" />\n";
	xml << "\t\t<arg name=\"paksa\" val=\"" << gpaksa << "\" />\n";
	xml << "\t\t<arg name=\"tithi\" val=\"" << gtithi << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Tithi\">\n";


	EARTHDATA earth = (EARTHDATA)loc;
	VCTIME vcs, vce, today;
	SUNDATA sun;
	int A, B;
	double sunrise;
	DAYDATA day;
	int oTithi, oPaksa, oMasa, oYear;

	if (gyearA > 1500)
	{
		A = gyearA - 1487;
		B = gyearB - 1485;
	}
	else
	{
		A = gyearA;
		B = gyearB;
	}

	for(; A <= B; A++)
	{
		vcs = CalcTithiEnd(A, gmasa, gpaksa, gtithi, earth, vce);
		if (gyearA > 1500)
		{
			if ((vcs.year < gyearA) || (vcs.year > gyearB))
				continue;
		}
		oTithi = gpaksa*15 + gtithi;
		oMasa = gmasa;
		oPaksa = gpaksa;
		oYear = 0;
		xml << "\t<celebration\n";
//		xml << "\t\t<tithi\n";
		xml << "\t\trtithi=\"" << GetTithiName(oTithi) << "\"\n";
		xml << "\t\trmasa=\"" << GetMasaName(oMasa) << "\"\n";
		xml << "\t\trpaksa=\"" << (oPaksa ? _T("Gaura") : _T("Krsna")) << "\"\n";
		// test ci je ksaya
		today = vcs;
		today.shour = 0.5;
		SunCalc(today, earth, sun);
		sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
		if (sunrise < vcs.shour)
		{
			today = vce;
			SunCalc(today, earth, sun);
			sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
			if (sunrise < vce.shour)
			{
				// normal type
				vcs.NextDay();
				xml << "\t\ttype=\"normal\"\n";
			}
			else
			{
				// ksaya
				vcs.NextDay();
				DayCalc(vcs, earth, day);
				oTithi = day.nTithi;
				oPaksa = day.nPaksa;
				oMasa = MasaCalc(vcs, day, earth, oYear);
				xml << "\t\ttype=\"ksaya\"\n";
			}
		}
		else
		{
			// normal, alebo prvy den vriddhi
			today = vce;
			SunCalc(today, earth, sun);
			if ((sun.sunrise_deg + loc.m_fTimezone*15.0)/360 < vce.shour)
			{
				// first day of vriddhi type
				xml << "\t\ttype=\"vriddhi\"\n";
			}
			else
			{
				// normal
				xml << "\t\ttype=\"normal\"\n";
			}
		}
		xml << "\t\tdate=\"" << vcs << "\"\n";
		xml << "\t\totithi=\"" << GetTithiName(oTithi) << "\"\n";
		xml << "\t\tomasa=\"" << GetMasaName(oMasa) << "\"\n";
		xml << "\t\topaksa=\"" << (oPaksa ? _T("Gaura") : _T("Krsna")) << "\"\n";
		xml << "\t/>\n";
//		xml << "\t\t</celebration>\n";

	}

/*	xml << "\t\t<tithi\n\t\t\tid=\"" << int(gpaksa*15 + gtithi) << "\"\n";
	xml << "\t\t\tname=\"" << GetTithiName(gpaksa*15 + gtithi) << "\"\n";
	xml << "\t\t\tstartdate=\"" << vcs << "\"\n";
	dt.SetDegTime(360.0 * vcs.shour);
	xml << "\t\t\tstarttime=\"" << dt << "\"\n";
	xml << "\t\t\tenddate=\"" << vce << "\"\n";
	dt.SetDegTime(360.0 * vce.shour);
	xml << "\t\t\tendtime=\"" << dt << "\"\n";
	xml << "\t\t\tmasa=\"" << GetMasaName(gmasa) << "\"\n";
	xml << "\t\t\tpaksa=\"" << (gpaksa ? _T("Gaura") : _T("Krsna")) << "\"\n";
	xml << "\t\t/>\n";
*/
	xml << "\t</result>\n";
	xml << "</xml>\n";

	return 1;
}

int WriteXML_GaurabdaNextTithi(FILE * fout, CLocationRef &loc, VCTIME vcStart, VATIME vaStart)
{
	int gmasa, gpaksa, gtithi;
	TFileXml xml;

	gmasa = vaStart.masa;
	gpaksa = vaStart.tithi / 15;
	gtithi = vaStart.tithi % 15;

	xml = fout;

	xml << "<xml>\n";
	xml << "\t<request name=\"Tithi\" version=\"" << gstr[130].c_str() << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << loc.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << loc.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << loc.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"start date\" val=\"" << vcStart << "\" />\n";
	xml << "\t\t<arg name=\"masa\" val=\"" << gmasa << "\" />\n";
	xml << "\t\t<arg name=\"paksa\" val=\"" << gpaksa << "\" />\n";
	xml << "\t\t<arg name=\"tithi\" val=\"" << gtithi << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Tithi\">\n";

	EARTHDATA earth = (EARTHDATA)loc;
	VCTIME vcs, vce, today;
	SUNDATA sun;
	int A;
	double sunrise;
	DAYDATA day;
	int oTithi, oPaksa, oMasa, oYear;

	today = vcStart;
	today.PreviousDay();
	vcStart -= 15;
	for(A = 0; A <= 3; A++)
	{
		vcs = CalcTithiEndEx(vcStart, 0, gmasa, gpaksa, gtithi, earth, vce);
		if (!vcs.IsBeforeThis(today))
		{
			oTithi = gpaksa*15 + gtithi;
			oMasa = gmasa;
			oPaksa = gpaksa;
			oYear = 0;
			xml << "\t<celebration\n";
	//		xml << "\t\t<tithi\n";
			xml << "\t\trtithi=\"" << GetTithiName(oTithi) << "\"\n";
			xml << "\t\trmasa=\"" << GetMasaName(oMasa) << "\"\n";
			xml << "\t\trpaksa=\"" << (oPaksa ? _T("Gaura") : _T("Krsna")) << "\"\n";
			// test ci je ksaya
			today = vcs;
			today.shour = 0.5;
			SunCalc(today, earth, sun);
			sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
			if (sunrise < vcs.shour)
			{
				today = vce;
				SunCalc(today, earth, sun);
				sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
				if (sunrise < vce.shour)
				{
					// normal type
					vcs.NextDay();
					xml << "\t\ttype=\"normal\"\n";
				}
				else
				{
					// ksaya
					vcs.NextDay();
					DayCalc(vcs, earth, day);
					oTithi = day.nTithi;
					oPaksa = day.nPaksa;
					oMasa = MasaCalc(vcs, day, earth, oYear);
					xml << "\t\ttype=\"ksaya\"\n";
				}
			}
			else
			{
				// normal, alebo prvy den vriddhi
				today = vce;
				SunCalc(today, earth, sun);
				if ((sun.sunrise_deg + loc.m_fTimezone*15.0)/360 < vce.shour)
				{
					// first day of vriddhi type
					xml << "\t\ttype=\"vriddhi\"\n";
				}
				else
				{
					// normal
					xml << "\t\ttype=\"normal\"\n";
				}
			}
			xml << "\t\tdate=\"" << vcs << "\"\n";
			xml << "\t\totithi=\"" << GetTithiName(oTithi) << "\"\n";
			xml << "\t\tomasa=\"" << GetMasaName(oMasa) << "\"\n";
			xml << "\t\topaksa=\"" << (oPaksa ? _T("Gaura") : _T("Krsna")) << "\"\n";
			xml << "\t/>\n";
			break;
		}
		else
		{
			vcStart = vcs;
			vcs.NextDay();
		}
	}

	xml << "\t</result>\n";
	xml << "</xml>\n";


	return 1;
}

