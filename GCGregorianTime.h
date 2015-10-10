#pragma once

#include "TString.h"

class VCTIME
{
public:
    int year;
    int month;
    int dayOfWeek;
    int day;
	double shour;
	double tzone;
	VCTIME();
	~VCTIME(void);

	const char * c_str();
	int GetSecond();
	int GetDayInteger();
	int GetMinute();
	int GetMinuteRound();
	int GetHour();
	int CompareYMD(VCTIME v);
	void Today();
	bool IsBeforeThis(VCTIME &date);
	void SetFromJulian(double jd);
	void InitWeekDay(void);
	double GetJulianComplete();
	double GetJulianDetailed();
	void NextDay();
	void PreviousDay();
	double GetJulian();
	int GetJulianInteger();
	void ChangeTimeZone(double);
	void NormalizeValues();
	void NormalizeValues(int &y, int &m, int &d, double &h);
	void Set(VCTIME &tt);
	
	static bool IsLeapYear(int year);
	static int GetMonthMaxDays(int year, int month);
	static void GetDateTextWithTodayExt(TString &, VCTIME date);
	static double CalculateJulianDay(int, int, int);
	const char * ToString(VCTIME vc);

	void AddDays(int);
	void SubtractDays(int);
	void Clear();

	bool IsLessThan(VCTIME &date);
	bool IsLessOrEqualTo(VCTIME &date);
	bool IsDateEqual(VCTIME &date);
};

