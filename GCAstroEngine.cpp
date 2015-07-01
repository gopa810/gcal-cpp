#include "stdafx.h"
#include "GCAstroEngine.h"
#include "GCMoonData.h"
#include "GCMath.h"
#include "GCMeeusEarth.h"

GCAstroEngine::GCAstroEngine(void)
{
}


GCAstroEngine::~GCAstroEngine(void)
{
}


GCEclipticalCoords GCAstroEngine::getMoonCoordinates(double julian)
{
	MOONDATA md;
	return md.CalculateEcliptical(julian);
}

GCEclipticalCoords GCAstroEngine::getEarthCoordinates(double date)
{
	GCEclipticalCoords ec;
	double t;
    double current;
	double * e;
    double rx[6];
    int i, j, m;

    t = (date - GCMath::J2000 /*- 0.5*/) / 365250.0;

	try
    {
		// distance
        for (j = 0; j <= 5; j++)
        {
            rx[j] = 0;
			m = GCMeeusEarth::radiusFactorCount(j);
			for(i = 0; i < m; i++)
			{
				e = GCMeeusEarth::radiusFactor(j, i);
                current = e[0] * GCMath::cosDeg(e[1] + e[2] * t);
                rx[j] += current;
                i++;
            };
        }
        ec.distance = (rx[0] + t * (rx[1] + t * (rx[2] + t * (rx[3] + t * (rx[4] + t * rx[5]))))) * 1e-8;

		// longitude
        for (j = 0; j <= 5; j++)
        {
            rx[j] = 0;
			m = GCMeeusEarth::longitudeFactorCount(j);
			for(i = 0; i < m; i++)
			{
				e = GCMeeusEarth::longitudeFactor(j, i);
                current = e[0] * GCMath::cosDeg(e[1] + e[2] * t);
                rx[j] += current;
                i++;
            };
        }
		ec.longitude = (rx[0] + t * (rx[1] + t * (rx[2] + t * (rx[3] + t * (rx[4] + t * rx[5]))))) * 1e-8;

		// latitude
        for (j = 0; j <= 5; j++)
        {
            rx[j] = 0;
			m = GCMeeusEarth::latitudeFactorCount(j);
			for(i = 0; i < m; i++)
			{
				e = GCMeeusEarth::latitudeFactor(j, i);
                current = e[0] * GCMath::cosDeg(e[1] + e[2] * t);
                rx[j] += current;
                i++;
            };
        }
        ec.latitude = (rx[0] + t * (rx[1] + t * (rx[2] + t * (rx[3] + t * (rx[4] + t * rx[5]))))) * 1e-8;

		DynamicToFK5(&ec.longitude, &ec.latitude, t);
    }
    catch(CException * exp)
    {
    }

	ec.longitude = GCMath::putIn360(GCMath::rad2deg(ec.longitude));
	ec.latitude = GCMath::rad2deg(ec.latitude);

	return ec;
}

void GCAstroEngine::DynamicToFK5(double *longitude, double *latitude, double tau)
{
    double lprime, t;
    double delta_l, delta_b;
    t = 10 * tau;
    lprime = *longitude + GCMath::deg2rad(-1.397 - 0.00031 * t) * t;
    delta_l = -GCMath::deg2rad(0.09033 / 3600) + GCMath::deg2rad(0.03916 / 3600) * (GCMath::cosDeg(lprime) + GCMath::sinDeg(lprime)) * GCMath::tanDeg(*latitude);
    delta_b = GCMath::deg2rad(0.03916 / 3600) * (GCMath::cosDeg(lprime) - GCMath::sinDeg(lprime));
    *longitude = *longitude + delta_l;
    *latitude = *latitude + delta_b;
}
GCEclipticalCoords GCAstroEngine::getSunCoordinate(double date)
{
    // calculate ecliptical coordinates
    GCEclipticalCoords ec = getEarthCoordinates(date);

    // convert earth coordinate to sun coordinate 
	double l = ec.longitude + 180;
	double b = -ec.latitude;
	double r = ec.distance;

    // abberation
    // according Meeus Chapter 24.10
    l -= (20.4898 / 3600) / r ;

    GCEclipticalCoords result;
    result.longitude = GCMath::putIn360(l);
    result.latitude = b;
    result.distance = r * GCMath::AU;

    return result;
}