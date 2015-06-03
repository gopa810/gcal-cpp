#include "stdafx.h"
#include "GCCalendar.h"
#include "GCDayData.h"
#include "GCTithi.h"

GCCalendar::GCCalendar(void)
{
}


GCCalendar::~GCCalendar(void)
{
}

int	GCCalendar::GetGaurabdaYear(VCTIME vc, EARTHDATA earth)
{
	DAYDATA day;

	day.DayCalc(vc, earth);
	day.MasaCalc(vc, earth);

	return day.nGaurabdaYear;
}

//===========================================================================
//
//===========================================================================

void GCCalendar::VATIMEtoVCTIME(VATIME va, VCTIME &vc, EARTHDATA earth)
{
	vc = GCTithi::CalcTithiDate(va.gyear, va.masa, va.tithi / 15, va.tithi % 15, earth);
}

//===========================================================================
//
//===========================================================================

void GCCalendar::VCTIMEtoVATIME(VCTIME vc, VATIME &va, EARTHDATA earth)
{
	DAYDATA day;

	day.DayCalc(vc, earth);
	va.masa = day.MasaCalc(vc, earth);
	va.tithi = day.nTithi;
	va.gyear = day.nGaurabdaYear;
}

