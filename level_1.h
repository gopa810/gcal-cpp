// PORTABLE
#ifndef _VCAL_SE_LEVEL_1
#define _VCAL_SE_LEVEL_1

#include "level_0.h"

void SunPosition(VCTIME vct, EARTHDATA ed, SUNDATA &sun, double double);

void SunCalc(VCTIME, EARTHDATA, SUNDATA &);

VCTIME GetNextSankranti( VCTIME startDate, int &zodiac);

#endif