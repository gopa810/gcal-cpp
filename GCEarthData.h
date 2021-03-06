#pragma once

#include "GCGregorianTime.h"
#include "TString.h"
#include "GCCoords.h"

class EARTHDATA
{
public:
	int dst;
	// terrestrial longitude of observation
	double longitude_deg;

	// terrestrial latitude of observation
	double latitude_deg;

	// time zone (hours)
	double tzone;

	// observated event
	// 0 - center of the sun
	// 1 - civil twilight
	// 2 - nautical twilight
	// 3 - astronomical twilight
	int obs;

	EARTHDATA();
	static void calc_epsilon_phi(double date, double &epsilon, double &phi);
	static GCEquatorialCoords eclipticalToEquatorialCoords(GCEclipticalCoords * ecc, double date);
	static GCHorizontalCoords equatorialToHorizontalCoords(GCEquatorialCoords * eqc, EARTHDATA obs, double date);
	static char * GetTextLatitude(double d);
	static char * GetTextLongitude(double d);
	static double star_time(double date);

	double GetHorizontDegrees(double jday);
	int GetNextAscendentStart(VCTIME startDate, VCTIME &nextDate);
	int ToString(TString &str);

};

