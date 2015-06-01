#include "stdafx.h"
#include "GCTithi.h"
#include "GCMoonData.h"
#include "GCSunData.h"
#include "gmath.h"

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
	double l1, l2, sunl;
	double jday = startDate.GetJulianComplete();
	double xj;
	MOONDATA moon;
	VCTIME d = startDate;
	VCTIME xd;
	double scan_step = 0.5;
	int prev_tit = 0;
	int new_tit = -1;

	moon.Calculate(jday, ed);
	sunl = SUNDATA::GetSunLongitude(d);
//	SunPosition(d, ed, sun, d.shour - 0.5 + d.tzone/24.0);
	l1 = put_in_360(moon.longitude_deg - sunl - 180.0);
	prev_tit = int(floor(l1/phi));

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
		//SunPosition(d, ed, sun, d.shour - 0.5 + d.tzone/24.0);
		//l2 = put_in_360(moon.longitude_deg - sun.longitude_deg - 180.0);
		sunl = SUNDATA::GetSunLongitude(d);
		l2 = put_in_360(moon.longitude_deg - sunl - 180.0);
		new_tit = int(floor(l2/phi));

		if (prev_tit != new_tit)
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
	VCTIME d = startDate;
	VCTIME xd;
	double scan_step = 0.5;
	int prev_tit = 0;
	int new_tit = -1;

	moon.Calculate(jday, ed);
	sunl = SUNDATA::GetSunLongitude(d);
	l1 = put_in_360(moon.longitude_deg - sunl - 180.0);
	prev_tit = int(floor(l1/phi));

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
		sunl = SUNDATA::GetSunLongitude(d);
		l2 = put_in_360(moon.longitude_deg - sunl - 180.0);
		new_tit = int(floor(l2/phi));

		if (prev_tit != new_tit)
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
//	nextDate.shour += startDate.tzone / 24.0;
//	nextDate.NormalizeValues();
	return new_tit;
}