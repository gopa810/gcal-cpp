#pragma once

#include "GCEarthData.h"


class GCEclipse
{
public:
	GCEclipse(void);
	~GCEclipse(void);

	static void CalculateTimesSunEclipse(double startDate, EARTHDATA obs, double * times);
	static void CalculateTimesMoonEclipse(double startDate, EARTHDATA obs, double * times);
};

