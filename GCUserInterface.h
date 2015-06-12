#pragma once

#include "GCGregorianTime.h"
#include "LocationRef.h"
#include "TResultCalendar.h"


class GCUserInterface
{
public:
	GCUserInterface(void);
	~GCUserInterface(void);

	static int CalculateCalendar(TResultCalendar & daybuff, CLocationRef & loc, VCTIME date, int nDaysCount); 
	static int CreateProgressWindow();
	static int SetProgressWindowRange(int nMin, int nMax);
	static int SetProgressWindowPos(int nPos);
	static int CloseProgressWindow();

	static int ShowMode;
};

