#pragma once

#include "GCEarthData.h"
#include "GCGregorianTime.h"

class GCTithi
{
public:
	GCTithi(void);
	~GCTithi(void);
	static int GetPrevTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
	static int GetNextTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
};

