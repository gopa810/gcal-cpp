#ifndef _VCAL_SE_LEVEL_0
#define _VCAL_SE_LEVEL_0
// PORTABLE

#include "math.h"
#include "gmath.h"
#include "platform.h"

typedef double JULIANDATE;

typedef double DAYHOURS;

class VATIME
{
public:
	VATIME() { };
	VATIME(int t, int m, int y) { tithi = t; masa = m; gyear = y; }
	int tithi;
	int masa;
	int gyear;
	void next()
	{
		tithi++;
		if (tithi >= 30)
		{
			tithi %= 30;
			masa++;
		}
		if (masa >= 12)
		{
			masa %= 12;
			gyear++;
		}
	}
	void prev()
	{
		if (tithi == 0)
		{
			if (masa == 0)
			{
				masa = 11;
				tithi = 29;
			}
			else
			{
				masa--;
				tithi = 29;
			}
		}
		else
		{
			tithi--;
		}
	}
};

class VCTIME
{
public:
	const char * c_str();
	int GetSecond();
	int GetDayInteger();
	int GetMinute();
	int GetMinuteRound();
	int GetHour();
	int CompareYMD(VCTIME v);
	void Today();
	bool IsBeforeThis(VCTIME &date);
	int operator=(int i);
	void SetFromJulian(JULIANDATE jd);
	void InitWeekDay(void);
	double GetJulianComplete();
	double GetJulianDetailed();
	void operator+=(int);
	void operator-=(int);
	void NextDay();
	void PreviousDay();
	double GetJulian();
	void ChangeTimeZone(double);
	bool IsLeapYear();
	bool IsLeapYear(int year);

	int GetMonthMaxDays(int year, int month);

	void NormalizeValues();
	void NormalizeValues(int &y, int &m, int &d, double &h);
	bool operator<(VCTIME &date);
	bool operator<=(VCTIME &date);
	bool operator==(VCTIME &date);
    int year;
    int month;
    int dayOfWeek;
    int day;
	DAYHOURS shour;
	double tzone;
	VCTIME() { 
		year = 0;
		month = 0;
		dayOfWeek = 0;
		day = 0;
		shour = 0.0;
		tzone = 0.0;
	}
	
};

class DAYTIME
{
public:
	double GetDayTime(void);
	Boolean operator<(DAYTIME &dt);
	Boolean operator>(DAYTIME &dt);
	Boolean operator>=(DAYTIME &dt);
	Boolean operator<=(DAYTIME &dt);
	void operator+=(int mn) {
		min += mn;
		while(min < 0) { min += 60; hour--;}
		while(min > 59) { min -= 60; hour++;}
	}
	void operator-=(int mn) {
		min -= mn;
		while(min < 0) { min += 60; hour--;}
		while(min > 59) { min -= 60; hour++;}
	}
	void SetValue(int i);
	void SetDayTime(double d);
	int hour;
	int min;
	int sec;
	int mili;
	void SetDegTime(double);
};

class MOONDATA 
{
public:
	/*// illumination (synodic) phase
	double ip;
	// age of moon
	int age;
	// distance from anomalistic phase
	double di;*/
	// latitude from nodal (draconic) phase
	double latitude_deg;
	// longitude from sidereal motion
	double longitude_deg;

	double radius; //(* lambda, beta, R *)
	double rektaszension, declination;  //(* alpha, delta *)
	double parallax;
	double elevation, azimuth;//          (* h, A *)

};

class EARTHDATA
{
public:
	int dst;
	// terrestrial longitude of observation
	double longitude_deg;

	// terrestrial latitude of observation
	double latitude_deg;

	// time zone (hours)
	double tzone;

	// observated event
	// 0 - center of the sun
	// 1 - civil twilight
	// 2 - nautical twilight
	// 3 - astronomical twilight
	int obs;

	EARTHDATA() { obs = 0; longitude_deg = 0.0; latitude_deg = 0.0; tzone = 0.0; dst = 0;}
};

class SUNDATA
{
public:
	double length_deg;
	double arunodaya_deg;
	double sunrise_deg;
	double sunset_deg;

	double declination_deg;
	double longitude_deg;
	double longitude_set_deg;
	double longitude_arun_deg;
	double right_asc_deg;

	// time of arunodaya - 96 mins before sunrise
	DAYTIME arunodaya;
	// time of sunrise
	DAYTIME rise;
	// time of noon
	DAYTIME noon;
	// time of sunset
	DAYTIME set;
	// length of the day
	DAYTIME length;
};

class DAYDATA
{
public:
	// date of Julian epoch
	JULIANDATE jdate;
	// sun
	SUNDATA sun;
	// moon
	MOONDATA moon;
	// year of Gaurabda epoch
	int nGaurabdaYear;
	// value of ayanamsa for this date
	double msAyanamsa;
	// sign of zodiac
	int nSunRasi;
	// rasi of the moon
	int nMoonRasi;
	// tithi #
	int nTithi;
	// tithi at arunodaya
	int nTithiArunodaya;
	// tithi at sunset
	int nTithiSunset;
	// tithi elaps.
	double nTithiElapse;
	// paksa
	int nPaksa;
	// yoga
	int nYoga;
	// yoga elaps.
	double nYogaElapse;
	// naksatra
	int nNaksatra;
	// naksatra elaps.
	double nNaksatraElapse;
	// masa
	int nMasa;
	// distance of moon and sun in degrees
	double msDistance;
};

#define EV_NULL              0x100
#define EV_SUDDHA            0x101
#define EV_UNMILANI          0x102
#define EV_VYANJULI          0x103
#define EV_TRISPRSA          0x104
#define EV_UNMILANI_TRISPRSA 0x105
#define EV_PAKSAVARDHINI     0x106
#define EV_JAYA              0x107
#define EV_JAYANTI           0x108
#define EV_PAPA_NASINI       0x109
#define EV_VIJAYA            0x110

#define FAST_NULL      0x0
#define FAST_NOON      0x201
#define FAST_SUNSET    0x202
#define FAST_MOONRISE  0x203
#define FAST_DUSK      0x204
#define FAST_MIDNIGHT  0x205
#define FAST_EKADASI   0x206
#define FAST_DAY       0x207

#define FEAST_NULL     0
#define FEAST_TODAY_FAST_YESTERDAY    1
#define FEAST_TOMMOROW_FAST_TODAY     2

#define SPEC_RETURNRATHA    3
#define SPEC_HERAPANCAMI    4
#define SPEC_GUNDICAMARJANA 5
#define SPEC_GOVARDHANPUJA  6
#define SPEC_VAMANADVADASI  7
#define SPEC_VARAHADVADASI  8
#define SPEC_RAMANAVAMI     9
#define SPEC_JANMASTAMI    10
#define SPEC_RATHAYATRA    11
#define SPEC_GAURAPURNIMA  12
#define SPEC_NANDAUTSAVA   13
#define SPEC_MISRAFESTIVAL 14
#define SPEC_PRABHAPP      15


#define CMASYA_PURN_1_FIRST 0x000011
#define CMASYA_PURN_1_LAST  0x000012
#define CMASYA_PURN_2_FIRST 0x000021
#define CMASYA_PURN_2_LAST  0x000022
#define CMASYA_PURN_3_FIRST 0x000031
#define CMASYA_PURN_3_LAST  0x000032
#define CMASYA_PURN_4_FIRST 0x000041
#define CMASYA_PURN_4_LAST  0x000042

#define CMASYA_PRAT_1_FIRST 0x001100
#define CMASYA_PRAT_1_LAST  0x001200
#define CMASYA_PRAT_2_FIRST 0x002100
#define CMASYA_PRAT_2_LAST  0x002200
#define CMASYA_PRAT_3_FIRST 0x003100
#define CMASYA_PRAT_3_LAST  0x003200
#define CMASYA_PRAT_4_FIRST 0x004100
#define CMASYA_PRAT_4_LAST  0x004200

#define CMASYA_EKAD_1_FIRST 0x110000
#define CMASYA_EKAD_1_LAST  0x120000
#define CMASYA_EKAD_2_FIRST 0x210000
#define CMASYA_EKAD_2_LAST  0x220000
#define CMASYA_EKAD_3_FIRST 0x310000
#define CMASYA_EKAD_3_LAST  0x320000
#define CMASYA_EKAD_4_FIRST 0x410000
#define CMASYA_EKAD_4_LAST  0x420000

#define CMASYA_1_CONT       0x01000000
#define CMASYA_2_CONT       0x02000000
#define CMASYA_3_CONT       0x03000000
#define CMASYA_4_CONT       0x04000000
#define CMASYA_CONT_MASK    0x0f000000


#define CMASYA_PURN_MASK_MASA    0x0000f0
#define CMASYA_PURN_MASK_DAY     0x00000f
#define CMASYA_PURN_MASK         0x0000ff
#define CMASYA_PRAT_MASK_MASA    0x00f000
#define CMASYA_PRAT_MASK_DAY     0x000f00
#define CMASYA_PRAT_MASK         0x00ff00
#define CMASYA_EKAD_MASK         0xff0000
#define CMASYA_EKAD_MASK_MASA    0xf00000
#define CMASYA_EKAD_MASK_DAY     0x0f0000

#define EP_TYPE_NULL    0
#define EP_TYPE_3DAY    1
#define EP_TYPE_4TITHI  2
#define EP_TYPE_NAKEND  3
#define EP_TYPE_SUNRISE 4
#define EP_TYPE_TEND    5

class VAISNAVADAY
{
public:
	void GetFastingSubject(TString &strFest, int &nFast, TString &strFastSubj);
	int GetLineCount();
	int GetFestivalClass(TString &str);
	Boolean GetTithiTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	Boolean GetNaksatraTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	Boolean GetNextFestival(int &i, TString &str);
	int GetHeadFestival(void);
	void GetTextEP(TString &str);
	/* BEGIN GCAL 1.4.3 */
	void GetTextA(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	void GetTextRtf(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	/* END GCAL 1.4.3 */
	DWORD nCaturmasya;
	int nDST;
	int ksaya_day2;
	int ksaya_day1;
//	int nSpecFestival;
	int nFeasting;
	void Clear();
	// date
	VCTIME date;
	// moon times
	DAYTIME moonrise;
	DAYTIME moonset;
	// astronomical data from astro-sub-layer
	DAYDATA astrodata;
	// data for vaisnava calculations
	TString festivals;
	int nFastType;
	int nMhdType;
	TString ekadasi_vrata_name;
	bool ekadasi_parana;
	double eparana_time1, eparana_time2;
	int eparana_type1, eparana_type2;
	int sankranti_zodiac;
	//double sankranti_time;
	VCTIME sankranti_day;
	bool was_ksaya;
	double ksaya_time1, ksaya_time2;
	bool is_vriddhi;
	// flag for validity
	bool fDateValid;
	bool fAstroValid;
	bool fVaisValid;
	VAISNAVADAY() {
		fDateValid = false;
		fAstroValid = false;
		fVaisValid = false;
		// init
		//festivals = "";
		nFastType = FAST_NULL;
		nMhdType = EV_NULL;
		ekadasi_parana = false;
		ekadasi_vrata_name.Empty();
		eparana_time1 = eparana_time2 = 0.0;
		eparana_type1 = eparana_type2 = EP_TYPE_NULL;
		sankranti_zodiac = -1;
		was_ksaya = false;
		ksaya_time1 = ksaya_time2 = 0.0;
		is_vriddhi = false;
		nDST = 0;
		nCaturmasya = 0;
		moonrise.SetValue(0);
		moonset.SetValue(0);
	}
	~VAISNAVADAY()
	{
		/*		if (festivals)
		{
			delete [] festivals;
		}*/
	}
	VAISNAVADAY &operator=(VCTIME &vc){ date = vc; fDateValid = true; return * this; }
	VAISNAVADAY &operator=(DAYDATA &dd) { astrodata = dd; fAstroValid = true; return * this; }
};

double GetJulianDay(VCTIME &);

double GetJulianDay(int, int, int);

int GetGregorianYearLength(bool bOverlapped);

int GetGregorianDay(bool bOverlapped, int nDayIndex);

int GetGregorianMonth(bool bOverlapped, int nDayIndex);

void GetNextDay(VCTIME &day);
void GetPrevDay(VCTIME &day);
int  GetMonthMaxDays(int year, int month);
bool IsLeapYear(int year);

char * AvcGetTextLatitude(double d);
char * AvcGetTextLongitude(double d);
char * AvcGetTextTimeZone(double d);
const char * AvcGetMonthAbr(int n);
void AvcGetDateText(TString &str, VCTIME vc);
int GetAyanamsaType();
int SetAyanamsaType(int i);
const char * GetAyanamsaName(int nType);

#endif