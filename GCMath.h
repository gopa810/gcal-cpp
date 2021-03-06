#pragma once

#include <math.h>

class GCMath
{
public:
	GCMath(void);
	~GCMath(void);

	static double PI;
	static double PI2;
	static double RADS;
	static double AU;
	static double EARTH_RADIUS, SUN_RADIUS, MOON_RADIUS;
	static double J1999, J2000;

	static double sinDeg(double x);
	static double cosDeg(double x);
	static double arccosDeg(double x);
	static double arcTan2Deg(double x, double y);
	static double tanDeg(double x);
	static double arcSinDeg(double x);
	static double arcTanDeg(double x);
	static double putIn1(double v);
	static double putIn24(double id);
	static double putIn360(double id);
	static double putIn180(double in_d);
	static int getSign(double d);
	static double deg2rad(double x);
	static double rad2deg(double x);
	static double getFraction(double x);
	static double Max(double a, double b);
	static double Min(double a, double b);
	static double Abs(double d);
	static double Floor(double d);
	static double arcDistance(double lon1, double lat1, double lon2, double lat2);
    static double arcDistanceDeg(double lon1, double lat1, double lon2, double lat2);

};

