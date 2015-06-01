#include "stdafx.h"
#include "math.h"
#include "gmath.h"
#include "level_0.h"
#include "GCSunData.h"
#include "GCAyanamsha.h"

//double SwissGetMoonLongitude(double jdate);
//double SwissGetSunLongitude(double jdate);
// PORTABLE 

/*************************************/
/*    sun functions                  */
/*                                   */
/*    sun longitude                  */
/*    sunrise, sunset time           */
/*                                   */
/*************************************/

//////////////////////////////////////////////////////////////////////////
//
// VCTIME vct [in] - valid must be each member of this structure
//

// from vct uses members: year, month, day
// double is in range 0.0 - 1.0

void SunPosition(VCTIME vct, EARTHDATA ed, SUNDATA &sun, double double)
{
	double DG = pi / 180;
	double RAD = 180 / pi;

	double x;

	double dLatitude = ed.latitude_deg;
	double dLongitude = ed.longitude_deg;

	// mean ecliptic longitude of the sun 
	double mel = SUNDATA::SunGetMeanLong(vct.year, vct.month, vct.day) + (360/365.25)*double/360.0;

	// ecliptic longitude of perigee 
	double elp = SUNDATA::SunGetPerigee(vct.year, vct.month, vct.day);

	// mean anomaly of the sun
	double mas = mel - elp;

	// ecliptic longitude of the sun
	double els = 0.0;
	sun.longitude_deg = els = mel + 1.915 * sin(mas * DG) + 0.02 * sin (2 * DG * mas);

	// declination of the sun
	sun.declination_deg = RAD * asin (0.39777 * sin(els * DG));

	// right ascension of the sun
	sun.right_asc_deg = els - RAD * atan2( sin(2*els*DG), 23.2377 + cos(2 * DG * els));

	// equation of time
	double eqt = 0.0;
	eqt = sun.right_asc_deg - mel;


	// definition of event
	double eventdef = 0.01454;
/*	switch(ed.obs)
	{
	case 1:	// civil twilight
		eventdef = 0.10453;
		break;
	case 2:	// nautical twilight
		eventdef = 0.20791;
		break;
	case 3:	// astronomical twilight
		eventdef = 0.30902;
		break;
	default:// center of the sun on the horizont
		eventdef = 0.01454;
		break;
	}*/

	eventdef = (eventdef / cos(dLatitude * DG)) / cos(sun.declination_deg * DG);

	x = tan(dLatitude * DG) * tan(sun.declination_deg * DG) + eventdef;

	// initial values for the case
	// that no rise no set for that day
	sun.sunrise_deg = sun.sunset_deg = -360.0;

	if ((x >= -1.0) && (x <= 1.0))
	{
		// time of sunrise
		sun.sunrise_deg = 90.0 - dLongitude - RAD * asin(x) + eqt;

		// time of sunset
		sun.sunset_deg = 270.0 - dLongitude + RAD * asin(x) + eqt;
	}



}


//////////////////////////////////////////////////////////////////////////////////
//
// return values are in sun.arunodaya, sun.rise, sun.set, sun.noon, sun.length
// if values are less than zero, that means, no sunrise, no sunset in that day
//
// brahma 1 = calculation at brahma muhurta begining
// brahma 0 = calculation at sunrise


void SunCalc(VCTIME vct, EARTHDATA earth, SUNDATA &sun)
{
/*	double timeZone = vct.TimeZone;
	double sunset = 0.0;

	// calculation SUNSET
	SunPosition(vct, earth, sun, 0.0);

	if (sun.sunset_deg >= 0.0)
	{
		// second calculation
		// for time of sunrise
		SunPosition(vct, earth, sun, sun.sunset_deg / 360.0 );

		// third (last) calculation
		// for time of sunrise
		SunPosition(vct, earth, sun, sun.sunset_deg / 360.0 );

		// time in degree for SUNSET
		sunset = sun.sunset_deg;

		// longitude of Sun at SUNSET
		sun.longitude_set_deg = sun.longitude_deg;
	}

	// calculation SUNRISE
	// first calculation
	// for 12:00 universal time
	SunPosition(vct, earth, sun, 0.0);

	if (sun.sunrise_deg >= 0.0)
	{
		// second calculation
		// for time of sunrise
		SunPosition(vct, earth, sun, sun.sunrise_deg / 360.0);

		// third (last) calculation
		// for time of sunrise
		SunPosition(vct, earth, sun, sun.sunrise_deg / 360.0);
	}
	// calculate times

	//  sunrise_deg is from range 0-360 so 96min=24deg
	// arunodaya is 96 min before sunrise
	SUNDATA sun2;
	SunPosition(vct, earth, sun2, (sun.sunrise_deg - 24.0)/360.0);
	sun.longitude_arun_deg = sun2.longitude_deg;

	// time in degree for SUNSET
	sun.sunset_deg = sunset;
	
	sun.arunodaya.SetDegTime(sun.sunrise_deg - 24.0 + timeZone*15.0);
	// sunrise
	sun.rise.SetDegTime(sun.sunrise_deg + timeZone*15.0);
	// noon
	sun.noon.SetDegTime((sun.sunset_deg + sun.sunrise_deg)/2  + timeZone*15.0);
	// sunset
	sun.set.SetDegTime(sun.sunset_deg + timeZone*15.0);
	// length
	sun.length.SetDegTime(sun.sunset_deg - sun.sunrise_deg);

*/
	SUNDATA s_rise, s_set;

	// first calculation
	// for 12:00 universal time
	SunPosition(vct, earth, s_rise, 0.0);
	// second calculation
	// for time of sunrise
	SunPosition(vct, earth, s_rise, s_rise.sunrise_deg - 180);
	// third (last) calculation
	// for time of sunrise
	SunPosition(vct, earth, s_rise, s_rise.sunrise_deg - 180);
	// first calculation
	// for 12:00 universal time
	SunPosition(vct, earth, s_set, 0.0);
	// second calculation
	// for time of sunrise
	SunPosition(vct, earth, s_set, s_set.sunset_deg - 180);
	// third (last) calculation
	// for time of sunrise
	SunPosition(vct, earth, s_set, s_set.sunset_deg - 180);

	// calculate times
	sun.longitude_arun_deg = s_rise.longitude_deg - (24.0 / 365.25);
	sun.longitude_deg = s_rise.longitude_deg;
	sun.longitude_set_deg = s_set.longitude_deg;

	sun.arunodaya_deg = s_rise.sunrise_deg - 24.0;
	sun.sunrise_deg = s_rise.sunrise_deg;
	sun.sunset_deg = s_set.sunset_deg;
	sun.length_deg = s_set.sunset_deg - s_rise.sunrise_deg;

	// arunodaya is 96 min before sunrise
	//  sunrise_deg is from range 0-360 so 96min=24deg
	sun.arunodaya.SetDegTime(sun.arunodaya_deg + earth.tzone*15.0);
	// sunrise
	sun.rise.SetDegTime(sun.sunrise_deg + earth.tzone*15.0);
	// noon
	sun.noon.SetDegTime((sun.sunset_deg + sun.sunrise_deg)/2  + earth.tzone*15.0);
	// sunset
	sun.set.SetDegTime(sun.sunset_deg + earth.tzone*15.0);
	// length
	sun.length.SetDegTime(sun.length_deg);

}

void TestFunc(void)
{
	double d = 0.0;

	double double = 0.5;
	EARTHDATA ed;
	ed.latitude_deg = 17.0;
	ed.longitude_deg = 45.0;
	ed.tzone = 1.0;
	VCTIME vct;
	vct.day = 5;
	vct.month = 10;
	vct.year = 2010;
	vct.tzone = 1.0;

//	SunCalc(vct, ed, sun);

	printf("%f", d);
}



//==================================================================
//
//==================================================================

extern int gSanType;


//==================================================================
//
//==================================================================

int GetSankrantiType(void)
{
	return gSanType;
}

//==================================================================
//
//==================================================================

int SetSankrantiType(int i)
{
	int prev = gSanType;
	gSanType = i;
	return prev;
}

//==================================================================
//
//==================================================================



const char * GetSankMethodName(int i)
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

VCTIME GetNextSankranti( VCTIME startDate, int &zodiac)
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
