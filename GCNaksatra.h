#pragma once

#include "GCGregorianTime.h"
#include "GCEarthData.h"
#include "LocationRef.h"

class GCNaksatra
{
public:
	GCNaksatra(void);
	~GCNaksatra(void);

	static int GetNextNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
	static int GetPrevNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &prevDate);
	static double CalculateMidnightNaksatra(VCTIME date, EARTHDATA earth);
	static int writeXml(FILE *, CLocationRef &loc, VCTIME, int);

};

