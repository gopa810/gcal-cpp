#pragma once

#include "enums.h"

#include "TString.h"
#include "GCGregorianTime.h"
#include "GCEarthData.h"
#include "GCDayData.h"
#include "GCDayHours.h"
#include "GCMutableArray.h"
#include "GCMutableDictionary.h"

class VAISNAVADAY
{
public:
	// date
	VCTIME date;
	// moon times
	DAYTIME moonrise;
	DAYTIME moonset;
	// astronomical data from astro-sub-layer
	DAYDATA astrodata;

	DWORD nCaturmasya;
	int nDST;
	FeastType nFeasting;
	// data for vaisnava calculations
	GCMutableArray dayEvents;

	TString festivals;
	FastType nFastType;
	int nMhdType;
	TString ekadasi_vrata_name;
	bool ekadasi_parana;
	double eparana_time1, eparana_time2;
	int eparana_type1, eparana_type2;
	int sankranti_zodiac;
	//double sankranti_time;
	VCTIME sankranti_day;

	VAISNAVADAY();
	~VAISNAVADAY();
	void Clear();
	bool GetTithiTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	bool GetNaksatraTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	void GetTextEP(TString &str);

	void GetTextA(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	void GetTextRtf(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	bool hasEventsOfDisplayIndex(int dispIndex);
	GCMutableDictionary * findEventsText(const char * text);
	GCMutableDictionary * AddEvent(int priority, int dispItem, const char * text);
	bool AddSpecFestival(int nSpecialFestival, int nFestClass);
	TString & GetFullTithiName(void);
	bool ConditionEvaluate(int nClass, int nValue, TString &strText, bool defaultRet);
	int GetTextLineCount();
};

