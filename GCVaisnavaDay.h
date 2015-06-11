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
	void GetFastingSubject(TString &strFest, int &nFast, TString &strFastSubj);
	int GetLineCount();
	int GetFestivalClass(TString &str);
	bool GetTithiTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	bool GetNaksatraTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to);
	bool GetNextFestival(int &i, TString &str);
	int GetHeadFestival(void);
	void GetTextEP(TString &str);
	/* BEGIN GCAL 1.4.3 */
	void GetTextA(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	void GetTextRtf(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi);
	bool hasEventsOfDisplayIndex(int dispIndex);
	/* END GCAL 1.4.3 */

	GCMutableDictionary * AddEvent(int priority, int dispItem, const char * text);

};

