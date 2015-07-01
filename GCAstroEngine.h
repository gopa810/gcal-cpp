#pragma once

#include "GCCoords.h"
#include "GCEarthData.h"

class GCAstroEngine
{
public:
	GCAstroEngine(void);
	~GCAstroEngine(void);
	static GCEclipticalCoords getMoonCoordinates(double julian);
	static GCEclipticalCoords getSunCoordinate(double date);
	static GCEclipticalCoords getEarthCoordinates(double date);
	static void DynamicToFK5(double *longitude, double *latitude, double tau);

};

