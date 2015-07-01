#pragma once

#include "GCEarthData.h"
#include "GCCoords.h"
#include "GCFunctor.h"

enum GCFunctorSunMoonValueType
{
	None,
    Longitude,
    Latitude,
    RightAscession,
    RightAscessionAbs,
    RightAscessionTopo,
    Declination,
    DeclinationTopo,
    Azimuth,
    Elevation,
    Nutation,
    Obliquity,
    ApparentSiderealTime,
    MeanSiderealTime,
    DynamicTime,
    UniversalTime,
    AzimuthElevationDistance,
    RigthAscessionDeclinationDistance,
    RigthAscessionDeclinationOpositeDistance,
    TithiDistance,
    Tithi,
    Yoga,
    YogaDistance
};

class GCFunctorSunMoon: public GCFunctor
{
public:
	GCFunctorSunMoon(void);
	GCFunctorSunMoon(GCFunctorSunMoonValueType vt);
	GCFunctorSunMoon(GCFunctorSunMoonValueType vt, EARTHDATA obs);
	~GCFunctorSunMoon(void);

	GCFunctorSunMoonValueType valueType;
    EARTHDATA obs;
	GCEquatorialCoords eqMoon;
	GCEquatorialCoords eqSun;
    GCEclipticalCoords ecMoon;
    GCEclipticalCoords ecSun;
	GCHorizontalCoords hzMoon;
	GCHorizontalCoords hzSun;

	double getDoubleValue(double arg);
	double getTithiDouble();
	double getYogaDouble(double day);

};

