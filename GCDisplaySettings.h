#pragma once

#include "showset.h"

class GCDisplaySettings
{
protected:
	static CShowSetting gss[];

public:
	static int getCount();
	static int getCountChanged();
	static char * getSettingName(int i);
	static int getValue(int i);
	static void setValue(int i, int val);
	static void setValue(const char * pszSign, int val);
	static void readFile(const char * psz);

	GCDisplaySettings(void);
	~GCDisplaySettings(void);
};

enum GCDS {
	DISP_ALWAYS = -1,
	CAL_ARUN_TIME  = 1,
	CAL_ARUN_TITHI = 0,
	CAL_SUN_RISE = 2,
	CAL_SUN_NOON = 34,
	CAL_SUN_SET = 3,
	CAL_MOON_RISE = 4,
	CAL_MOON_SET = 5,
	CAL_KSAYA = 7,
	CAL_VRDDHI = 8,
	CAL_SUN_LONG = 9,
	CAL_MOON_LONG = 10,
	CAL_AYANAMSHA = 11,
	CAL_JULIAN = 12,
	CATURMASYA_PURNIMA = 13,
	CATURMASYA_PRATIPAT = 14,
	CATURMASYA_EKADASI = 15,
	CAL_SANKRANTI = 16,
	CAL_EKADASI_PARANA = 17,
	CAL_FEST_0 = 22,
	CAL_FEST_1 = 23,
	CAL_FEST_2 = 24,
	CAL_FEST_3 = 25,
	CAL_FEST_4 = 26,
	CAL_FEST_5 = 27,
	CAL_FEST_6 = 28,
	COREEVENTS_SUN = 52,
	COREEVENTS_TITHI = 53,
	COREEVENTS_NAKSATRA = 54,
	COREEVENTS_SANKRANTI = 55,
	COREEVENTS_CONJUNCTION = 56,
	COREEVENTS_RAHUKALAM = 57,
	COREEVENTS_YAMAGHANTI = 58,
	COREEVENTS_GULIKALAM = 59,
	COREEVENTS_MOON = 60,
	COREEVENTS_MOONRASI = 61,
	COREEVENTS_ASCENDENT = 62,
	COREEVENTS_SORT = 63
};

enum DisplayPriorities {
	PRIO_EKADASI_PARANA = 90,
	PRIO_FESTIVALS_0 = 100,
	PRIO_FESTIVALS_1 = 200,
	PRIO_FESTIVALS_2 = 300,
	PRIO_FESTIVALS_3 = 400,
	PRIO_FESTIVALS_4 = 500,
	PRIO_FESTIVALS_5 = 600,
	PRIO_FESTIVALS_6 = 700,
	PRIO_SANKRANTI = 960,
	PRIO_KSAYA = 965,
	PRIO_CM_CONT = 971,
	PRIO_CM_DAY = 972,
	PRIO_CM_DAYNOTE = 973,
	PRIO_ARUN = 975,
	PRIO_SUN  = 980,
	PRIO_MOON = 990,
	PRIO_ASTRO = 1000,

};