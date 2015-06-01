#pragma once

#include "GCEarthData.h"

class MOONDATA 
{
public:
	/*// illumination (synodic) phase
	double ip;
	// age of moon
	int age;
	// distance from anomalistic phase
	double di;*/
	// latitude from nodal (draconic) phase
	double latitude_deg;
	// longitude from sidereal motion
	double longitude_deg;

	double radius; //(* lambda, beta, R *)
	double rektaszension, declination;  //(* alpha, delta *)
	double parallax;
	double elevation, azimuth;//          (* h, A *)

	MOONDATA();
	~MOONDATA();
	void Calculate(double jdate, EARTHDATA earth);
};

