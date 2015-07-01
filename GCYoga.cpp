#include "stdafx.h"
#include "GCYoga.h"
#include "GCSunData.h"
#include "GCMoonData.h"
#include "GCMath.h"
#include "GCAyanamsha.h"

GCYoga::GCYoga(void)
{
}


GCYoga::~GCYoga(void)
{
}

int GCYoga::GetNextYogaStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate)
{
	double phi = 40.0/3.0;
	double l1, l2, sunl, sun2;
	double jday = startDate.GetJulianComplete();
	double xj;
	MOONDATA moon;
	VCTIME d = startDate;
	VCTIME xd;
	double scan_step = 0.5;
	int prev_tit = 0;
	int new_tit = -1;
	double ayanamsha = GCAyanamsha::GetAyanamsa(jday);
	moon.Calculate(jday, ed);
	sunl = SUNDATA::GetSunLongitude(d);
	l1 = GCMath::putIn360( moon.longitude_deg + sunl - 2*ayanamsha);
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
		sunl = SUNDATA::GetSunLongitude(d);
		l2 = GCMath::putIn360( moon.longitude_deg + sunl - 2*ayanamsha);
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

	return new_tit;
}
