#ifndef _TRESULTEVENT________
#define _TRESULTEVENT________
#include "level_0.h"
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
};
// PORTABLE

typedef TDayEvent * TDayEventPtr;

class TResultEvents
{
public:
	VCTIME  m_vcStart;
	VCTIME  m_vcEnd;
	UInt32 m_options;
	CLocationRef m_location;
	void clear();
	TDayEventPtr * e_ptr;
	TDayEvent * p_events;
	UInt32      n_count;
	UInt32      n_size;
	Boolean     b_sorted;
	Boolean AddEvent(VCTIME inTime, int inType, int inData, int inDst);
	void Sort(Boolean inSort);
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


#endif