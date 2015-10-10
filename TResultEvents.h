#pragma once

#include "locationref.h"

enum CoreEventType {
	CCTYPE_DATE = 1,
	CCTYPE_S_ARUN = 10,
	CCTYPE_S_RISE = 11,
	CCTYPE_S_NOON = 12,
	CCTYPE_S_SET =  13,
	CCTYPE_TITHI =  20,
	CCTYPE_NAKS  =  21,
	CCTYPE_SANK  =  22,
	CCTYPE_CONJ  =  23,
	CCTYPE_YOGA  = 24,
	CCTYPE_KALA_START=  30,
	CCTYPE_KALA_END  =  31,
	CCTYPE_M_RISE = 41,
	CCTYPE_M_SET  = 42,
	CCTYPE_M_RASI = 45,
	CCTYPE_ASCENDENT = 50
};

class TDayEvent
{
public:
	int nType;
	int nData;
	VCTIME Time;
	int nDst;
	void Set(TDayEvent &de)
	{
		nType = de.nType;
		nData = de.nData;
		Time.Set(de.Time);
		nDst = de.nDst;
	}


};
// PORTABLE

typedef TDayEvent * TDayEventPtr;

class TResultEvents
{
public:
	VCTIME  m_vcStart;
	VCTIME  m_vcEnd;
	unsigned int m_options;
	CLocationRef m_location;
	void clear();
	TDayEventPtr * e_ptr;
	TDayEvent * p_events;
	unsigned int n_count;
	unsigned int n_size;
	bool     b_sorted;
	bool AddEvent(VCTIME inTime, int inType, int inData, int inDst);
	void Sort(bool inSort);
	TDayEvent &operator[](int i)
	{
		if (b_sorted) return *(e_ptr[i]);
		else return p_events[i];
	}
	TResultEvents();
	~TResultEvents();

	void CalculateEvents(CLocationRef &loc, VCTIME vcStart, VCTIME vcEnd);
	int formatText(TString &res);
	int formatXml(TString &strXml);
	int formatRtf(TString &res);
	int writeHtml(FILE * f);


};

