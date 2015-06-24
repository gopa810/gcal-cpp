#include "stdafx.h"
#include "GCMath.h"

GCMath::GCMath(void)
{
}


GCMath::~GCMath(void)
{
}

// pi
double GCMath::PI = 3.1415926535897932385;
// 2 * pi
double GCMath::PI2 = 6.2831853071795864769;
// pi / 180
double GCMath::RADS = 0.0174532925199432958;

double GCMath::AU = 149597869.0;


/////////////////////////////////////
// input value: arc in degrees

double GCMath::cosDeg(double x)
{
	return cos( x * GCMath::RADS);
}

/////////////////////////////////////
// input value: arc in degrees

double GCMath::sinDeg(double x)
{
	return sin( x * GCMath::RADS);
}

double GCMath::arccosDeg(double x)
{
	return acos(x) / GCMath::RADS;
}

/////////////////////////////////////
// input value: arc in degrees
// it is calculating arctan(x/y)
// with testing values

double GCMath::arcTan2Deg(double x, double y)
{
	return atan2(x, y) / GCMath::RADS;
}

/////////////////////////////////////
// input value: arc in degrees
// output value: tangens 

double GCMath::tanDeg(double x)
{
	return tan(x * GCMath::RADS);
}

/////////////////////////////////////
// input value: -1.0 .. 1.0
// output value: -180 deg .. 180 deg

double GCMath::arcSinDeg(double x)
{
	return asin(x) / GCMath::RADS;
}



double GCMath::arcTanDeg(double x)
{
	return atan(x) / GCMath::RADS;
}

// modulo 1

double GCMath::putIn1(double v)
{
	double v2 = v - floor(v);
	while (v2 < 0.0)
		v2 += 1.0;
	while (v2 > 1.0)
		v2 -= 1.0;
	return v2;
}

double GCMath::putIn24(double id)
{
	double d = id;
	while(d >= 24.0)
		d -= 24.0;
	while(d < 0.0)
		d += 24.0;
	return d;
}

// modulo 360

double GCMath::putIn360(double id)
{
	double d = id;
	while(d >= 360.0)
		d -= 360.0;
	while(d < 0.0)
		d += 360.0;
	return d;
}

// modulo 360 but in range -180deg .. 180deg
// used for comparision values around 0deg
// so difference between 359deg and 1 deg 
// is not 358 degrees, but 2 degrees (because 359deg = -1 deg)

double GCMath::putIn180(double in_d)
{
	double d = in_d;

	while(d < -180.0)
	{
		d += 360.0;
	}
	while(d > 180.0)
	{
		d -= 360.0;
	}

	return d;
}



// sign of the number
// -1: number is less than zero
// 0: number is zero
// +1: number is greater than zero

int GCMath::getSign(double d)
{
	if (d > 0.0)
		return 1;
	if (d < 0.0)
		return -1;
	return 0;
}

double GCMath::deg2rad(double x)
{
	return x * GCMath::RADS;
}

double GCMath::rad2deg(double x)
{
	return x / GCMath::RADS;
}

double GCMath::getFraction(double x) 
{
	return x - floor(x);
}

double GCMath::Max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

double GCMath::Min(double a, double b)
{
	if (a < b)
		return a;
	return b;
}

double GCMath::Fabs(double x)
{
	return fabs(x);
}
