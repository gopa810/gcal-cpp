#pragma once

#include "GCGregorianTime.h"
#include "LocationRef.h"
#include "TResultCalendar.h"

class TResultToday
{
	VCTIME currentDay;
	TResultCalendar * calendar;
public:
	TResultToday(void);
	~TResultToday(void);
	void Calculate(VCTIME dateTime, CLocationRef& location);
	VAISNAVADAY * GetCurrentDay(void);
	void formatPlain(TString &str);
	void formatRtf(TString &str);
	void writeHtml(FILE * f);
};

