#include "stdafx.h"
#include "GCSankranti.h"
#include "gmath.h"
#include "GCAyanamsha.h"
#include "GCSunData.h"
#include "TFileXml.h"
#include "GCStrings.h"


GCSankranti::GCSankranti(void)
{
}


GCSankranti::~GCSankranti(void)
{
}


//==================================================================
//
//==================================================================

int gSanType = 2;


//==================================================================
//
//==================================================================

int GCSankranti::GetSankrantiType(void)
{
	return gSanType;
}

//==================================================================
//
//==================================================================

int GCSankranti::SetSankrantiType(int i)
{
	int prev = gSanType;
	gSanType = i;
	return prev;
}

//==================================================================
//
//==================================================================



const char * GCSankranti::GetSankMethodName(int i)
{
	static char * snam[] = {
		"midnight to midnight",
		"sunrise to sunrise",
		"noon to noon",
		"sunset to sunset"};

	return snam[i];
}


/*********************************************************************/
/*  Finds next time when rasi is changed                             */
/*                                                                   */
/*  startDate - starting date and time, timezone member must be valid */
/*  zodiac [out] - found zodiac sign into which is changed           */
/*                                                                   */
/*********************************************************************/

VCTIME GCSankranti::GetNextSankranti( VCTIME startDate, int &zodiac)
{
	VCTIME d;
	double step = 1.0;
	int count = 0;
	double ld, prev;
	int prev_rasi, new_rasi;
	VCTIME prevday;

	d = startDate;
	//d.ChangeTimeZone(0.0);
	//d.shour = 0.0;

	prev = put_in_360( SUNDATA::GetSunLongitude(d) - GCAyanamsha::GetAyanamsa(d.GetJulian()));
	prev_rasi = (int)floor(prev / 30.0);

	while(count < 20)
	{
		prevday = d;
		d.shour += step;
		if (d.shour > 1.0)
		{
			d.shour -= 1.0;
			d.NextDay();
		}
		
		ld = put_in_360(SUNDATA::GetSunLongitude(d) - GCAyanamsha::GetAyanamsa(d.GetJulian()));
		new_rasi = (int)floor(ld/30.0);

		if (prev_rasi != new_rasi)
		{
			zodiac = new_rasi;
			//v uplynulom dni je sankranti
			step *= 0.5;
			d = prevday;
			count++;
			continue;
		}
	}

	return d;
}

int GCSankranti::writeXml(FILE * fout, CLocationRef & loc, VCTIME vcStart, VCTIME vcEnd)
{
	VCTIME d;
	DAYTIME dt;
	int zodiac;

	TFileXml xml;

	// open file
	xml.initWithFile(fout);

	d = vcStart;

	xml.write("<xml>\n");
	xml.write("\t<request name=\"Sankranti\" version=\"");
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
	xml.write("\t\t<arg name=\"location\" val=\"");
	xml.write(loc.m_strName);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"startdate\" val=\"");
				xml.write(vcStart);
	xml.write("\" />\n");
	xml.write("\t\t<arg name=\"enddate\" val=\"");
				xml.write(vcEnd);
	xml.write("\" />\n");
	xml.write("\t</request>\n");
	xml.write("\t<result name=\"SankrantiList\">\n");

	while(d.IsBeforeThis(vcEnd))
	{
		d = GCSankranti::GetNextSankranti(d, zodiac);
		d.InitWeekDay();
		xml.write("\t\t<sank date=\"");
		xml.write(d);
		xml.write("\" ");
		xml.write("dayweekid=\"");
		xml.write(d.dayOfWeek);
		xml.write("\" dayweek=\"");
		xml.write(GCStrings::getString(d.dayOfWeek));
		xml.write("\" ");

		dt.SetDegTime( 360 * d.shour );

		xml.write(" time=\"");
		xml.write(dt);
		xml.write("\" >\n");
		xml.write("\t\t<zodiac sans=\"");
		xml.write(GCStrings::GetSankrantiName(zodiac));
		xml.write("\" eng=\"");
		xml.write(GCStrings::GetSankrantiNameEn(zodiac));
		xml.write("\" id=\"");
		xml.write(zodiac);
		xml.write("\" />\n");
		xml.write("\t\t</sank>\n\n");

		d.NextDay();
		d.NextDay();
	}

	xml.write("\t</result>\n");
	xml.write("</xml>");

	return 1;
}
