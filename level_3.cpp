#include "stdafx.h"
#include "level_2.h"

// PORTABLE 

/*

Hindu calendar calculation

*/

void correct_parallax(MOONDATA &, JULIANDATE, double, double);


double DayCalcEx(VCTIME date, EARTHDATA earth, int nType)
{
	double d;
	JULIANDATE jdate;
	MOONDATA moon;
	SUNDATA sun;

	if (nType == DCEX_NAKSATRA_MIDNIGHT)
	{
		date.shour = 1.0;
		jdate = date.GetJulianDetailed();
		MoonCalc(jdate, moon, earth);
		d = put_in_360( moon.longitude_deg - GetAyanamsa(jdate));
		return floor(( d * 3.0) / 40.0 );
	}
	else if (nType == DCEX_MOONRISE)
	{
		return 0.3;
	}
	else if (nType == DCEX_SUNRISE)
	{
		// calculation TITHI at sunset
		SunCalc( date, earth, sun);
		return (sun.sunrise_deg / 360.0) + (date.tzone/24.0);
	}

	return 0;
}

/*********************************************************************/
/*                                                                   */
/* Calculation of tithi, paksa, naksatra, yoga for given             */
/*    Gregorian date                                                 */
/*                                                                   */
/*                                                                   */
/*********************************************************************/

int DayCalc(VCTIME date, EARTHDATA earth, DAYDATA &day)
{
	double d;
	JULIANDATE jdate;
//	SUNDATA sun;

	// sun position on sunrise on that day
	SunCalc( date, earth, day.sun);
	date.shour = day.sun.sunrise_deg/360.0;

	// date.shour is [0..1] time of sunrise in local timezone time
	day.jdate = jdate = date.GetJulianDetailed();

	// moon position at sunrise on that day
	MoonCalc(date.GetJulianDetailed(), day.moon, earth);

	// correct_parallax(day.moon, jdate, earth.latitude_deg, earth.longitude_deg);

	day.msDistance = put_in_360( day.moon.longitude_deg - day.sun.longitude_deg - 180.0);
	day.msAyanamsa = GetAyanamsa( jdate );

	// tithi
	d = day.msDistance / 12.0;
	day.nTithi = int(floor(d));
	day.nTithiElapse = (d - floor(d)) * 100.0;
	day.nPaksa = (day.nTithi >= 15) ? 1 : 0;


	// naksatra
	d = put_in_360( day.moon.longitude_deg - day.msAyanamsa );
	d = ( d * 3.0) / 40.0;
	day.nNaksatra = int(floor(d));
	day.nNaksatraElapse = (d - floor(d)) * 100.0;

	// yoga
	d = put_in_360( day.moon.longitude_deg + day.sun.longitude_deg - 2*day.msAyanamsa);
	d = (d * 3.0) / 40.0;
	day.nYoga = int(floor(d));
	day.nYogaElapse = (d - floor(d)) * 100.0;

	// masa
	day.nMasa = -1;

	// rasi
	day.nSunRasi = GetRasi(day.sun.longitude_deg, day.msAyanamsa);
	day.nMoonRasi = GetRasi(day.moon.longitude_deg, day.msAyanamsa);

	MOONDATA moon;
	date.shour = day.sun.sunset_deg/360.0;
	MoonCalc(date.GetJulianDetailed(), moon, earth);
	d = put_in_360(moon.longitude_deg - day.sun.longitude_set_deg - 180) / 12.0;
	day.nTithiSunset = (int)floor(d);

	date.shour = day.sun.arunodaya_deg/360.0;
	MoonCalc(date.GetJulianDetailed(), moon, earth);
	d = put_in_360(moon.longitude_deg - day.sun.longitude_arun_deg - 180) / 12.0;
	day.nTithiArunodaya = (int)floor(d);

	return 1;
}

/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/

int MasaCalc(VCTIME date, DAYDATA day, EARTHDATA earth, int & nGaurabdaYear)
{
//	SUNDATA sun;
//	MOONDATA moon;
//	VCTIME Conj[6], ConjA;
//	double Long[6], LongA;
//	int Sank[6], SankA;

	const int PREV_MONTHS = 6;

	double L[8];
	VCTIME C[8];
	int    R[8];
	int    n, rasi, masa;
	int    ksaya_from = -1;
	int    ksaya_to = -1;

	date.shour = day.sun.sunrise_deg / 360.0 + earth.tzone / 24.0;

	// STEP 1: calculate position of the sun and moon
	// it is done by previous call of DayCalc
	// and results are in argument DAYDATA day
	// *DayCalc(date, earth, day, moon, sun);*

	L[1] = /*Long[0] =*/ GetNextConjunction(date, C[1], false, earth);
	L[0] = /*LongA   =*/ GetNextConjunction(C[1], C[0], true, earth);

	// on Pratipat (nTithi == 15) we need to look for previous conjunction
	// but this conjunction can occur on this date before sunrise
	// so we need to include this very date into looking for conjunction
	// on other days we cannot include it
	// and exclude it from looking for next because otherwise that will cause
	// incorrect detection of Purusottama adhika masa
	L[2] = GetPrevConjunction(date, C[2], false, earth);

	for(n = 3; n < PREV_MONTHS; n++)
		L[n] = GetPrevConjunction(C[n-1], C[n], true, earth);

	for(n = 0; n < PREV_MONTHS; n++)
		R[n] = GetRasi(L[n], GetAyanamsa(C[n].GetJulian()));

/*	TRACE("TEST Date: %d %d %d\n", date.day, date.month, date.year);
	TRACE("FOUND CONJ Date: %d %d %d rasi: %d\n", C[1].day, C[1].month, C[1].year, R[1]);
	TRACE("FOUND CONJ Date: %d %d %d rasi: %d\n", C[2].day, C[2].month, C[2].year, R[2]);
	TRACE("FOUND CONJ Date: %d %d %d rasi: %d\n", C[3].day, C[3].month, C[3].year, R[3]);
	TRACE("FOUND CONJ Date: %d %d %d rasi: %d\n", C[4].day, C[4].month, C[4].year, R[4]);
	TRACE("---\n");
*/
	// test for Adhika-Ksaya sequence
	// this is like 1-2-2-4-5...
	// second (2) is replaced by rasi(3)
/*	if ( ((Sank[1] + 2) % 12 == SankA) && ((Sank[1] == Sank[0]) || (Sank[0] == SankA)))
	{
		Sank[0] = (Sank[1] + 1) % 12;
	}
	
	if ( ((Sank[2] + 2) % 12 == Sank[0]) && ((Sank[2] == Sank[1]) || (Sank[1] == Sank[0])))
	{
		Sank[1] = (Sank[2] + 1) % 12;
	}*/

	// look for ksaya month
	ksaya_from = -1;
	for(n = PREV_MONTHS - 2; n >= 0; n--)
	{
		if ((R[n+1] + 2) % 12 == R[n])
		{
			ksaya_from = n;
			break;
		}
	}

	if (ksaya_from >= 0)
	{
		for(n = ksaya_from; n > 0; n--)
		{
			if (R[n] == R[n-1])
			{
				ksaya_to = n;
				break;
			}
		}

		if (ksaya_to >= 0)
		{
			// adhika masa found
			// now correct succession of rasis
		}
		else
		{
			// adhika masa not found
			// there will be some break in masa queue
			ksaya_to = 0;
		}

		int current_rasi = R[ksaya_from + 1] + 1;
		for(n = ksaya_from; n >= ksaya_to; n--)
		{
			R[n] = current_rasi;
			current_rasi = (current_rasi + 1) % 12;
		}
	}

	// STEP 3: test for adhika masa
	// test for adhika masa
	if (R[1] == R[2])
	{
		// it is adhika masa
		masa = 12;
		rasi = R[1];
	}
	else
	{
		// STEP 2. select nearest Conjunction
		if (day.nPaksa == 0)
		{
			masa = R[1];
		}
		else if (day.nPaksa == 1)
		{
			masa = R[2];
		}
		rasi = masa;
	}

	// calculation of Gaurabda year
	nGaurabdaYear = date.year - 1486;

	if ((rasi > 7) && (rasi < 11)) // Visnu
	{
		if (date.month < 6)
			nGaurabdaYear--;
	}


	return masa;

}

