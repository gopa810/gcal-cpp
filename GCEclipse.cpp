#include "stdafx.h"
#include "GCEclipse.h"
#include "GCMath.h"
#include "GCDynamicTime.h"
#include "GCFunctorSunMoon.h"
#include "GCSearch.h"

GCEclipse::GCEclipse(void)
{
}


GCEclipse::~GCEclipse(void)
{
}


/// <summary>
/// Calculates times relevant for sun eclipse.
/// returned array of double values has this meaning:
/// times[0] - start of partial eclipse or -1 if no eclipse
/// times[1] - start of total eclipse or -1 if no total eclipse
/// times[2] - center of eclipse or -1 if no eclipse
/// times[3] - end of total eclipse or -1 if no total eclipse
/// times[4] - end of partial eclipse or -1 if no eclipse
/// 
/// here are possible scenarios of eclipse
/// 
/// times[2] == -1 : no eclipse
/// 
/// times[2] != -1, times[1] == -1 : only partial eclipse
/// 
/// times[2] != -1, times[1] != -1 :  partial and total eclipse
/// </summary>
/// <param name="startDate">This is date returned by NextEclipse() function. 
/// It is approximate date of center of the eclipse.</param>
/// <param name="obs">Observer of eclipse</param>
/// <param name="times">Array of 5 double output values. Times are in Universal Time</param>
void GCEclipse::CalculateTimesSunEclipse(double startDate, EARTHDATA obs, double * times)
{
    double dStart = startDate - 0.5;
    double dEnd = startDate + 0.5;
    //double jd = 0;
    double deltaT = GCDynamicTime::GetDeltaT(startDate);
    double sunMoonPartialDist;
    double sunMoonFullDist;
    double m1, m2;
    double oneMin = 1 / 1440.0;

    GCFunctorSunMoon * functor = new GCFunctorSunMoon(GCFunctorSunMoonValueType::AzimuthElevationDistance, obs);
    GCSearch * sr = new GCSearch(functor);
    sr->setSilentErrorValue(true, -1);

    for (int i = 0; i < 5; i++)
    {
        times[i] = -1;
    }
    times[2] = sr->findMinimum(startDate - 0.5, startDate + 0.5);
    if (sr->wasError())
    {
        times[2] = -1;
        return;
    }
	m1 = GCMath::arcSinDeg(GCMath::MOON_RADIUS / functor->ecMoon.distance);
	m2 = GCMath::arcSinDeg(GCMath::SUN_RADIUS / functor->ecSun.distance);
    //m1 = functor.getMoonCoordinates().getVisibleAngle(GCMath::MOON_RADIUS);
    //m2 = functor.getSunCoordinates().getVisibleAngle(GCMath::SUN_RADIUS);
    sunMoonFullDist = m1 - m2;
    sunMoonPartialDist = m1 + m2;

    if (times[2] > sunMoonPartialDist)
        times[2] = -1;

    if (times[2] > 0)
    {
        times[0] = sr->findSet(startDate - 0.5, times[2], sunMoonPartialDist);
        if (times[0] > 0)
            times[1] = sr->findSet(times[0], times[2], sunMoonFullDist);
        if (GCMath::Abs(times[1] - times[2]) < oneMin)
            times[1] = -1;
        times[4] = sr->findRise(times[2], startDate + 0.5, sunMoonPartialDist);
        if (times[4] > 0)
            times[3] = sr->findRise(times[2], times[4], sunMoonFullDist);
        if (GCMath::Abs(times[3] - times[4]) < oneMin)
            times[3] = -1;
    }

    for (int i = 0; i < 5; i++)
    {
        if (times[i] > 0)
            times[i] -= deltaT;
    }

	delete functor;
}

/// <summary>
/// 
/// </summary>
/// <param name="startDate"></param>
/// <param name="obs"></param>
/// <param name="times">Array of 9 double values (Julian day - universal time)
/// Meaning of items in array:
/// index      meaning
/// -----------------------------------
/// 0        start of partial penumbral eclipse
/// 1        start of total penumbral eclipse
/// 2        start of partial full eclipse
/// 3        start of total eclipse
/// 4        center of eclipse (not necessary total)
/// 5        end of total eclipse
/// 6        end of partial full eclipse
/// 7        end of total penumbral eclipse
/// 8        end of partial penumbral eclipse
/// 
/// Every value: if value is less than 0, then
/// time of this has not occured
/// 
/// if times[4] is less than  0, then no eclipse occured in given date
/// </param>
void GCEclipse::CalculateTimesMoonEclipse(double startDate, EARTHDATA obs, double * times)
{
    double dStart = startDate - 0.5;
    double dEnd = startDate + 0.5;
    double deltaT = GCDynamicTime::GetDeltaT(startDate) / 86400;
    double fullShadowOuter;
    double fullShadowInner;
    double penumbralShadowInner;
    double penumbralShadowOuter;
    double m1, m2, m3, fullShadowRadius;
    double penumShadowRadius;

    GCFunctorSunMoon * functor = new GCFunctorSunMoon(GCFunctorSunMoonValueType::RigthAscessionDeclinationOpositeDistance, obs);
    GCSearch * sr = new GCSearch(functor);
    sr->setSilentErrorValue(true, -1);

    // nullify
    for (int i = 0; i < 9; i++)
    {
        times[i] = -1;
    }

    // get time of possible eclipse
    times[4] = sr->findMinimum(startDate - 0.5, startDate + 0.5);
    if (sr->wasError())
    {
        times[4] = -1;
        return;
    }
    double dSun, dMoon;
    double oneMin = 1/1440.0;

    // 
    // calculating shadow angles
	dSun = functor->ecSun.distance;
	dMoon = functor->ecMoon.distance;

    double sinalpha = (GCMath::SUN_RADIUS - GCMath::EARTH_RADIUS) / dSun;
    double dx = GCMath::EARTH_RADIUS / sinalpha - dMoon;

    // diameter of full shadow in km in distance of moon
    fullShadowRadius = sinalpha * dx;
	m1 = GCMath::arcSinDeg(GCMath::MOON_RADIUS / functor->ecMoon.distance);
	m2 = GCMath::arcSinDeg(fullShadowRadius / functor->ecMoon.distance);
    //m1 = functor.getMoonCoordinates().getVisibleAngle(GCMath::MOON_RADIUS);
    //m2 = functor.getMoonCoordinates().getVisibleAngle(fullShadowRadius);

    sinalpha = (GCMath::SUN_RADIUS + GCMath::EARTH_RADIUS) / dSun;
    dx = GCMath::EARTH_RADIUS / sinalpha;
    penumShadowRadius = (dx + dMoon) * sinalpha;
	m3 = GCMath::arcSinDeg(penumShadowRadius / functor->ecMoon.distance);
    //m3 = functor.getMoonCoordinates().getVisibleAngle(penumShadowRadius);

    fullShadowInner = m2 - m1;
    fullShadowOuter = m1 + m2;
    penumbralShadowOuter = m3 + m1;
    penumbralShadowInner = m3 - m1;

    dx = functor->getDoubleValue(times[4]);
    if (dx > penumbralShadowOuter)
        times[4] = -1;

    // start of penumbral
    if (times[4] > 0)
    {
        times[0] = sr->findSet(times[4] - 0.5, times[4], penumbralShadowOuter);
        if (times[0] > 0 && penumbralShadowInner > fullShadowOuter)
            times[1] = sr->findSet(times[0], times[4], penumbralShadowInner);
        if (GCMath::Abs(times[1] - times[4]) < oneMin)
            times[1] = -1;

        // full start
        if (times[0] > 0)
            times[2] = sr->findSet(times[0], times[4], fullShadowOuter);
        if (GCMath::Abs(times[2] - times[4]) < oneMin)
            times[2] = -1;
        if (times[2] > 0)
            times[3] = sr->findSet(times[2], times[4], fullShadowInner);
        if (GCMath::Abs(times[3] - times[4]) < oneMin)
            times[3] = -1;

        // end of penumbral
        times[8] = sr->findRise(times[4], times[4] + 0.5, penumbralShadowOuter);
        if (times[8] > 0 && penumbralShadowInner > fullShadowOuter)
            times[7] = sr->findRise(times[4], times[8], penumbralShadowInner);

        // full end
        if (times[8] > 0)
            times[6] = sr->findRise(times[4], times[8], fullShadowOuter);
        if (GCMath::Abs(times[6] - times[8]) < oneMin)
            times[6] = -1;
        if (times[6] > 0)
            times[5] = sr->findRise(times[4], times[6], fullShadowInner);
        if (GCMath::Abs(times[5] - times[6]) < oneMin)
            times[5] = -1;
    }

    for (int i = 0; i < 9; i++)
    {
        if (times[i] > 0)
            times[i] -= deltaT;
    }


	for(double s = times[0]; s <= times[8]; s+= (times[8] - times[0]) / 20)
	{
		TRACE2(" time[%f] = %f\n", s, functor->getDoubleValue(s));
	}

	delete functor;
}

