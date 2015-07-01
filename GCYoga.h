#pragma once

#include "GCGregorianTime.h"
#include "GCEarthData.h"

class GCYoga
{
public:
	GCYoga(void);
	~GCYoga(void);

	static int GetNextYogaStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);

};

