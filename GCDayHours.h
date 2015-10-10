#pragma once

#include "TString.h"

class DAYTIME
{
public:
	int hour;
	int min;
	int sec;
	int mili;

	double GetDayTime(void);
	bool IsLessThan(DAYTIME &dt);
	bool IsGreaterThan(DAYTIME &dt);
	bool IsGreaterOrEqualThan(DAYTIME &dt);
	bool IsLessOrEqualThan(DAYTIME &dt);
	void AddMinutes(int mn);
	void SetValue(int i);
	void Set(DAYTIME &);
	void SetDayTime(double d);
	void SetDegTime(double);

	void ToLongTimeString(TString &str);

};



