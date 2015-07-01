#include "stdafx.h"
#include "GCFunctorSunMoon.h"
#include "GCMath.h"
#include "GCAyanamsha.h"
#include "GCMoonData.h"
#include "GCSunData.h"
#include "GCAstroEngine.h"

GCFunctorSunMoon::GCFunctorSunMoon(void)
{
	valueType = None;
}

GCFunctorSunMoon::GCFunctorSunMoon(GCFunctorSunMoonValueType vt)
{
	valueType = vt;
}

GCFunctorSunMoon::GCFunctorSunMoon(GCFunctorSunMoonValueType vt, EARTHDATA obsa)
{
	valueType = vt;
	obs = obsa;
}


GCFunctorSunMoon::~GCFunctorSunMoon(void)
{
}

double GCFunctorSunMoon::getDoubleValue(double arg)
{
	MOONDATA moon;
	SUNDATA sun;

	moon.Calculate(arg, obs);
	ecMoon.longitude = moon.longitude_deg;
	ecMoon.latitude = moon.latitude_deg;
	ecMoon.distance = moon.radius;
	eqMoon.rightAscension = moon.rektaszension;
	eqMoon.declination = moon.declination;
	ecSun = GCAstroEngine::getSunCoordinate(arg);
	eqSun = EARTHDATA::eclipticalToEquatorialCoords(&this->ecSun, arg);

    switch (valueType)
    {
        /*case Longitude:
            return GCMath::putIn180(ecMoon.longitude - ecSun.longitude);
        case Latitude:
            return GCMath::putIn180(ecMoon.latitude - ecSun.latitude);
        case Azimuth:
            if (obs != null)
            {
                coordMoon.makeTopocentric(obs);
                GPAstroEngine.calcHorizontal(coordMoon, obs);

                GPAstroEngine.calcHorizontal(coordSun, obs);
                return GCMath::putIn180(coordMoon.azimuth - coordSun.azimuth);
            }
            break;
        case Elevation:
            if (obs != null)
            {
                coordMoon.makeTopocentric(obs);
                GPAstroEngine.calcHorizontal(coordMoon, obs);
                GPAstroEngine.calcHorizontal(coordSun, obs);
                return GPMath.putIn180(coordMoon.elevation - coordSun.elevation);
            }
            break;
        case DeclinationTopo:
            if (obs != null)
            {
                coordMoon.makeTopocentric(obs);
                return GCMath::putIn180(eqMoon.declination - eqSun.declination);
            }
            break;
        case RightAscessionTopo:
            if (obs != null)
            {
                coordMoon.makeTopocentric(obs);
                return GPMath.putIn180(eqMoon.rightAscension - eqSun.rightAscension);
            }
            break;
        case Declination:
            return GCMath::putIn180(eqMoon.declination - eqSun.declination);
        case TithiDistance:
            {
                double tithi = getTithiDouble();
                tithi = 0.51 - GCMath::Abs(tithi - GCMath::Floor(tithi) - 0.5);
                return tithi;
            }
        case YogaDistance:
            {
                double yoga = getYogaDouble(arg);
                yoga = 0.51 - GCMath::Abs(yoga - GCMath::Floor(yoga) - 0.5);
                return yoga;
            }
        case RightAscession:
            return GCMath::putIn180(coordMoon.right_ascession - coordSun.right_ascession);
        case RightAscessionAbs:
            return GCMath::Abs(GCMath::putIn180(eqMoon.rightAscension - eqSun.rightAscension));
        case ApparentSiderealTime:
            return coordMoon.apparent_sidereal_time;
        case MeanSiderealTime:
            return coordMoon.mean_sidereal_time;
        case DynamicTime:
            return coordMoon.getDynamicTime();
        case Nutation:
            return coordMoon.getNutation();
        case Obliquity:
            return coordMoon.getObliquity();*/
        case AzimuthElevationDistance:
			this->eqMoon = moon.getTopocentricEquatorial(obs, arg);
			this->hzMoon = EARTHDATA::equatorialToHorizontalCoords(&this->eqMoon, obs, arg);
			this->hzSun = EARTHDATA::equatorialToHorizontalCoords(&this->eqSun, obs, arg);
            return GCMath::arcDistanceDeg(hzMoon.azimut, hzMoon.elevation, hzSun.azimut, hzSun.elevation);
        case RigthAscessionDeclinationDistance:
            return GCMath::arcDistanceDeg(eqMoon.rightAscension, eqMoon.declination, eqSun.rightAscension, eqSun.declination);
        case RigthAscessionDeclinationOpositeDistance:
            return GCMath::arcDistanceDeg(180 + eqMoon.rightAscension, -eqMoon.declination, eqSun.rightAscension, eqSun.declination);
        case Tithi:
            return getTithiDouble();
        case Yoga:
            return getYogaDouble(arg);
        default:
            break;
    }

    return 0;
}

double GCFunctorSunMoon::getTithiDouble()
{
    return GCMath::putIn360(ecMoon.longitude - ecSun.longitude - 180) / 30;
}

double GCFunctorSunMoon::getYogaDouble(double day)
{
    double d = GCMath::putIn360(ecMoon.longitude + ecSun.longitude - 2 * GCAyanamsha::GetAyanamsa(day));
    return (d * 3.0) / 40.0;
}

