#include "stdafx.h"
#include "GCNaksatra.h"
#include "GCMoonData.h"
#include "GCMath.h"
#include "GCAyanamsha.h"
#include "GCStrings.h"
#include "TFileXml.h"
#include "GCSunData.h"

GCNaksatra::GCNaksatra(void)
{
}


GCNaksatra::~GCNaksatra(void)
{
}

double GCNaksatra::CalculateMidnightNaksatra(VCTIME date, EARTHDATA earth)
{
	double d;
	double jdate;
	MOONDATA moon;

	date.shour = 1.0;
	jdate = date.GetJulianDetailed();
	moon.Calculate(jdate, earth);
	d = GCMath::putIn360( moon.longitude_deg - GCAyanamsha::GetAyanamsa(jdate));
	return floor(( d * 3.0) / 40.0 );
}

/*********************************************************************/
/*                                                                   */
/*   finds next time when starts next naksatra                       */
/*                                                                   */
/*   timezone is not changed                                         */
/*                                                                   */
/*   return value: index of naksatra 0..26                           */
/*                 or -1 if failed                                   */
/*********************************************************************/

int GCNaksatra::GetNextNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate)
{
	double phi = 40.0/3.0;
	double l1, l2;
	double jday = startDate.GetJulianComplete();
	MOONDATA moon;
	VCTIME d = startDate;
	double ayanamsa = GCAyanamsha::GetAyanamsa(jday);
	double scan_step = 0.5;
	int prev_naks = 0;
	int new_naks = -1;

	double xj;
	VCTIME xd;

	moon.Calculate(jday, ed);
	l1 = GCMath::putIn360(moon.longitude_deg - ayanamsa);
	prev_naks = int(floor(l1 / phi));

	int counter = 0;
	while(counter < 20)
	{
		xj = jday;
		xd = d;

		jday += scan_step;
		d.shour += scan_step;
		if (d.shour > 1.0)
		{
			d.shour -= 1.0;
			d.NextDay();
		}

		moon.Calculate(jday, ed);
		l2 = GCMath::putIn360(moon.longitude_deg - ayanamsa);
		new_naks = int(floor(l2/phi));
		if (prev_naks != new_naks)
		{
			jday = xj;
			d = xd;
			scan_step *= 0.5;
			counter++;
			continue;
		}
		else
		{
			l1 = l2;
		}
	}
	nextDate = d;
	return new_naks;
}

/*********************************************************************/
/*                                                                   */
/*   finds previous time when starts next naksatra                   */
/*                                                                   */
/*   timezone is not changed                                         */
/*                                                                   */
/*   return value: index of naksatra 0..26                           */
/*                 or -1 if failed                                   */
/*********************************************************************/

int GCNaksatra::GetPrevNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate)
{
	double phi = 40.0/3.0;
	double l1, l2;
	double jday = startDate.GetJulianComplete();
	MOONDATA moon;
	VCTIME d = startDate;
	double ayanamsa = GCAyanamsha::GetAyanamsa(jday);
	double scan_step = 0.5;
	int prev_naks = 0;
	int new_naks = -1;

	double xj;
	VCTIME xd;

	moon.Calculate(jday, ed);
	l1 = GCMath::putIn360(moon.longitude_deg - ayanamsa);
	prev_naks = int(floor(l1/phi));

	int counter = 0;
	while(counter < 20)
	{
		xj = jday;
		xd = d;

		jday -= scan_step;
		d.shour -= scan_step;
		if (d.shour < 0.0)
		{
			d.shour += 1.0;
			d.PreviousDay();
		}

		moon.Calculate(jday, ed);
		l2 = GCMath::putIn360(moon.longitude_deg - ayanamsa);
		new_naks = int(floor(l2/phi));

		if (prev_naks != new_naks)
		{
			jday = xj;
			d = xd;
			scan_step *= 0.5;
			counter++;
			continue;
		}
		else
		{
			l1 = l2;
		}

	}

	nextDate = d;
	return new_naks;

}

int GCNaksatra::writeXml(FILE * fout, CLocationRef &loc, VCTIME vc, int nDaysCount)
{
	TString str;
	VCTIME date;

	TFileXml xml;
	xml.initWithFile(fout);

	xml.write("<xml>\n");
	xml.write("\t<request name=\"Naksatra\" version=\"");
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
	xml.write("\t\t<arg name=\"daycount\" val=\"");
	xml.write(nDaysCount);
	xml.write("\" />\n");
	xml.write("\t</request>\n");
	xml.write("\t<result name=\"Naksatra\">\n");

	VCTIME d = vc;
	d.tzone = loc.m_fTimezone;
	VCTIME dn;
	DAYTIME dt;
	SUNDATA sun;
	int nak;
	EARTHDATA earth = (EARTHDATA)loc;

	for(int i = 0; i < 30; i++)
	{
		nak = GCNaksatra::GetNextNaksatra(earth, d, dn);
		d = dn;
		xml.write("\t\t<day date=\"");
		xml.write(d);
		xml.write("\">\n");
		//str.Format("%d.%d.%d", d.day, d.month, d.year);
		//n = m_list.InsertItem(50, GetNaksatraName(nak));
		//m_list.SetItemText(n, 1, str);
		xml.write("\t\t\t<naksatra id=\"");
		xml.write(nak);
		xml.write("\" name=\"");
		xml.write(GCStrings::GetNaksatraName(nak));
		xml.write("\"\n");
		dt.SetDegTime( d.shour * 360);
		//time_print(str, dt);
		xml.write("\t\t\t\tstarttime=\"");
		xml.write(dt);
		xml.write("\" />\n");
		//m_list.SetItemText(n, 2, str);

		// sunrise time get
		sun.SunCalc(d, earth);
		//time_print(str, sun.rise);
		//m_list.SetItemText(n, 3, str);
		xml.write("\t\t\t<sunrise time=\"");
		xml.write(sun.rise);
		xml.write("\" />\n");

		xml.write("\t\t</day>\n");
		// increment for non-duplication of naksatra
		d = dn;
		d.shour += 1.0/8.0;
	}


	xml.write("\t</result>\n");
	xml.write("</xml>\n");


	return 1;
}

double GCNaksatra::GetEndHour(EARTHDATA earth, VCTIME yesterday, VCTIME today)
{
	VCTIME nend;
	VCTIME snd = yesterday;
	snd.shour = 0.5;
	GCNaksatra::GetNextNaksatra(earth, snd, nend);
	return nend.GetJulian() - today.GetJulian() + nend.shour;
}
