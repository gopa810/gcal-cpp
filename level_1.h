// PORTABLE
#ifndef _VCAL_SE_LEVEL_1
#define _VCAL_SE_LEVEL_1

#include "level_0.h"

//double GetSunLongitude(JULIANDATE jDate);

double GetSunLongitude(VCTIME date);

void SunPosition(VCTIME vct, EARTHDATA ed, SUNDATA &sun, DAYHOURS DayHours);

void SunCalc(VCTIME, EARTHDATA, SUNDATA &);

double GetAyanamsa(JULIANDATE);

int GetRasi(double SunLongitude, double Ayanamsa);

VCTIME GetNextSankranti( VCTIME startDate, int &zodiac);

void MoonCalc(JULIANDATE, MOONDATA &, EARTHDATA);

void calc_horizontal(MOONDATA &, JULIANDATE, double, double);

#endif