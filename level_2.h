// PORTABLE
#ifndef _VCAL_SE_LEVEL_2
#define _VCAL_SE_LEVEL_2

#include "level_1.h"

#define DCEX_NAKSATRA_MIDNIGHT 3
#define DCEX_MOONRISE          4
#define DCEX_SUNRISE           5

double GetTithiTimes(EARTHDATA, VCTIME vc, double &titBeg, double &titEnd, double sunRise);

double GetPrevConjunction(VCTIME &, EARTHDATA);
double GetNextConjunction(VCTIME &, EARTHDATA);
double GetPrevConjunction(VCTIME, VCTIME &, bool, EARTHDATA);
double GetNextConjunction(VCTIME, VCTIME &, bool, EARTHDATA);

#endif