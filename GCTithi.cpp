#include "stdafx.h"
#include "GCTithi.h"
#include "GCMoonData.h"
#include "GCSunData.h"
#include "GCDayData.h"
#include "GCMath.h"
#include "TFileXml.h"
#include "GCStrings.h"

GCTithi::GCTithi(void)
{
}


GCTithi::~GCTithi(void)
{
}

/*********************************************************************/
/*                                                                   */
/*   finds next time when starts next tithi                          */
/*                                                                   */
/*   timezone is not changed                                         */
/*                                                                   */
/*   return value: index of tithi 0..29                              */
/*                 or -1 if failed                                   */
/*********************************************************************/

int GCTithi::GetNextTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate)
{
	double phi = 12.0;
	double l1, l2, sunl, sun2;
	double jday = startDate.GetJulianComplete();
	double xj;
	MOONDATA moon;
	VCTIME d;
	d.Set(startDate);
	VCTIME xd;
	double scan_step = 0.5;
	int prev_tit = 0;
	int new_tit = -1;

	moon.Calculate(jday, ed);
	sunl = SUNDATA::GetSunLongitude(d);
//	SunPosition(d, ed, sun, d.shour - 0.5 + d.tzone/24.0);
	l1 = GCMath::putIn360(moon.longitude_deg - sunl - 180.0);
	prev_tit = int(floor(l1/phi));

	int counter = 0;
	while(counter < 20)
	{
		xj = jday;
		xd.Set(d);

		jday += scan_step;
		d.shour += scan_step;
		if (d.shour > 1.0)
		{
			d.shour -= 1.0;
			d.NextDay();
		}

		moon.Calculate(jday, ed);
		//SunPosition(d, ed, sun, d.shour - 0.5 + d.tzone/24.0);
		//l2 = GCMath::putIn360(moon.longitude_deg - sun.longitude_deg - 180.0);
		sunl = SUNDATA::GetSunLongitude(d);
		l2 = GCMath::putIn360(moon.longitude_deg - sunl - 180.0);
		new_tit = int(floor(l2/phi));

		if (prev_tit != new_tit)
		{
			jday = xj;
			d.Set(xd);
			scan_step *= 0.5;
			counter++;
			continue;
		}
		else
		{
			l1 = l2;
		}
	}
	nextDate.Set(d);
//	nextDate.shour += startDate.tzone / 24.0;
//	nextDate.NormalizeValues();
	return new_tit;
}

/*********************************************************************/
/*                                                                   */
/*   finds previous time when starts next tithi                      */
/*                                                                   */
/*   timezone is not changed                                         */
/*                                                                   */
/*   return value: index of tithi 0..29                              */
/*                 or -1 if failed                                   */
/*********************************************************************/

int GCTithi::GetPrevTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate)
{
	double phi = 12.0;
	double l1, l2, sunl;
	double jday = startDate.GetJulianComplete();
	double xj;
	MOONDATA moon;
	VCTIME d;
	d.Set(startDate);
	VCTIME xd;
	double scan_step = 0.5;
	int prev_tit = 0;
	int new_tit = -1;

	moon.Calculate(jday, ed);
	sunl = SUNDATA::GetSunLongitude(d);
	l1 = GCMath::putIn360(moon.longitude_deg - sunl - 180.0);
	prev_tit = int(floor(l1/phi));

	int counter = 0;
	while(counter < 20)
	{
		xj = jday;
		xd.Set(d);

		jday -= scan_step;
		d.shour -= scan_step;
		if (d.shour < 0.0)
		{
			d.shour += 1.0;
			d.PreviousDay();
		}

		moon.Calculate(jday, ed);
		sunl = SUNDATA::GetSunLongitude(d);
		l2 = GCMath::putIn360(moon.longitude_deg - sunl - 180.0);
		new_tit = int(floor(l2/phi));

		if (prev_tit != new_tit)
		{
			jday = xj;
			d.Set(xd);
			scan_step *= 0.5;
			counter++;
			continue;
		}
		else
		{
			l1 = l2;
		}
	}
	nextDate.Set(d);
//	nextDate.shour += startDate.tzone / 24.0;
//	nextDate.NormalizeValues();
	return new_tit;
}

/*

Routines for calculation begining and ending times of given Tithi

Main function is GetTithiTimes

*/

double GCTithi::GetTithiTimes(EARTHDATA ed, VCTIME vc, double &titBeg, double &titEnd, double sunRise)
{
	VCTIME d1, d2;

	vc.shour = sunRise;
	GCTithi::GetPrevTithiStart(ed, vc, d1);
	GCTithi::GetNextTithiStart(ed, vc, d2);

	titBeg = d1.shour + d1.GetJulian() - vc.GetJulian();
	titEnd = d2.shour + d2.GetJulian() - vc.GetJulian();

	return (titEnd - titBeg);
}

/*********************************************************************/
/* Finds starting and ending time for given tithi                    */
/*                                                                   */
/* tithi is specified by Gaurabda year, masa, paksa and tithi number */
/*      nGYear - 0..9999                                             */
/*       nMasa - 0..12, 0-Madhusudana, 1-Trivikrama, 2-Vamana        */
/*                      3-Sridhara, 4-Hrsikesa, 5-Padmanabha         */
/*                      6-Damodara, 7-Kesava, 8-narayana, 9-Madhava  */
/*                      10-Govinda, 11-Visnu, 12-PurusottamaAdhika   */
/*       nPaksa -       0-Krsna, 1-Gaura                             */
/*       nTithi - 0..14                                              */
/*       earth  - used timezone                                      */
/*                                                                   */
/*********************************************************************/

VCTIME GCTithi::CalcTithiEnd(int nGYear, int nMasa, int nPaksa, int nTithi, EARTHDATA earth, VCTIME & endTithi)
{
	VCTIME d;

	d.Set(DAYDATA::GetFirstDayOfYear(earth, nGYear + 1486));
	d.shour = 0.5;
	d.tzone = earth.tzone;

	return CalcTithiEndEx(d, nGYear, nMasa, nPaksa, nTithi, earth, endTithi);
}

VCTIME GCTithi::CalcTithiEndEx(VCTIME vcStart, int GYear, int nMasa, int nPaksa, int nTithi, EARTHDATA earth, VCTIME & endTithi)
{
	int i, gy, nType;
	VCTIME d, dtemp;
	DAYDATA day;
	int tithi;
	int counter;
	VCTIME start, end;
//	SUNDATA sun;
//	MOONDATA moon;
	double sunrise;
	start.shour = -1.0;
	end.shour = -1.0;
	start.day = start.month = start.year = -1;
	end.day = end.month = end.year = -1;

/*	d = GetFirstDayOfYear(earth, nGYear + 1486);
	d.shour = 0.5;
	d.TimeZone = earth.tzone;
*/
	d.Set(vcStart);

	i = 0;
	do
	{
		d.AddDays(13);
		day.DayCalc(d, earth);
		day.nMasa = day.MasaCalc(d, earth);
		gy = day.nGaurabdaYear;
		i++;
	}
	while(((day.nPaksa != nPaksa) || (day.nMasa != nMasa)) && (i <= 30));

	if (i >= 30)
	{
		d.year = d.month = d.day = -1;
		return d;
	}

	// we found masa and paksa
	// now we have to find tithi
	tithi = nTithi + nPaksa*15;

	if (day.nTithi == tithi)
	{
		// loc1
		// find tithi juncts in this day and according to that times,
		// look in previous or next day for end and start of this tithi
		nType = 1;
	}
	else
	{
		if (day.nTithi < tithi)
		{
			// do increment of date until nTithi == tithi
			//   but if nTithi > tithi
			//       then do decrement of date
			counter = 0;
			while(counter < 30)
			{
				d.NextDay();
				day.DayCalc(d, earth);
				if (day.nTithi == tithi)
					goto cont_2;
				if ((day.nTithi < tithi ) && (day.nPaksa != nPaksa))
				{
					d.PreviousDay();
					goto cont_2;
				}
				if (day.nTithi > tithi)
				{
					d.PreviousDay();
					goto cont_2;
				}
				counter++;
			}
			// somewhere is error
			d.year = d.month = d.day = 0;
			nType = 0;
		}
		else
		{
			// do decrement of date until nTithi <= tithi
			counter = 0;
			while(counter < 30)
			{
				d.PreviousDay();
				day.DayCalc(d, earth);
				if (day.nTithi == tithi)
					goto cont_2;
				if ((day.nTithi > tithi ) && (day.nPaksa != nPaksa))
				{
					goto cont_2;
				}
				if (day.nTithi < tithi)
				{
					goto cont_2;
				}
				counter++;
			}
			// somewhere is error
			d.year = d.month = d.day = 0;
			nType = 0;

		}
cont_2:
		if (day.nTithi == tithi)
		{
			// do the same as in loc1
			nType = 1;
		}
		else 
		{
			// nTithi != tithi and nTithi < tithi
			// but on next day is nTithi > tithi
			// that means we will find start and the end of tithi
			// in this very day or on next day before sunrise
			nType = 2;
		}

	}

	// now we know the type of day-accurancy
	// nType = 0 means, that we dont found a day
	// nType = 1 means, we find day, when tithi was present at sunrise
	// nType = 2 means, we found day, when tithi started after sunrise
	//                  but ended before next sunrise
	//
	sunrise = day.sun.sunrise_deg / 360 + earth.tzone/24;

	if (nType == 1)
	{
		VCTIME d1, d2;
		d.shour = sunrise;
		GCTithi::GetPrevTithiStart(earth, d, d1);
		//d = d1;
		//d.shour += 0.02;
		GCTithi::GetNextTithiStart(earth, d, d2);

		endTithi.Set(d2);
		return d1;
	}
	else if (nType == 2)
	{
		VCTIME d1, d2;
		d.shour = sunrise;
		GCTithi::GetNextTithiStart(earth, d, d1);
		d.Set(d1);
		d.shour += 0.1;
		d.NormalizeValues();
		GCTithi::GetNextTithiStart(earth, d, d2);

		endTithi.Set(d2);
		return d1;
	}

	// if nType == 0, then this algoritmus has some failure
	if (nType == 0)
	{
		d.year = 0;
		d.month = 0;
		d.day = 0;
		d.shour = 0.0;
		endTithi.Set(d);
	}
	else
	{
		d.Set(start);
		endTithi.Set(end);
	}

	return d;
}


/*********************************************************************/
/*  Calculates Date of given Tithi                                   */
/*********************************************************************/

VCTIME GCTithi::CalcTithiDate(int nGYear, int nMasa, int nPaksa, int nTithi, EARTHDATA earth)
{
	int i = 0, gy = 0;
	VCTIME d, dtemp;
	DAYDATA day;
	int tithi = 0;
	int counter = 0;
	unsigned int tmp = 0;

	if (nGYear >= 464 && nGYear < 572)
	{
		tmp = DAYDATA::gGaurBeg[(nGYear-464)*26 + nMasa*2 + nPaksa];
		d.month = (tmp & 0x3e0) >> 5;
		d.day   = (tmp & 0x1f);
		d.year  = (tmp & 0xfffc00) >> 10;
		d.tzone = earth.tzone;
		d.NextDay();

		day.DayCalc(d, earth);
		day.nMasa = day.MasaCalc(d, earth);
		gy = day.nGaurabdaYear;
	}
	else
	{
		//d = GetFirstDayOfYear(earth, nGYear + 1486);
		d.day = 15;
		d.month = 2 + nMasa;
		d.year = nGYear + 1486;
		if (d.month > 12)
		{
			d.month -= 12;
			d.year++;
		}
		d.shour = 0.5;
		d.tzone = earth.tzone;

		i = 0;
		do
		{
			d.AddDays(13);
			day.DayCalc(d, earth);
			day.nMasa = day.MasaCalc(d, earth);
			gy = day.nGaurabdaYear;
			i++;
		}
		while(((day.nPaksa != nPaksa) || (day.nMasa != nMasa)) && (i <= 30));
	}

	if (i >= 30)
	{
		d.year = d.month = d.day = -1;
		return d;
	}

	// we found masa and paksa
	// now we have to find tithi
	tithi = nTithi + nPaksa*15;

	if (day.nTithi == tithi)
	{
		// loc1
		// find tithi juncts in this day and according to that times,
		// look in previous or next day for end and start of this tithi
		d.PreviousDay();
		day.DayCalc(d, earth);
		if ((day.nTithi > tithi ) && (day.nPaksa != nPaksa))
		{
			d.NextDay();
		}
		return d;
	}

	if (day.nTithi < tithi)
	{
		// do increment of date until nTithi == tithi
		//   but if nTithi > tithi
		//       then do decrement of date
		counter = 0;
		while(counter < 16)
		{
			d.NextDay();
			day.DayCalc(d, earth);
			if (day.nTithi == tithi)
				return d;
			if ((day.nTithi < tithi ) && (day.nPaksa != nPaksa))
				return d;
			if (day.nTithi > tithi)
				return d;
			counter++;
		}
		// somewhere is error
		d.year = d.month = d.day = 0;
		return d;
	}
	else
	{
		// do decrement of date until nTithi <= tithi
		counter = 0;
		while(counter < 16)
		{
			d.PreviousDay();
			day.DayCalc(d, earth);
			if (day.nTithi == tithi)
				return d;
			if ((day.nTithi > tithi ) && (day.nPaksa != nPaksa))
			{
				d.NextDay();
				return d;
			}
			if (day.nTithi < tithi)
			{
				d.NextDay();
				return d;
			}
			counter++;
		}
		// somewhere is error
		d.year = d.month = d.day = 0;
		return d;
	}

	// now we know the type of day-accurancy
	// nType = 0 means, that we dont found a day
	// nType = 1 means, we find day, when tithi was present at sunrise
	// nType = 2 means, we found day, when tithi started after sunrise
	//                  but ended before next sunrise
	//

	return d;
}

int GCTithi::writeXml(FILE * fout, CLocationRef &loc, VCTIME vc)
{
	TString str;
	VCTIME date;

	TFileXml xml;
	xml.initWithFile(fout);

	xml.write("<xml>\n");
	xml.write("\t<request name=\"Tithi\" version=\"");
	xml.write(GCStrings::getString(130));
	xml.write("\">\n");
	xml.write("\t\t<arg name=\"longitude\" val=\"");
	xml.write(loc.m_fLongitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"latitude\" val=\"");
	xml.write(loc.m_fLatitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"timezone\" val=\"");
	xml.write(loc.m_fTimezone);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"startdate\" val=\"");
	xml.write(vc);
	xml.write("\" />\n");
	xml.write("\t</request>\n");
	xml.write("\t<result name=\"Tithi\">\n");

	VCTIME d;
	d.Set(vc);
	VCTIME d1, d2;
	d.tzone = loc.m_fTimezone;
	VCTIME dn;
	DAYTIME dt;
	EARTHDATA earth = (EARTHDATA)loc;


	DAYDATA day;

	day.DayCalc(vc, earth);

	d.shour = day.sun.sunrise_deg/360.0 + loc.m_fTimezone/24.0;

	GCTithi::GetPrevTithiStart(earth, d, d1);
	GCTithi::GetNextTithiStart(earth, d, d2);

	{
		dt.SetDegTime( d1.shour * 360 );
		// start tithi at t[0]
		xml.write("\t\t<tithi\n\t\t\tid=\"");
	xml.write(day.nTithi);
	xml.write("\"\n");
		xml.write("\t\t\tname=\"");
	xml.write(GCStrings::GetTithiName(day.nTithi));
	xml.write("\"\n");
		xml.write("\t\t\tstartdate=\"");
	xml.write(d1);
	xml.write("\"\n");
		xml.write("\t\t\tstarttime=\"");
	xml.write(dt);
	xml.write("\"\n");
	
		dt.SetDegTime( d2.shour * 360 );
		xml.write("\t\t\tenddate=\"");
	xml.write(d2);
	xml.write("\"\n");
		xml.write("\t\t\tendtime=\"");
	xml.write(dt);
	xml.write("\"\n />");
	}

	xml.write("\t</result>\n");
	xml.write("</xml>\n");

	return 1;
}



int GCTithi::writeGaurabdaTithiXml(FILE * fout, CLocationRef &loc, VATIME vaStart, VATIME vaEnd)
{
	int gyearA = vaStart.gyear;
	int gyearB = vaEnd.gyear;
	int gmasa = vaStart.masa;
	int gpaksa = vaStart.tithi / 15;
	int gtithi = vaStart.tithi % 15;

	if (gyearB < gyearA)
		gyearB = gyearA;

	TFileXml xml;

	xml.initWithFile(fout);

	xml.write("<xml>\n");
	xml.write("\t<request name=\"Tithi\" version=\"");
	xml.write(GCStrings::getString(130));
	xml.write("\">\n");
	xml.write("\t\t<arg name=\"longitude\" val=\"");
	xml.write(loc.m_fLongitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"latitude\" val=\"");
	xml.write(loc.m_fLatitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"timezone\" val=\"");
	xml.write(loc.m_fTimezone);
	xml.write("\" />\n");
	if (gyearA > 1500)
	{
		xml.write("\t\t<arg name=\"year-start\" val=\"");
		xml.write(gyearA);
		xml.write("\" />\n");
		xml.write("\t\t<arg name=\"year-end\" val=\"");
		xml.write(gyearB);
		xml.write("\" />\n");
	}
	else
	{
		xml.write("\t\t<arg name=\"gaurabdayear-start\" val=\"");
		xml.write(gyearA);
		xml.write("\" />\n");
		xml.write("\t\t<arg name=\"gaurabdayear-end\" val=\"");
		xml.write(gyearB);
		xml.write("\" />\n");
	}
	xml.write("\t\t<arg name=\"masa\" val=\"");
	xml.write(gmasa);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"paksa\" val=\"");
	xml.write(gpaksa);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"tithi\" val=\"");
	xml.write(gtithi);
	xml.write("\" />\n");
	xml.write("\t</request>\n");
	xml.write("\t<result name=\"Tithi\">\n");


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
		vcs.Set(GCTithi::CalcTithiEnd(A, gmasa, gpaksa, gtithi, earth, vce));
		if (gyearA > 1500)
		{
			if ((vcs.year < gyearA) || (vcs.year > gyearB))
				continue;
		}
		oTithi = gpaksa*15 + gtithi;
		oMasa = gmasa;
		oPaksa = gpaksa;
		oYear = 0;
		xml.write("\t<celebration\n");
		//		xml.write("\t\t<tithi\n";
		xml.write("\t\trtithi=\"");
		xml.write(GCStrings::GetTithiName(oTithi));
		xml.write("\"\n");
		xml.write("\t\trmasa=\"");
		xml.write(GCStrings::GetMasaName(oMasa));
		xml.write("\"\n");
		xml.write("\t\trpaksa=\"");
		xml.write((oPaksa ? _T("Gaura") : _T("Krsna")));
		xml.write("\"\n");
		// test ci je ksaya
		today.Set(vcs);
		today.shour = 0.5;
		sun.SunCalc(today, earth);
		sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
		if (sunrise < vcs.shour)
		{
			today.Set(vce);
			sun.SunCalc(today, earth);
			sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
			if (sunrise < vce.shour)
			{
				// normal type
				vcs.NextDay();
				xml.write("\t\ttype=\"normal\"\n");
			}
			else
			{
				// ksaya
				vcs.NextDay();
				day.DayCalc(vcs, earth);
				oTithi = day.nTithi;
				oPaksa = day.nPaksa;
				oMasa = day.MasaCalc(vcs, earth);
				oYear = day.nGaurabdaYear;
				xml.write("\t\ttype=\"ksaya\"\n");
			}
		}
		else
		{
			// normal, alebo prvy den vriddhi
			today.Set(vce);
			sun.SunCalc(today, earth);
			if ((sun.sunrise_deg + loc.m_fTimezone*15.0)/360 < vce.shour)
			{
				// first day of vriddhi type
				xml.write("\t\ttype=\"vriddhi\"\n");
			}
			else
			{
				// normal
				xml.write("\t\ttype=\"normal\"\n");
			}
		}
		xml.write("\t\tdate=\"");
		xml.write(vcs);
		xml.write("\"\n");
		xml.write("\t\totithi=\"");
		xml.write(GCStrings::GetTithiName(oTithi));
		xml.write("\"\n");
		xml.write("\t\tomasa=\"");
		xml.write(GCStrings::GetMasaName(oMasa));
		xml.write("\"\n");
		xml.write("\t\topaksa=\"");
		xml.write((oPaksa ? _T("Gaura") : _T("Krsna")));
		xml.write("\"\n");
		xml.write("\t/>\n");
		//		xml.write("\t\t</celebration>\n";

	}

	xml.write("\t</result>\n");
	xml.write("</xml>\n");

	return 1;
}

int GCTithi::writeGaurabdaNextTithiXml(FILE * fout, CLocationRef &loc, VCTIME vcStart, VATIME vaStart)
{
	int gmasa, gpaksa, gtithi;
	TFileXml xml;

	gmasa = vaStart.masa;
	gpaksa = vaStart.tithi / 15;
	gtithi = vaStart.tithi % 15;

	xml.initWithFile(fout);

	xml.write("<xml>\n");
	xml.write("\t<request name=\"Tithi\" version=\"");
	xml.write(GCStrings::getString(130).c_str());
	xml.write("\">\n");
	xml.write("\t\t<arg name=\"longitude\" val=\"");
	xml.write(loc.m_fLongitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"latitude\" val=\"");
	xml.write(loc.m_fLatitude);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"timezone\" val=\"");
	xml.write(loc.m_fTimezone);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"start date\" val=\"");
	xml.write(vcStart);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"masa\" val=\"");
	xml.write(gmasa);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"paksa\" val=\"");
	xml.write(gpaksa);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"tithi\" val=\"");
	xml.write(gtithi);
	xml.write("\" />\n");
	xml.write("\t</request>\n");
	xml.write("\t<result name=\"Tithi\">\n");

	EARTHDATA earth = (EARTHDATA)loc;
	VCTIME vcs, vce, today;
	SUNDATA sun;
	int A;
	double sunrise;
	DAYDATA day;
	int oTithi, oPaksa, oMasa, oYear;

	today.Set(vcStart);
	today.PreviousDay();
	vcStart.SubtractDays(15);
	for(A = 0; A <= 3; A++)
	{
		vcs.Set(GCTithi::CalcTithiEndEx(vcStart, 0, gmasa, gpaksa, gtithi, earth, vce));
		if (!vcs.IsBeforeThis(today))
		{
			oTithi = gpaksa*15 + gtithi;
			oMasa = gmasa;
			oPaksa = gpaksa;
			oYear = 0;
			xml.write("\t<celebration\n");
			//		xml.write("\t\t<tithi\n";
			xml.write("\t\trtithi=\"");
			xml.write(GCStrings::GetTithiName(oTithi));
			xml.write("\"\n");
			xml.write("\t\trmasa=\"");
			xml.write(GCStrings::GetMasaName(oMasa));
			xml.write("\"\n");
			xml.write("\t\trpaksa=\"");
			xml.write((oPaksa ? _T("Gaura") : _T("Krsna")));
			xml.write("\"\n");
			// test ci je ksaya
			today.Set(vcs);
			today.shour = 0.5;
			sun.SunCalc(today, earth);
			sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
			if (sunrise < vcs.shour)
			{
				today.Set(vce);
				sun.SunCalc(today, earth);
				sunrise = (sun.sunrise_deg + loc.m_fTimezone*15.0)/360;
				if (sunrise < vce.shour)
				{
					// normal type
					vcs.NextDay();
					xml.write("\t\ttype=\"normal\"\n");
				}
				else
				{
					// ksaya
					vcs.NextDay();
					day.DayCalc(vcs, earth);
					oTithi = day.nTithi;
					oPaksa = day.nPaksa;
					oMasa = day.MasaCalc(vcs, earth);
					oYear = day.nGaurabdaYear;
					xml.write("\t\ttype=\"ksaya\"\n");
				}
			}
			else
			{
				// normal, alebo prvy den vriddhi
				today.Set(vce);
				sun.SunCalc(today, earth);
				if ((sun.sunrise_deg + loc.m_fTimezone*15.0)/360 < vce.shour)
				{
					// first day of vriddhi type
					xml.write("\t\ttype=\"vriddhi\"\n");
				}
				else
				{
					// normal
					xml.write("\t\ttype=\"normal\"\n");
				}
			}
			xml.write("\t\tdate=\"");
			xml.write(vcs);
			xml.write("\"\n");
			xml.write("\t\totithi=\"");
			xml.write(GCStrings::GetTithiName(oTithi));
			xml.write("\"\n");
			xml.write("\t\tomasa=\"");
			xml.write(GCStrings::GetMasaName(oMasa));
			xml.write("\"\n");
			xml.write("\t\topaksa=\"");
			xml.write((oPaksa ? _T("Gaura") : _T("Krsna")));
			xml.write("\"\n");
			xml.write("\t/>\n");
			break;
		}
		else
		{
			vcStart.Set(vcs);
			vcs.NextDay();
		}
	}

	xml.write("\t</result>\n");
	xml.write("</xml>\n");


	return 1;
}

