#include "stdafx.h"

#include "level_1.h"

// PORTABLE 

/*

Routines for calculation begining and ending times of given Tithi

Main function is GetTithiTimes

*/

int GetPrevTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &prevDate);
int GetNextTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);


DAYHOURS GetTithiTimes(EARTHDATA ed, VCTIME vc, DAYHOURS &titBeg, DAYHOURS &titEnd, DAYHOURS sunRise)
{
	VCTIME d1, d2;

	vc.shour = sunRise;
	GetPrevTithiStart(ed, vc, d1);
	GetNextTithiStart(ed, vc, d2);

	titBeg = d1.shour + d1.GetJulian() - vc.GetJulian();
	titEnd = d2.shour + d2.GetJulian() - vc.GetJulian();

	return (titEnd - titBeg);
}