#include "stdafx.h"
#include "gmath.h"
#include "math.h"
#include "level_0.h"

double SwissGetMoonLongitude(double jdate);
double SwissGetSunLongitude(double jdate);

// PORTABLE 
/*********************************************************************************/
/*  Procedures for calculation                                                   */
/*                                                                               */
/*    - moon longitude (MoonCalc)                                                */
/*    - moonrise time (CalcMoonTimes)                                            */
/*    - moonset time                                                             */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/


void correct_position(MOONDATA & position, double jdate, double latitude, double longitude, double height);




void calc_geocentric(double &longitude, double &latitude, double &rektaszension, double &declination, double date)
{
	//var
	double epsilon; //: extended;
	double delta_phi; //: extended;
	double alpha,delta; //: extended;

	EARTHDATA::calc_epsilon_phi(date, delta_phi, epsilon);
	longitude = put_in_360(longitude+delta_phi);

	alpha = arctan2_d( sin_d(longitude)*cos_d(epsilon)-tan_d(latitude)*sin_d(epsilon), cos_d(longitude));

	delta = arcsin_d( sin_d(latitude)*cos_d(epsilon)+cos_d(latitude)*sin_d(epsilon)*sin_d(longitude));

	rektaszension = alpha;
	declination = delta;

	double xg, yg, zg;

	xg = cos_d(longitude)*cos_d(latitude);
	yg = sin_d(longitude)*cos_d(latitude);
	zg = sin_d(latitude);

	alpha = arctan2_d(yg*cos_d(epsilon) - zg*sin_d(epsilon), cos_d(longitude)*cos_d(latitude));
}


/*
#define _rise    0
#define _set     1
#define _transit 2


//  function interpolation(y1,y2,y3,n: extended):extended;  
double interpolation(double y1, double y2, double y3, double n)
{
	double a, b, c;

	a = y2-y1;
	b = y3-y2;
	if (a>100)
		a -= 360;
	if (a<-100)
		a += 360;
	if (b>100)
		b -= 360;
	if (b<-100)
		b += 360;
	c = b - a;
	return y2+0.5*n*(a+b+n*c);
}

double correction(double m, int kind, MOONDATA pos1, MOONDATA pos2, MOONDATA pos3, double longitude, double latitude, double theta0, double h0)
{
	double alpha,delta,h, height;

	alpha = interpolation(pos1.rektaszension,
                       pos2.rektaszension,
                       pos3.rektaszension,
                       m);
	delta = interpolation(pos1.declination,
                       pos2.declination,
                       pos3.declination,
                       m);
	h = put_in_360((theta0+360.985647*m)-longitude-alpha);
	if (h>180) 
		h = h-360;

	height = arcsin_d(sin_d(latitude)*sin_d(delta)
                   +cos_d(latitude)*cos_d(delta)*cos_d(h));

	switch(kind)
	{
		case 0:   return (-h/360);
		case 1: case 2: return ((height-h0)/(360.0*cos_d(delta)*cos_d(latitude)*sin_d(h)));
		default: return 0;
	}
}*/

// star_time(double h)
// correct_position
// double frac(double)

double star_time(double date)
{
	double jd, t;
	double delta_phi, epsilon;

	jd = date;
	t =(jd-2451545.0)/36525.0;
	EARTHDATA::calc_epsilon_phi(date,delta_phi,epsilon);
	return put_in_360(280.46061837+360.98564736629*(jd-2451545.0)+
                     t*t*(0.000387933-t/38710000)+
                     delta_phi*cos_d(epsilon) );
}

double MoonDistance(double jdate)
{
	const int arg_lr[60][4] = {
		 { 0, 0, 1, 0},		 { 2, 0,-1, 0},		 { 2, 0, 0, 0},		 { 0, 0, 2, 0},		 { 0, 1, 0, 0},
		 { 0, 0, 0, 2},		 { 2, 0,-2, 0},		 { 2,-1,-1, 0},		 { 2, 0, 1, 0},		 { 2,-1, 0, 0},
		 { 0, 1,-1, 0},		 { 1, 0, 0, 0},		 { 0, 1, 1, 0},		 { 2, 0, 0,-2},		 { 0, 0, 1, 2},
		 { 0, 0, 1,-2},		 { 4, 0,-1, 0},		 { 0, 0, 3, 0},		 { 4, 0,-2, 0},		 { 2, 1,-1, 0},
		 { 2, 1, 0, 0},		 { 1, 0,-1, 0},		 { 1, 1, 0, 0},		 { 2,-1, 1, 0},		 { 2, 0, 2, 0},
		 { 4, 0, 0, 0},		 { 2, 0,-3, 0},		 { 0, 1,-2, 0},		 { 2, 0,-1, 2},		 { 2,-1,-2, 0},
		 { 1, 0, 1, 0},		 { 2,-2, 0, 0},		 { 0, 1, 2, 0},		 { 0, 2, 0, 0},		 { 2,-2,-1, 0},
		 { 2, 0, 1,-2},		 { 2, 0, 0, 2},		 { 4,-1,-1, 0},		 { 0, 0, 2, 2},		 { 3, 0,-1, 0},
		 { 2, 1, 1, 0},		 { 4,-1,-2, 0},		 { 0, 2,-1, 0},		 { 2, 2,-1, 0},		 { 2, 1,-2, 0},
		 { 2,-1, 0,-2},		 { 4, 0, 1, 0},		 { 0, 0, 4, 0},		 { 4,-1, 0, 0},		 { 1, 0,-2, 0},
		 { 2, 1, 0,-2},		 { 0, 0, 2,-2},		 { 1, 1, 1, 0},		 { 3, 0,-2, 0},		 { 4, 0,-3, 0},
		 { 2,-1, 2, 0},		 { 0, 2, 1, 0},		 { 1, 1,-1, 0},		 { 2, 0, 3, 0},		 { 2, 0,-1,-2}
	   };

	  const int sigma_r[60] = {
	   -20905355,		-3699111,		-2955968,		 -569925,		   48888,		   -3149,		  246158,
		 -152138,		 -170733,		 -204586,		 -129620,		  108743,		  104755,		   10321,
			   0,		   79661,		  -34782,		  -23210,		  -21636,		   24208,		   30824,
		   -8379,		  -16675,		  -12831,		  -10445,		  -11650,		   14403,		   -7003,
			   0,		   10056,			6322,		   -9884,			5751,			   0,		   -4950,
			4130,			   0,		   -3958,			   0,			3258,			2616,		   -1897,
		   -2117,			2354,			   0,			   0,		   -1423,		   -1117,		   -1571,
		   -1739,			   0,		   -4421,			   0,			   0,			   0,			   0,
			1165,			   0,			   0,			8752
				  };

	double temp, t,d,m,ms,f,e,ls,sr;// : extended;

	t = (jdate - 2451545.0)/36525.0;

	//(* mean elongation of the moon  
	d = 297.8502042+(445267.1115168+(-0.0016300+(1.0/545868-1.0/113065000*t)*t)*t)*t;

	//(* mean anomaly of the sun  
	m =357.5291092+(35999.0502909+(-0.0001536+1.0/24490000*t)*t)*t;

	//(* mean anomaly of the moon  
	ms =134.9634114+(477198.8676313+(0.0089970+(1.0/69699-1.0/1471200*t)*t)*t)*t;

	//(* argument of the longitude of the moon  
	f =93.2720993+(483202.0175273+(-0.0034029+(-1.0/3526000+1.0/863310000*t)*t)*t)*t;

	//(* correction term due to excentricity of the earth orbit  
	e = 1.0+(-0.002516-0.0000074*t)*t;

	//(* mean longitude of the moon  
	ls =218.3164591+(481267.88134236+(-0.0013268+(1.0/538841-1.0/65194000*t)*t)*t)*t;
	
	sr =0;

	int i;
	for(i = 0; i < 60; i++)
	{
		temp =sigma_r[i]*cos_d( arg_lr[i][0]*d
							   +arg_lr[i][1]*m
							   +arg_lr[i][2]*ms
							   +arg_lr[i][3]*f);
		if (abs(arg_lr[i][1])==1) temp =temp*e;
		if (abs(arg_lr[i][1])==2) temp =temp*e*e;
		sr =sr+temp;
	}
	
	
	return 385000.56+sr/1000;

}

#define AU 149597869.0
//

void correct_parallax(MOONDATA & position, double jdate, double latitude, double longitude)
{
	double moondist = MoonDistance(jdate);
	double longit = position.longitude_deg + longitude;
	double newval = cos_d(longit) * arctan2_d(6378.14, moondist);

	position.longitude_deg = put_in_360(position.longitude_deg + newval);
}

void correct_position(MOONDATA & position, double jdate, double latitude, double longitude, double height)
{
	double u,h,delta_alpha;
	double rho_sin, rho_cos;
	const double b_a=0.99664719;

	u =arctan_d(b_a*b_a*tan_d(latitude));
	rho_sin =b_a*sin_d(u)+height/6378140.0*sin_d(latitude);
	rho_cos =cos_d(u)+height/6378140.0*cos_d(latitude);

	position.parallax = arcsin_d(sin_d(8.794/3600)/(MoonDistance(jdate) / AU));

	h = star_time(jdate)-longitude-position.rektaszension;
	delta_alpha = arctan_d(
                (-rho_cos*sin_d(position.parallax)*sin_d(h))/
                (cos_d(position.declination)-
                  rho_cos*sin_d(position.parallax)*cos_d(h)));
	position.rektaszension =position.rektaszension+delta_alpha;
	position.declination =arctan_d(
      (( sin_d(position.declination)
        -rho_sin*sin_d(position.parallax))*cos_d(delta_alpha))/
      ( cos_d(position.declination)
       -rho_cos*sin_d(position.parallax)*cos_d(h)));
}

double frac(double x) 
{
	return x - floor(x);
}

void calc_horizontal(MOONDATA &coord, double date, double longitude, double latitude)
{
	double h;

	h = put_in_360(star_time(date)-coord.rektaszension+longitude);
	
	coord.azimuth = rad2deg( atan2(sin_d(h),
                           cos_d(h)*sin_d(latitude)-
                           tan_d(coord.declination)*cos_d(latitude) ));
	
	coord.elevation = rad2deg(asin(sin_d(latitude)*sin_d(coord.declination)+
                            cos_d(latitude)*cos_d(coord.declination)*cos_d(h)));
}

//==================================================================================
//
//==================================================================================

double MoonCalcElevation(EARTHDATA e, VCTIME vc)
{
	MOONDATA moon;
	double d = vc.GetJulianComplete();
	moon.Calculate(d, e);
	correct_position(moon, d, e.latitude_deg, e.longitude_deg, 0);
	calc_horizontal(moon, d, e.longitude_deg, e.latitude_deg);

	return moon.elevation;
}

//==================================================================================
//
//==================================================================================

void CalcMoonTimes(EARTHDATA e, VCTIME vc, double nDaylightSavingShift, DAYTIME &rise, DAYTIME &set)
{
	double UT;
	int i;
	double prev_elev;
	int nType, nFound = 0;
	double a, ae, b, be, c, ce, elev;

	rise.SetValue(-1);
	set.SetValue(-1);

	// inicializacia prvej hodnoty ELEVATION
	vc.shour = (-nDaylightSavingShift - 1.0) / 24.0;
	prev_elev = MoonCalcElevation(e, vc);

	// prechod cez vsetky hodiny
	for(UT = (- 0.1 - nDaylightSavingShift); UT <= (24.1 - nDaylightSavingShift); UT+=1.0)
	{
		vc.shour = UT / 24.0;
		elev = MoonCalcElevation(e, vc);
		
		if (prev_elev * elev <= 0.0)
		{
			if (prev_elev <= 0.0)
			{
				nType = 0x1;
			}
			else
			{
				nType = 0x2;
			}
			
			a = UT - 1.0;
			ae = prev_elev;
			b = UT;
			be = elev;
			for(i = 0; i < 20; i++)
			{
				c = (a + b) / 2.0;
				vc.shour = c / 24.0;
				ce = MoonCalcElevation(e, vc);
				if (ae*ce <= 0.0)
				{
					be = ce;
					b = c;
				}
				else
				{
					ae = ce;
					a = c;
				}
			}

			if (nType == 1)
			{
				rise.SetDayTime((c + nDaylightSavingShift)/24.0);
			}
			else
			{
				set.SetDayTime((c + nDaylightSavingShift)/24.0);
			}

			nFound |= nType;

			if (nFound == 0x3)
				break;
		}

		prev_elev = elev;
	}

}



