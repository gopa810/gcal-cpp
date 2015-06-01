#include "stdafx.h"

#include "level_1.h"
#include "GCTithi.h"

// PORTABLE 

/*

Routines for calculation begining and ending times of given Tithi

Main function is GetTithiTimes

*/

double GetTithiTimes(EARTHDATA ed, VCTIME vc, double &titBeg, double &titEnd, double sunRise)
{
	VCTIME d1, d2;

	vc.shour = sunRise;
	GCTithi::GetPrevTithiStart(ed, vc, d1);
	GCTithi::GetNextTithiStart(ed, vc, d2);

	titBeg = d1.shour + d1.GetJulian() - vc.GetJulian();
	titEnd = d2.shour + d2.GetJulian() - vc.GetJulian();

	return (titEnd - titBeg);
}