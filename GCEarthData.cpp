#include "stdafx.h"
#include "GCEarthData.h"
#include "GCMath.h"
#include "GCAyanamsha.h"
#include "TTimeZone.h"
#include "GCStrings.h"

EARTHDATA::EARTHDATA()
{
	obs = 0;
	longitude_deg = 0.0;
	latitude_deg = 0.0;
	tzone = 0.0;
	dst = 0;
}

int EARTHDATA::ToString(TString &str)
{
	str.Format("%s: %s  %s: %s  %s: %s", 
		GCStrings::getString(10).c_str(), EARTHDATA::GetTextLatitude(latitude_deg), 
		GCStrings::getString(11).c_str(), EARTHDATA::GetTextLongitude(longitude_deg),
		GCStrings::getString(12).c_str(), TTimeZone::GetTimeZoneOffsetText(tzone));

	return 1;
}

void EARTHDATA::calc_epsilon_phi(double date, double & delta_phi, double & epsilon)
{
	const int arg_mul[31][5] = {
		 { 0, 0, 0, 0, 1},
		 {-2, 0, 0, 2, 2},
		 { 0, 0, 0, 2, 2},
		 { 0, 0, 0, 0, 2},
		 { 0, 1, 0, 0, 0},
		 { 0, 0, 1, 0, 0},
		 {-2, 1, 0, 2, 2},
		 { 0, 0, 0, 2, 1},
		 { 0, 0, 1, 2, 2},
		 {-2,-1, 0, 2, 2},
		 {-2, 0, 1, 0, 0},
		 {-2, 0, 0, 2, 1},
		 { 0, 0,-1, 2, 2},
		 { 2, 0, 0, 0, 0},
		 { 0, 0, 1, 0, 1},
		 { 2, 0,-1, 2, 2},
		 { 0, 0,-1, 0, 1},
		 { 0, 0, 1, 2, 1},
		 {-2, 0, 2, 0, 0},
		 { 0, 0,-2, 2, 1},
		 { 2, 0, 0, 2, 2},
		 { 0, 0, 2, 2, 2},
		 { 0, 0, 2, 0, 0},
		 {-2, 0, 1, 2, 2},
		 { 0, 0, 0, 2, 0},
		 {-2, 0, 0, 2, 0},
		 { 0, 0,-1, 2, 1},
		 { 0, 2, 0, 0, 0},
		 { 2, 0,-1, 0, 1},
		 {-2, 2, 0, 2, 2},
		 { 0, 1, 0, 0, 1}
					   };
	const int arg_phi[31][2] = {
		 {-171996,-1742},
		 { -13187,  -16},
		 {  -2274,   -2},
		 {   2062,    2},
		 {   1426,  -34},
		 {    712,    1},
		 {   -517,   12},
		 {   -386,   -4},
		 {   -301,    0},
		 {    217,   -5},
		 {   -158,    0},
		 {    129,    1},
		 {    123,    0},
		 {     63,    0},
		 {     63,    1},
		 {    -59,    0},
		 {    -58,   -1},
		 {    -51,    0},
		 {     48,    0},
		 {     46,    0},
		 {    -38,    0},
		 {    -31,    0},
		 {     29,    0},
		 {     29,    0},
		 {     26,    0},
		 {    -22,    0},
		 {     21,    0},
		 {     17,   -1},
		 {     16,    0},
		 {    -16,    1},
		 {    -15,    0}
		};
	const int arg_eps[31][2] = {
		{ 92025,   89},
		{  5736,  -31},
		{   977,   -5},
		{  -895,    5},
		{    54,   -1},
		{    -7,    0},
		{   224,   -6},
		{   200,    0},
		{   129,   -1},
		{   -95,    3},
		{     0,    0},
		{   -70,    0},
		{   -53,    0},
		{     0,    0},
		{   -33,    0},
		{    26,    0},
		{    32,    0},
		{    27,    0},
		{     0,    0},
		{   -24,    0},
		{    16,    0},
		{    13,    0},
		{     0,    0},
		{   -12,    0},
		{     0,    0},
		{     0,    0},
		{   -10,    0},
		{     0,    0},
		{    -8,    0},
		{     7,    0},
		{     9,    0}
	};

	double t,omega;
	double d,m,ms,f,s, l, ls;
	int i;
	double epsilon_0,delta_epsilon;

	t = ( date -2451545.0)/36525;

	// longitude of rising knot
	omega =GCMath::putIn360(125.04452+(-1934.136261+(0.0020708+1.0/450000*t)*t)*t);

	if (1)
	{
 // (*@/// delta_phi and delta_epsilon - low accuracy *)
  //(* mean longitude of sun (l) and moon (ls) *)
  l = 280.4665+36000.7698*t;
  ls = 218.3165+481267.8813*t;

  //(* correction due to nutation *)
  delta_epsilon =9.20*GCMath::cosDeg(omega)+0.57*GCMath::cosDeg(2*l)+0.10*GCMath::cosDeg(2*ls)-0.09*GCMath::cosDeg(2*omega);

  //(* longitude correction due to nutation *)
  delta_phi =(-17.20*GCMath::sinDeg(omega)-1.32*GCMath::sinDeg(2*l)-0.23*GCMath::sinDeg(2*ls)+0.21*GCMath::sinDeg(2*omega))/3600;
	}
	else
	{
	// mean elongation of moon to sun
	d = GCMath::putIn360(297.85036+(445267.111480+(-0.0019142+t/189474)*t)*t);

	// mean anomaly of the sun
	m =GCMath::putIn360(357.52772+(35999.050340+(-0.0001603-t/300000)*t)*t);

	// mean anomaly of the moon
	ms =GCMath::putIn360(134.96298+(477198.867398+(0.0086972+t/56250)*t)*t);

	// argument of the latitude of the moon
	f = GCMath::putIn360(93.27191+(483202.017538+(-0.0036825+t/327270)*t)*t);

	delta_phi = 0;
	delta_epsilon = 0;

	for(i=0;i<31;i++)
	{
		s= arg_mul[i][0]*d
		   +arg_mul[i][1]*m
		   +arg_mul[i][2]*ms
		   +arg_mul[i][3]*f
		   +arg_mul[i][4]*omega;
		delta_phi=delta_phi+(arg_phi[i][0]+arg_phi[i][1]*t*0.1)*GCMath::sinDeg(s);
		delta_epsilon=delta_epsilon+(arg_eps[i][0]+arg_eps[i][1]*t*0.1)*GCMath::cosDeg(s);
	}

	delta_phi=delta_phi*0.0001/3600;
	delta_epsilon=delta_epsilon*0.0001/3600;
	}
	// angle of ecliptic
	epsilon_0=84381.448+(-46.8150+(-0.00059+0.001813*t)*t)*t;

	epsilon=(epsilon_0+delta_epsilon)/3600;

}

GCEquatorialCoords EARTHDATA::eclipticalToEquatorialCoords(GCEclipticalCoords * ecc, double date)
{
	//var
	GCEquatorialCoords eqc;

	double epsilon; //: extended;
	double delta_phi; //: extended;
	double alpha,delta; //: extended;

	EARTHDATA::calc_epsilon_phi(date, delta_phi, epsilon);

	ecc->longitude = GCMath::putIn360(ecc->longitude+delta_phi);

	eqc.rightAscension = GCMath::arcTan2Deg( GCMath::sinDeg(ecc->longitude)*GCMath::cosDeg(epsilon)-GCMath::tanDeg(ecc->latitude)*GCMath::sinDeg(epsilon), GCMath::cosDeg(ecc->longitude));

	eqc.declination = GCMath::arcSinDeg( GCMath::sinDeg(ecc->latitude)*GCMath::cosDeg(epsilon)+GCMath::cosDeg(ecc->latitude)*GCMath::sinDeg(epsilon)*GCMath::sinDeg(ecc->longitude));

	return eqc;
}


GCHorizontalCoords EARTHDATA::equatorialToHorizontalCoords(GCEquatorialCoords * eqc, EARTHDATA obs, double date)
{
	double h;
	GCHorizontalCoords hc;

	h = GCMath::putIn360(EARTHDATA::star_time(date) - eqc->rightAscension + obs.longitude_deg);
	
	hc.azimut = GCMath::rad2deg( atan2(GCMath::sinDeg(h),
                           GCMath::cosDeg(h)*GCMath::sinDeg(obs.latitude_deg)-
                           GCMath::tanDeg(eqc->declination)*GCMath::cosDeg(obs.latitude_deg) ));
	
	hc.elevation = GCMath::rad2deg(asin(GCMath::sinDeg(obs.latitude_deg)*GCMath::sinDeg(eqc->declination) +
                            GCMath::cosDeg(obs.latitude_deg)*GCMath::cosDeg(eqc->declination)*GCMath::cosDeg(h)));

	return hc;
}

char * EARTHDATA::GetTextLatitude(double d)
{
	static char inter[16];
	int a0, a1;
	char c0;

	c0 = d < 0.0 ? 'S' : 'N';
	d = d < 0.0 ? -d : d;
	a0 = int(d);
	a1 = int((d - a0)*60 + 0.5);

	sprintf(inter, "%d%c%02d", a0, c0, a1);

	return inter;
}

char * EARTHDATA::GetTextLongitude(double d)
{
	static char inter[16];
	int a0, a1;
	char c0;

	c0 = d < 0.0 ? 'W' : 'E';
	a0 = int(fabs(d));
	a1 = int((fabs(d) - a0)*60 + 0.5);

	sprintf(inter, "%d%c%02d", a0, c0, a1);

	return inter;
}


double EARTHDATA::star_time(double date)
{
	double jd, t;
	double delta_phi, epsilon;

	jd = date;
	t =(jd-2451545.0)/36525.0;
	EARTHDATA::calc_epsilon_phi(date,delta_phi,epsilon);
	return GCMath::putIn360(280.46061837+360.98564736629*(jd-2451545.0)+
                     t*t*(0.000387933-t/38710000)+
                     delta_phi*GCMath::cosDeg(epsilon) );
}

double EARTHDATA::GetHorizontDegrees(double jday)
{
	return GCMath::putIn360(EARTHDATA::star_time(jday) - longitude_deg - GCAyanamsha::GetAyanamsa(jday) + 155);
}

int EARTHDATA::GetNextAscendentStart(VCTIME startDate, VCTIME &nextDate)
{
	double phi = 30.0;
	double l1, l2;
	double jday = startDate.GetJulianComplete();
	double xj;
	VCTIME d;
	d.Set(startDate);
	VCTIME xd;
	double scan_step = 0.05;
	int prev_tit = 0;
	int new_tit = -1;

	l1 = GetHorizontDegrees(jday);

	prev_tit = int(floor(l1/phi));

	int counter = 0;
	while(counter < 20)
	{
		xj = jday;
		xd.Set(d);

		jday += scan_step;
		d.shour += scan_step;
		if (d.shour > 1.0)
		{
			d.shour -= 1.0;
			d.NextDay();
		}

		l2 = GetHorizontDegrees(jday);
		new_tit = int(floor(l2/phi));

		if (prev_tit != new_tit)
		{
			jday = xj;
			d.Set(xd);
			scan_step *= 0.5;
			counter++;
			continue;
		}
		else
		{
			l1 = l2;
		}
	}
	nextDate.Set(d);

	return new_tit;
}



