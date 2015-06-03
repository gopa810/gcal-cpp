#pragma once

#include "GCGregorianTime.h"
#include "GCEarthData.h"
#include "GCVedicTime.h"

class GCCalendar
{
public:
	GCCalendar(void);
	~GCCalendar(void);

	static int	GetGaurabdaYear(VCTIME vc, EARTHDATA earth);
	static void VATIMEtoVCTIME(VATIME va, VCTIME &vc, EARTHDATA earth);
    static void VCTIMEtoVATIME(VCTIME vc, VATIME &va, EARTHDATA earth);
};

