#include "stdafx.h"
#include "GCDayHours.h"
#include "GCMath.h"

void DAYTIME::SetDayTime(double d)
{
	double time_hr = 0.0;

	// hour
	time_hr = d * 24;
	hour = int( floor(time_hr) );

	// minute
	time_hr -= hour;
	time_hr *= 60;
	min = int( floor(time_hr) );

	// second
	time_hr -= min;
	time_hr *= 60;
	sec = int( floor(time_hr) );

	// miliseconds
	time_hr -= sec;
	time_hr *= 1000;
	mili = int( floor(time_hr) );
}

void DAYTIME::SetValue(int i)
{
	hour = min = sec = mili = i;
}

void DAYTIME::Set(DAYTIME &d)
{
	hour = d.hour;
	min = d.min;
	sec = d.sec;
	mili = d.mili;
}

bool DAYTIME::IsGreaterThan(DAYTIME &dt)
{
	if (hour > dt.hour)
		return TRUE;
	else if (hour < dt.hour)
		return FALSE;

	if (min > dt.min)
		return TRUE;
	else if (min < dt.min)
		return FALSE;

	if (sec > dt.sec)
		return TRUE;
	else if (sec < dt.sec)
		return FALSE;

	if (mili > dt.mili)
		return TRUE;

	return FALSE;
}

bool DAYTIME::IsLessThan(DAYTIME &dt)
{
	if (hour < dt.hour)
		return TRUE;
	else if (hour > dt.hour)
		return FALSE;

	if (min < dt.min)
		return TRUE;
	else if (min > dt.min)
		return FALSE;

	if (sec < dt.sec)
		return TRUE;
	else if (sec > dt.sec)
		return FALSE;

	if (mili < dt.mili)
		return TRUE;

	return FALSE;
}

bool DAYTIME::IsGreaterOrEqualThan(DAYTIME &dt)
{
	if (hour >= dt.hour)
		return TRUE;
	else if (hour < dt.hour)
		return FALSE;

	if (min >= dt.min)
		return TRUE;
	else if (min < dt.min)
		return FALSE;

	if (sec >= dt.sec)
		return TRUE;
	else if (sec < dt.sec)
		return FALSE;

	if (mili >= dt.mili)
		return TRUE;

	return FALSE;
}

bool DAYTIME::IsLessOrEqualThan(DAYTIME &dt)
{
	if (hour <= dt.hour)
		return TRUE;
	else if (hour > dt.hour)
		return FALSE;

	if (min <= dt.min)
		return TRUE;
	else if (min > dt.min)
		return FALSE;

	if (sec <= dt.sec)
		return TRUE;
	else if (sec > dt.sec)
		return FALSE;

	if (mili <= dt.mili)
		return TRUE;

	return FALSE;
}


void DAYTIME::AddMinutes(int mn) 
{
	min += mn;
	while(min < 0) { min += 60; hour--;}
	while(min > 59) { min -= 60; hour++;}
}

double DAYTIME::GetDayTime()
{
	return ((double(hour)*60.0 + double(min))*60.0 + double(sec)) / 86400.0;
}



////////////////////////////////////////////////////////////////
//
//  Conversion time from DEGREE fromat to H:M:S:MS format
//
//  time - output
//  time_deg - input time in range 0 deg to 360 deg
//             where 0 deg = 0:00 AM and 360 deg = 12:00 PM
//
void DAYTIME::SetDegTime(double time_deg)
{
	double time_hr = 0.0;

	time_deg = GCMath::putIn360(time_deg);

	// hour
	time_hr = time_deg / 360 * 24;
	hour = int( floor(time_hr) );

	// minute
	time_hr -= hour;
	time_hr *= 60;
	min = int( floor(time_hr) );

	// second
	time_hr -= min;
	time_hr *= 60;
	sec = int( floor(time_hr) );

	// miliseconds
	time_hr -= sec;
	time_hr *= 1000;
	mili = int( floor(time_hr) );
}

void DAYTIME::ToLongTimeString(TString &str)
{
	str.Format("%02d:%02d:%02d", hour, min, sec);
}
