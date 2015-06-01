#include "stdafx.h"
#include "level_0.h"
#include "vedic_ui.h"
#include "strings.h"
#include "TTimeZone.h"
#include "level_3.h"


int GetShowSetVal(int i);

// PORTABLE 
/************************************************************************

Routines for gregorian calendar days, julian calendar, daylight saving time routines, etc.

*************************************************************************/


double GetJulianDay(int year, int month, int day)
{
	int yy = year - int((12 - month) / 10);
	int mm = month + 9;

	if (mm >= 12)
		mm -= 12;

	int k1, k2, k3;
	int j;

	k1 = int (floor(365.25 * (yy + 4712)));
	k2 = int (floor(30.6 * mm + 0.5));
	k3 = int (floor(floor((double)(yy/100)+49)*.75))-38;
	j = k1 + k2 + day + 59;
	if (j > 2299160)
		j -= k3;

	return double(j);
}


double GetJulianDay(VCTIME & date)
{
	// julian day is xxx.0 at 12:00 UTC
	// date.shour is time of event (0..1) = (0..24hour)
	// timezone shifts julian day - east longitude
	return GetJulianDay(date.year, date.month, date.day) 
		- 0.5 + date.shour;
}

char g_year_days[2][367][2] = {
{
	{1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1}, {10,1},
	{11,1}, {12,1}, {13,1}, {14,1}, {15,1}, {16,1}, {17,1}, {18,1}, {19,1}, {20,1},
	{21,1}, {22,1}, {23,1}, {24,1}, {25,1}, {26,1}, {27,1}, {28,1}, {29,1}, {30, 1}, {31,1},

	{1,2}, {2,2}, {3,2}, {4,2}, {5,2}, {6,2}, {7,2}, {8,2}, {9,2}, {10,2},
	{11,2}, {12,2}, {13,2}, {14,2}, {15,2}, {16,2}, {17,2}, {18,2}, {19,2}, {20,2},
	{21,2}, {22,2}, {23,2}, {24,2}, {25,2}, {26,2}, {27,2}, {28,2},

	{1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {6,3}, {7,3}, {8,3}, {9,3}, {10,3},
	{11,3}, {12,3}, {13,3}, {14,3}, {15,3}, {16,3}, {17,3}, {18,3}, {19,3}, {20,3},
	{21,3}, {22,3}, {23,3}, {24,3}, {25,3}, {26,3}, {27,3}, {28,3}, {29,3}, {30, 3}, {31,3},

	{1,4}, {2,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, {8,4}, {9,4}, {10,4},
	{11,4}, {12,4}, {13,4}, {14,4}, {15,4}, {16,4}, {17,4}, {18,4}, {19,4}, {20,4},
	{21,4}, {22,4}, {23,4}, {24,4}, {25,4}, {26,4}, {27,4}, {28,4}, {29,4}, {30, 4},

	{1,5}, {2,5}, {3,5}, {4,5}, {5,5}, {6,5}, {7,5}, {8,5}, {9,5}, {10,5},
	{11,5}, {12,5}, {13,5}, {14,5}, {15,5}, {16,5}, {17,5}, {18,5}, {19,5}, {20,5},
	{21,5}, {22,5}, {23,5}, {24,5}, {25,5}, {26,5}, {27,5}, {28,5}, {29,5}, {30, 5}, {31,5},

	{1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6}, {8,6}, {9,6}, {10,6},
	{11,6}, {12,6}, {13,6}, {14,6}, {15,6}, {16,6}, {17,6}, {18,6}, {19,6}, {20,6},
	{21,6}, {22,6}, {23,6}, {24,6}, {25,6}, {26,6}, {27,6}, {28,6}, {29,6}, {30, 6},

	{1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7}, {7,7}, {8,7}, {9,7}, {10,7},
	{11,7}, {12,7}, {13,7}, {14,7}, {15,7}, {16,7}, {17,7}, {18,7}, {19,7}, {20,7},
	{21,7}, {22,7}, {23,7}, {24,7}, {25,7}, {26,7}, {27,7}, {28,7}, {29,7}, {30, 7}, {31,7},

	{1,8}, {2,8}, {3,8}, {4,8}, {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, {10,8},
	{11,8}, {12,8}, {13,8}, {14,8}, {15,8}, {16,8}, {17,8}, {18,8}, {19,8}, {20,8},
	{21,8}, {22,8}, {23,8}, {24,8}, {25,8}, {26,8}, {27,8}, {28,8}, {29,8}, {30, 8}, {31,8},

	{1,9}, {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9},
	{11,9}, {12,9}, {13,9}, {14,9}, {15,9}, {16,9}, {17,9}, {18,9}, {19,9}, {20,9},
	{21,9}, {22,9}, {23,9}, {24,9}, {25,9}, {26,9}, {27,9}, {28,9}, {29,9}, {30, 9},

	{1,10}, {2,10}, {3,10}, {4,10}, {5,10}, {6,10}, {7,10}, {8,10}, {9,10}, {10,10},
	{11,10}, {12,10}, {13,10}, {14,10}, {15,10}, {16,10}, {17,10}, {18,10}, {19,10}, {20,10},
	{21,10}, {22,10}, {23,10}, {24,10}, {25,10}, {26,10}, {27,10}, {28,10}, {29,10}, {30, 10}, {31,10},

	{1,11}, {2,11}, {3,11}, {4,11}, {5,11}, {6,11}, {7,11}, {8,11}, {9,11}, {10,11},
	{11,11}, {12,11}, {13,11}, {14,11}, {15,11}, {16,11}, {17,11}, {18,11}, {19,11}, {20,11},
	{21,11}, {22,11}, {23,11}, {24,11}, {25,11}, {26,11}, {27,11}, {28,11}, {29,11}, {30, 11},

	{1,12}, {2,12}, {3,12}, {4,12}, {5,12}, {6,12}, {7,12}, {8,12}, {9,12}, {10,12},
	{11,12}, {12,12}, {13,12}, {14,12}, {15,12}, {16,12}, {17,12}, {18,12}, {19,12}, {20,12},
	{21,12}, {22,12}, {23,12}, {24,12}, {25,12}, {26,12}, {27,12}, {28,12}, {29,12}, {30, 12}, {31,12},

	{-1,-1}, {-1,-1}//end of year
},
{
	{1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1}, {10,1},
	{11,1}, {12,1}, {13,1}, {14,1}, {15,1}, {16,1}, {17,1}, {18,1}, {19,1}, {20,1},
	{21,1}, {22,1}, {23,1}, {24,1}, {25,1}, {26,1}, {27,1}, {28,1}, {29,1}, {30, 1}, {31,1},

	{1,2}, {2,2}, {3,2}, {4,2}, {5,2}, {6,2}, {7,2}, {8,2}, {9,2}, {10,2},
	{11,2}, {12,2}, {13,2}, {14,2}, {15,2}, {16,2}, {17,2}, {18,2}, {19,2}, {20,2},
	{21,2}, {22,2}, {23,2}, {24,2}, {25,2}, {26,2}, {27,2}, {28,2}, {29,2},

	{1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {6,3}, {7,3}, {8,3}, {9,3}, {10,3},
	{11,3}, {12,3}, {13,3}, {14,3}, {15,3}, {16,3}, {17,3}, {18,3}, {19,3}, {20,3},
	{21,3}, {22,3}, {23,3}, {24,3}, {25,3}, {26,3}, {27,3}, {28,3}, {29,3}, {30, 3}, {31,3},

	{1,4}, {2,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, {8,4}, {9,4}, {10,4},
	{11,4}, {12,4}, {13,4}, {14,4}, {15,4}, {16,4}, {17,4}, {18,4}, {19,4}, {20,4},
	{21,4}, {22,4}, {23,4}, {24,4}, {25,4}, {26,4}, {27,4}, {28,4}, {29,4}, {30, 4},

	{1,5}, {2,5}, {3,5}, {4,5}, {5,5}, {6,5}, {7,5}, {8,5}, {9,5}, {10,5},
	{11,5}, {12,5}, {13,5}, {14,5}, {15,5}, {16,5}, {17,5}, {18,5}, {19,5}, {20,5},
	{21,5}, {22,5}, {23,5}, {24,5}, {25,5}, {26,5}, {27,5}, {28,5}, {29,5}, {30, 5}, {31,5},

	{1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6}, {8,6}, {9,6}, {10,6},
	{11,6}, {12,6}, {13,6}, {14,6}, {15,6}, {16,6}, {17,6}, {18,6}, {19,6}, {20,6},
	{21,6}, {22,6}, {23,6}, {24,6}, {25,6}, {26,6}, {27,6}, {28,6}, {29,6}, {30, 6},

	{1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7}, {7,7}, {8,7}, {9,7}, {10,7},
	{11,7}, {12,7}, {13,7}, {14,7}, {15,7}, {16,7}, {17,7}, {18,7}, {19,7}, {20,7},
	{21,7}, {22,7}, {23,7}, {24,7}, {25,7}, {26,7}, {27,7}, {28,7}, {29,7}, {30, 7}, {31,7},

	{1,8}, {2,8}, {3,8}, {4,8}, {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, {10,8},
	{11,8}, {12,8}, {13,8}, {14,8}, {15,8}, {16,8}, {17,8}, {18,8}, {19,8}, {20,8},
	{21,8}, {22,8}, {23,8}, {24,8}, {25,8}, {26,8}, {27,8}, {28,8}, {29,8}, {30, 8}, {31,8},

	{1,9}, {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9},
	{11,9}, {12,9}, {13,9}, {14,9}, {15,9}, {16,9}, {17,9}, {18,9}, {19,9}, {20,9},
	{21,9}, {22,9}, {23,9}, {24,9}, {25,9}, {26,9}, {27,9}, {28,9}, {29,9}, {30, 9},

	{1,10}, {2,10}, {3,10}, {4,10}, {5,10}, {6,10}, {7,10}, {8,10}, {9,10}, {10,10},
	{11,10}, {12,10}, {13,10}, {14,10}, {15,10}, {16,10}, {17,10}, {18,10}, {19,10}, {20,10},
	{21,10}, {22,10}, {23,10}, {24,10}, {25,10}, {26,10}, {27,10}, {28,10}, {29,10}, {30, 10}, {31,10},

	{1,11}, {2,11}, {3,11}, {4,11}, {5,11}, {6,11}, {7,11}, {8,11}, {9,11}, {10,11},
	{11,11}, {12,11}, {13,11}, {14,11}, {15,11}, {16,11}, {17,11}, {18,11}, {19,11}, {20,11},
	{21,11}, {22,11}, {23,11}, {24,11}, {25,11}, {26,11}, {27,11}, {28,11}, {29,11}, {30, 11},

	{1,12}, {2,12}, {3,12}, {4,12}, {5,12}, {6,12}, {7,12}, {8,12}, {9,12}, {10,12},
	{11,12}, {12,12}, {13,12}, {14,12}, {15,12}, {16,12}, {17,12}, {18,12}, {19,12}, {20,12},
	{21,12}, {22,12}, {23,12}, {24,12}, {25,12}, {26,12}, {27,12}, {28,12}, {29,12}, {30, 12}, {31,12},

	{-1,-1}//end of year
	}
};

int GetGregorianYearLength(bool bOverlapped)
{
	return bOverlapped ? 366 : 365;
}

int GetGregorianDay(bool bOverlapped, int nDayIndex)
{
	return g_year_days[(bOverlapped ? 1 : 0)][nDayIndex][0];
}

int GetGregorianMonth(bool bOverlapped, int nDayIndex)
{
	return g_year_days[(bOverlapped ? 1 : 0)][nDayIndex][1];
}

bool IsLeapYear(int year)
{
	if ((year % 4) == 0)
	{
		if ((year % 100 == 0) && (year % 400 != 0))
			return false;
		else
			return true;
	}

	return false;
}

int GetMonthMaxDays(int year, int month)
{
	int m_months_ovr[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 
	int m_months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 

	if (IsLeapYear(year))
		return m_months_ovr[month];
	else
		return m_months[month];
}

void GetPrevDay(VCTIME &day)
{
	day.day--;

	if (day.day < 1)
	{
		day.month--;
		if (day.month < 1)
		{
			day.month = 12;
			day.year--;
		}
		day.day = GetMonthMaxDays(day.year, day.month);
	}
}

void GetNextDay(VCTIME &day)
{
	day.day++;
	if (day.day > GetMonthMaxDays(day.year, day.month))
	{
		day.day = 1;
		day.month ++;
	}

	if (day.month > 12)
	{
		day.month = 1;
		day.year++;
	}
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

	time_deg = put_in_360(time_deg);

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


bool VCTIME::operator<(VCTIME &date)
{
	int y1, y2, m1, m2, d1, d2;
	double h1, h2;
	d1 = this->day;
	d2 = date.day;
	m1 = this->month;
	m2 = date.month;
	y1 = this->year;
	y2 = date.year;
	h1 = shour + tzone/24.0;
	h2 = date.shour + date.tzone/24.0;

	NormalizeValues(y1, m1, d1, h1);
	NormalizeValues(y2, m2, d2, h2);

	if (y1 > y2)
		return false;
	else if (y1 < y2)
		return true;
	else
	{
		if (m1 > m2)
			return false;
		else if (m1 < m2)
			return true;
		else
		{
			if (d1 < d2)
				return true;
			else 
				return false;
		}
	}
}

bool VCTIME::operator<=(VCTIME &date)
{
	int y1, y2, m1, m2, d1, d2;
	double h1, h2;

	d1 = this->day;
	d2 = date.day;
	m1 = this->month;
	m2 = date.month;
	y1 = this->year;
	y2 = date.year;
	h1 = shour + tzone/24.0;
	h2 = date.shour + date.tzone/24.0;

	NormalizeValues(y1, m1, d1, h1);
	NormalizeValues(y2, m2, d2, h2);

	if (y1 > y2)
		return false;
	else if (y1 < y2)
		return true;
	else
	{
		if (m1 > m2)
			return false;
		else if (m1 < m2)
			return true;
		else
		{
			if (d1 <= d2)
				return true;
			else 
				return false;
		}
	}
}

bool VCTIME::operator==(VCTIME &date)
{
	int y1, y2, m1, m2, d1, d2;
	double h1, h2;

	d1 = this->day;
	d2 = date.day;
	m1 = this->month;
	m2 = date.month;
	y1 = this->year;
	y2 = date.year;
	h1 = shour + tzone/24.0;
	h2 = date.shour + date.tzone/24.0;

	NormalizeValues(y1, m1, d1, h1);
	NormalizeValues(y2, m2, d2, h2);

	return ((y1 == y2) && (m1 == m2) && (d1 == d2));

}

void VCTIME::NormalizeValues(int &y1, int &m1, int &d1, double &h1)
{

	if (h1 < 0.0)
	{
		d1--;
		h1 += 1.0;
	}
	else if (h1 >= 1.0)
	{
		h1 -= 1.0;
		d1++;
	}
	if (d1 < 1)
	{
		m1--;
		if (m1 < 1)
		{
			m1 = 12;
			y1--;
		}
		d1 = GetMonthMaxDays(y1, m1);
	}
	else if (d1 > GetMonthMaxDays(y1, m1))
	{
		m1++;
		if (m1 > 12)
		{
			m1 = 1;
			y1++;
		}
		d1 = 1;
	}

}

int VCTIME::GetMonthMaxDays(int year, int month)
{
	int m_months_ovr[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 
	int m_months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 

	if (IsLeapYear(year))
		return m_months_ovr[month];
	else
		return m_months[month];
}

bool VCTIME::IsLeapYear(int year)
{
	if ((year % 4) == 0)
	{
		if ((year % 100 == 0) && (year % 400 != 0))
			return false;
		else
			return true;
	}

	return false;
}

bool VCTIME::IsLeapYear()
{
	return IsLeapYear(this->year);
}

void VCTIME::ChangeTimeZone(double tZone)
{
	shour += (tZone - tzone)/24;
	NormalizeValues(year, month, day, shour);
	tzone = tZone;
}

double VCTIME::GetJulian()
{
	int yy = year - int((12 - month) / 10);
	int mm = month + 9;

	if (mm >= 12)
		mm -= 12;

	int k1, k2, k3;
	int j;

	k1 = int (floor(365.25 * (yy + 4712)));
	k2 = int (floor(30.6 * mm + 0.5));
	k3 = int (floor(floor((double)(yy/100)+49)*.75))-38;
	j = k1 + k2 + day + 59;
	if (j > 2299160)
		j -= k3;

	return double(j);
}

void VCTIME::NormalizeValues()
{
	NormalizeValues(year, month, day, shour);
}

void VCTIME::PreviousDay()
{
	day--;
	if (day < 1)
	{
		month--;
		if (month < 1)
		{
			month = 12;
			year--;
		}
		day = GetMonthMaxDays(year, month);
	}
	dayOfWeek = (dayOfWeek + 6) % 7;
}

void VCTIME::NextDay()
{
	day++;
	if (day > GetMonthMaxDays(year, month))
	{
		month++;
		if (month > 12)
		{
			month = 1;
			year++;
		}
		day = 1;
	}
	dayOfWeek = (dayOfWeek + 1) % 7;
}

void VCTIME::operator+=(int n)
{
	for(int i = 0; i < n; i++)
		NextDay();
}

void VCTIME::operator-=(int n)
{
	for(int i = 0; i < n; i++)
		PreviousDay();
}

double VCTIME::GetJulianDetailed()
{
	return GetJulian() - 0.5 + shour;
}

double VCTIME::GetJulianComplete()
{
	return GetJulian() - 0.5 + shour - tzone/24.0;
}


void VAISNAVADAY::Clear()
{
		fVaisValid = false;
		// init
		festivals.Empty();
		nFastType = FAST_NULL;
		nFeasting = FEAST_NULL;
		nMhdType = EV_NULL;
		ekadasi_parana = false;
		ekadasi_vrata_name.Empty();
		eparana_time1 = eparana_time2 = 0.0;
		sankranti_zodiac = -1;
		sankranti_day.day = 0;
		sankranti_day.shour = 0.0;
		sankranti_day.month = 0;
		sankranti_day.year = 0;
		was_ksaya = false;
//		nSpecFestival = 0;
		ksaya_time1 = ksaya_time2 = 0.0;
		is_vriddhi = false;
		nCaturmasya = 0;
		//moonset.SetValue(0);
		//moonrise.SetValue(0);
		//nDST = 0;
}

void VCTIME::InitWeekDay()
{
	dayOfWeek = (int(GetJulian()) + 1) % 7;
}

//***********************************************************************/
//* Name:    calcDateFromJD								*/
//* Type:    Function									*/
//* Purpose: Calendar date from Julian Day					*/
//* Arguments:										*/
//*   jd   : Julian Day									*/
//* Return value:										*/
//*   String date in the form DD-MONTHNAME-YYYY					*/
//* Note:											*/
//***********************************************************************/

void VCTIME::SetFromJulian(JULIANDATE jd)
{
	double z = floor(jd + 0.5);

	double f = (jd + 0.5) - z;

	double A, B, C, D, E, alpha;

	if (z < 2299161.0)
	{
		A = z;
	} 
	else 
	{
		alpha = floor((z - 1867216.25)/36524.25);
		A = z + 1.0 + alpha - floor(alpha/4.0);
	}

	B = A + 1524;
	C = floor((B - 122.1)/365.25);
	D = floor(365.25 * C);
	E = floor((B - D)/30.6001);
	day = (int) floor(B - D - floor(30.6001 * E) + f);
	month = int ((E < 14) ? E - 1 : E - 13);
	year = int((month > 2) ? C - 4716 : C - 4715);
	tzone = 0.0;
	shour = modf(jd + 0.5, &z);
}

// n - is order number of given day
// x - is number of day in week (0-sunday, 1-monday, ..6-saturday)
// if x >= 5, then is calculated whether day is after last x-day

int is_n_xday(VCTIME vc, int n, int x)
{
	int xx[7] = {1, 7, 6, 5, 4, 3, 2};

	int fdm, fxdm, nxdm, max;

	// prvy den mesiaca
	fdm = xx[ (7 + vc.day - vc.dayOfWeek) % 7 ];

	// 1. x-day v mesiaci ma datum
	fxdm = xx[ (fdm - x + 7) % 7 ];

	// n-ty x-day ma datum
	if ((n < 0) || (n >= 5))
	{
		nxdm = fxdm + 28;
		max = GetMonthMaxDays(vc.year, vc.month);
		while(nxdm > max)
		{
			nxdm -= 7;
		}
	}
	else
	{
		nxdm = fxdm + (n - 1)* 7;
	}

	return (vc.day >= nxdm) ? 1 : 0;
}

/*TTimezone gzone[] = 
{
	{ 0, 0x00000000, "<none>", 0.0},
	{60, 0x3050a050, "Europian Union", 0.0},
	{60, 0x3020b010, "USA",  0.0},
	{60, 0xa0503050, "Australia",  0.0},
	{60, 0x40549054, "Egypt",  0.0},
	{60, 0x90104010, "Namibia",  0.0},
	{60, 0x3050a050, "Tunisia", 0.0},
	{60, 0x90504010, "New Zealand", 0.0},
	{60, 0xa0103050, "Australia - Tasmania", 0.0},
	{60, 0xb0101050, "Tonga", 0.0},
	{60, 0x3050a050, "Russia, Belarus and Armenia", 0.0},
	{60, 0x4010a050, "Mexico", 0.0},
	{ 0,          0,      NULL, 0.0}
};*/


// This table has 8 items for each line:
//  [0]: starting month
//  [4]: ending month
// 
//  [1]: type of day, 0-day is given as n-th x-day of month, 1- day is given as DATE
//  [2]: for [1]==1 this means day of month
//     : for [1]==0 this order number of occurance of the given day (1,2,3,4 is acceptable, 5 means *last*)
//  [3]: used only for [1]==0, and this means day of week (0=sunday,1=monday,2=tuesday,3=wednesday,...)
//     : [1] to [3] are used for starting month
//  [5] to [7] is used for ending month in the same way as [1] to [3] for starting month
//
// EXAMPLE: (first line)   3 0 5 0 10 0 5 0
// [0] == 3, that means starting month is March
// [1] == 0, that means starting system is (day of week)
// [2] == 5, that would mean that we are looking for 5th occurance of given day in the month, but 5 here means,
//           that we are looking for *LAST* occurance of given day
// [3] == 0, this is *GIVEN DAY*, and it is SUNDAY
//
//         so, DST is starting on last sunday of March
//
// similarly analysed, DST is ending on last sunday of October
//

int GetDaylightBias(VCTIME vc, DWORD val)
{
	int DSTtable[8];
	int bias = 1;

	TTimeZone::ExpandVal(val, DSTtable);

	if (vc.month == DSTtable[0])
	{
		if (DSTtable[1] == 0)
			return is_n_xday(vc, DSTtable[2], DSTtable[3]) * bias;
		else
			return (vc.day >= DSTtable[2]) ? bias : 0;
	}
	else if (vc.month == DSTtable[4])
	{
		if (DSTtable[5] == 0)
			return (1 - is_n_xday(vc, DSTtable[6], DSTtable[7]))*bias;
		else
			return (vc.day >= DSTtable[6]) ? 0 : bias;
	}
	else 
	{
		if (DSTtable[0] > DSTtable[4])
		{
			// zaciatocny mesiac ma vyssie cislo nez koncovy
			// napr. pre australiu
			if ((vc.month > DSTtable[0]) || (vc.month < DSTtable[4]))
				return bias;
		}
		else
		{
			// zaciatocny mesiac ma nizsie cislo nez koncovy
			// usa, europa, asia
			if ((vc.month > DSTtable[0]) && (vc.month < DSTtable[4]))
				return bias;
		}

		return 0;
	}
}

int is_daylight_time(VCTIME vc, int nIndex)
{
	return GetDaylightBias(vc, TTimeZone::gzone[TTimeZone::ID2INDEX(nIndex)].val);
}

int VCTIME::operator=(int i)
{
	year = i;
	month = i;
	day = i;
	shour = 0.0;
	tzone = 0.0;

	return i;
}

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

Boolean DAYTIME::operator>(DAYTIME &dt)
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

Boolean DAYTIME::operator<(DAYTIME &dt)
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

Boolean DAYTIME::operator>=(DAYTIME &dt)
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

Boolean DAYTIME::operator<=(DAYTIME &dt)
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

/* BEGIN GCAL 1.4.3 */
void VAISNAVADAY::GetTextA(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi)
{
//	static char * dow[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

	TString s1, s2;

	s1 = GetTithiName(astrodata.nTithi);

	if ((astrodata.nTithi == 10) || (astrodata.nTithi == 25) || (astrodata.nTithi == 11) || (astrodata.nTithi == 26))
	{
		if (ekadasi_parana == false)
		{
			if (nMhdType == EV_NULL)
			{
				s1 += " ";
				s1 += gstr[58];
			}
			else
			{
				s1 += " ";
				s1 += gstr[59];
			}
		}
	}

	gstr[this->date.dayOfWeek].Left(2, s2);
	str.Format("%2d %s %d %s  %-34s%c ", this->date.day, AvcGetMonthAbr(date.month), this->date.year
		, s2.c_str() ,s1.c_str(), (bPaksa ? GetPaksaChar(this->astrodata.nPaksa) : ' '));

	if (bYoga)
	{
		s2.Format("%-10s", GetYogaName(this->astrodata.nYoga));
		str += s2;
	}

	if (bNaks)
	{
		s2.Format("%-15s", GetNaksatraName(this->astrodata.nNaksatra));
		str += s2;
	}
	
	if (this->nFastType != FAST_NULL && bFlag)
		str += " *";
	else
		str += "  ";

	if (bRasi)
	{
		if (bRasi == 1)
			s2.Format("   %-15s", GetSankrantiName(GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		else
			s2.Format("   %-15s", GetSankrantiNameEn(GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		str += s2;
	}
}

void VAISNAVADAY::GetTextRtf(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi)
{
	TString s1, s2;

	s1 = GetTithiName(astrodata.nTithi);

	if ((astrodata.nTithi == 10) || (astrodata.nTithi == 25) || (astrodata.nTithi == 11) || (astrodata.nTithi == 26))
	{
		if (ekadasi_parana == false)
		{
			if (nMhdType == EV_NULL)
			{
				s1 += " ";
				s1 += gstr[58];
			}
			else
			{
				s1 += " ";
				s1 += gstr[59];
			}
		}
	}

	gstr[this->date.dayOfWeek].Left(2, s2);
	str.Format("\\par %2d %s %d %s\\tab %s\\tab %c ", this->date.day, AvcGetMonthAbr(date.month), this->date.year
		, s2.c_str() ,s1.c_str(), (bPaksa ? GetPaksaChar(this->astrodata.nPaksa) : ' '));

	if (bYoga)
	{
		s2.Format("\\tab %s", GetYogaName(this->astrodata.nYoga));
		str += s2;
	}

	if (bNaks)
	{
		s2.Format("\\tab %s", GetNaksatraName(this->astrodata.nNaksatra));
		str += s2;
	}
	
	if (this->nFastType != FAST_NULL && bFlag)
		str += "\\tab *";
	else if (bFlag)
		str += "\\tab ";

	if (bRasi)
	{
		if (bRasi == 1)
			s2.Format("\\tab %s", GetSankrantiName(GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		else
			s2.Format("\\tab %s", GetSankrantiNameEn(GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		str += s2;
	}

	str += "\r\n";
}

/* END GCAL 1.4.3 */

void VAISNAVADAY::GetTextEP(TString &str)
{
	double h1, m1, h2, m2;
	m1 = modf(eparana_time1, &h1);
	if (eparana_time2 >= 0.0)
	{
		m2 = modf(eparana_time2, &h2);
		if (GetShowSetVal(50))
			str.Format("                 %s %02d:%02d (%s) - %02d:%02d (%s) %s", gstr[60].c_str(),
				int(h1), int(m1*60), AvcGetParanaReasonText(eparana_type1), 
				int(h2), int(m2*60), AvcGetParanaReasonText(eparana_type2),
				AvcGetDSTSignature(nDST));
		else
			str.Format("                 %s %02d:%02d - %02d:%02d (%s)", gstr[60].c_str(),
				int(h1), int(m1*60), int(h2), int(m2*60), AvcGetDSTSignature(nDST));
	}
	else if (eparana_time1 >= 0.0)
	{
		if (GetShowSetVal(50))
			str.Format("                 %s %02d:%02d (%s) %s", gstr[61].c_str(),
				int(h1), int(m1*60), AvcGetParanaReasonText(eparana_type1), AvcGetDSTSignature(nDST) );
		else
			str.Format("                 %s %02d:%02d (%s)", gstr[61].c_str(),
				int(h1), int(m1*60), AvcGetDSTSignature(nDST) );
	}
	else
	{
		str = "                 ";
		str += gstr[62];
	}
}


int VAISNAVADAY::GetHeadFestival()
{
	if (festivals.IsEmpty())
		return -1;

	return 0;
}

Boolean VAISNAVADAY::GetNextFestival(int &i, TString &str)
{
	str.Empty();

	if (i < 0)
		return FALSE;

	while(i < festivals.GetLength() && festivals.GetAt(i) != '#')
	{
		str += festivals.GetAt(i);
		i++;
	}

	if (i < festivals.GetLength())
	{
		i++;
	}
	else
	{
		i = -1;
	}

	return TRUE;
}

const char * AvcGetEventClassText(int i)
{
	switch(i)
	{
		case 0: return "Appearance Days of the Lord and His Incarnations"; break;
		case 1: return "Events in the Pastimes of the Lord and His Associates"; break;
		case 2: return "Appearance and Disappearance Days of Recent Acaryas"; break;
		case 3: return "Appearance and Disappearance Days of Mahaprabhu's Associates and Other Acaryas"; break;
		case 4: return "ISKCON's Historical Events"; break;
		case 5: return "Bengal-specific Holidays"; break;
		case 6: return "My Personal Events"; break;
		default: break;
	}
	return "";
}

char * AvcGetTextLatitude(double d)
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

char * AvcGetTextLongitude(double d)
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

char * AvcGetTextTimeZone(double d)
{
	int a4, a5;
	int sig;
	static char inter[16];

	if (d < 0.0)
	{
		sig = -1;
		d = -d;
	}
	else
	{
		sig = 1;
	}
	a4 = int(d);
	a5 = int((d - a4)*60 + 0.5);

	sprintf(inter, "%c%d:%02d", (sig>0?'+':'-'), a4, a5);

	return inter;
}

char * AvcGetTextTimeZoneArg(double d)
{
	int a4, a5;
	int sig;
	static char inter[16];

	if (d < 0.0)
	{
		sig = -1;
		d = -d;
	}
	else
	{
		sig = 1;
	}
	a4 = int(d);
	a5 = int((d - a4)*60 + 0.5);

	sprintf(inter, "%d%c%02d", a4, (sig>0?'E':'W'),a5);

	return inter;
}

const char * AvcGetMonthAbr(int n)
{
	return gstr[64 + n].c_str();
}

bool VCTIME::IsBeforeThis(VCTIME &date)
{
	int y1, y2, m1, m2, d1, d2;
	d1 = this->day;
	d2 = date.day;
	m1 = this->month;
	m2 = date.month;
	y1 = this->year;
	y2 = date.year;

	if (y1 > y2)
		return false;
	else if (y1 < y2)
		return true;
	else if (m1 > m2)
		return false;
	else if (m1 < m2)
		return true;
	else if (d1 < d2)
		return true;
	else 
		return false;
}

double DAYTIME::GetDayTime()
{
	return ((double(hour)*60.0 + double(min))*60.0 + double(sec)) / 86400.0;
}

void VCTIME::Today()
{
	SYSTEMTIME st;
	TIME_ZONE_INFORMATION tzi;
	DWORD dw;


	GetLocalTime(&st);

	day = st.wDay;
	month = st.wMonth;
	year = st.wYear;
	shour = 0.5;
	tzone = 0;

	dw = GetTimeZoneInformation(&tzi);
	switch(dw)
	{
	case TIME_ZONE_ID_DAYLIGHT:
		shour = -tzi.DaylightBias / 1440.0;
		tzone = -tzi.Bias / 60.0;
		break;
	case TIME_ZONE_ID_STANDARD:
		shour = 0.0;
		tzone = -tzi.Bias / 60.0;
		break;
	}
}

// vracia -1, ak zadany den je den nasledujuci po THIS
// vracia 1 ak THIS je nasledujuci den po zadanom dni

int VCTIME::CompareYMD(VCTIME v)
{
	if (v.year < year)
		return (year - v.year) * 365;
	else if (v.year > year)
		return (year - v.year) * 365;

	if (v.month < month)
		return (month - v.month)*31;
	else if (v.month > month)
		return (month - v.month)*31;

	return (day - v.day);
}

Boolean VAISNAVADAY::GetNaksatraTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to)
{
	VCTIME start;

	start = date;
	start.shour = this->astrodata.sun.sunrise_deg / 360 + earth.tzone/24.0;

	GetNextNaksatra(earth, start, to);
	GetPrevNaksatra(earth, start, from);

	return TRUE;
}

int VCTIME::GetHour()
{
	return int(floor(shour*24));
}

int VCTIME::GetMinute()
{
	return int(floor((shour*24 - int(shour*24)) * 60));
}

int VCTIME::GetMinuteRound()
{
	return int(floor((shour*24 - int(shour*24)) * 60 + 0.5));
}

Boolean VAISNAVADAY::GetTithiTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to)
{
	VCTIME start;

	start = date;
	start.shour = this->astrodata.sun.sunrise_deg / 360 + earth.tzone/24.0;

	GetNextTithiStart(earth, start, to);
	GetPrevTithiStart(earth, start, from);

	return TRUE;

}

int VCTIME::GetDayInteger()
{
	return year * 384 + month * 32 + day;
}

int VCTIME::GetSecond()
{
	return int(floor((shour*1440 - int(floor(shour*1440))) * 60));
}

int VAISNAVADAY::GetFestivalClass(TString &str)
{
	int i, j, val;

	i = str.Find("[c");

	if (i >= 0)
	{
//		i += 2;
		if ((i + 2) < str.GetLength())
		{
			val = int(str.GetAt(i+2) - '0');
			j = str.Find("]", i);
			if (j >= str.GetLength())
				j = str.GetLength();
			if (j > i)
			{
				str.Delete(i, j - i + 1);
			}
			if (val < 0 || val > 6)
				return -1;
			return val;
		}
		else
			return -1;
	}
	else
	{
		return -1;
	}

}


const char * VCTIME::c_str()
{
	static char sz[36];

	sprintf(sz, "%2d %s %d  %02d:%02d:%02d", day, AvcGetMonthAbr(month), year, GetHour(), GetMinute(), GetSecond());

	return sz;
}

void VAISNAVADAY::GetFastingSubject(TString &strFest, int &nFast, TString &strFastSubj)
{
	int nf, nf2;

	// default values
	nFast = 0;
	strFastSubj.Empty();

	// finding fast subject
	nf = strFest.Find("[f:");
	if (nf >= 0 && nf < strFest.GetLength())
	{
		// ziskava typ postu
		nFast = strFest.GetAt(nf+3) - '0';
		nf2 = strFest.Find("]", nf);
		if (nf2 >= 0)
		{
			strFest.Mid(nf + 5, nf2 - nf - 5, strFastSubj);
		}
		strFest.Delete(nf, strFest.GetLength() - nf);
	}
}
