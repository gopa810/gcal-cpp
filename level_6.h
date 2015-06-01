// PORTABLE
#ifndef _gcal_level_6__
#define _gcal_level_6__

#include "level_4.h"
#include "level_5.h"
#include "level_5_days.h"
#include "locationref.h"
#include "TResultEvent.h"

#define CCE_SUN  0x0001
#define CCE_TIT  0x0002
#define CCE_NAK  0x0004
#define CCE_SNK  0x0008
#define CCE_CNJ  0x0010
#define CCE_SORT 0x0100
#define CCE_ALL  0x0fff

#define CCTYPE_DATE 1
#define CCTYPE_S_ARUN 10
#define CCTYPE_S_RISE 11
#define CCTYPE_S_NOON 12
#define CCTYPE_S_SET  13
#define CCTYPE_TITHI  20
#define CCTYPE_NAKS   21
#define CCTYPE_SANK   22
#define CCTYPE_CONJ   23

void CalcEvents(TResultEvents &inEvents, CLocationRef &loc, VCTIME va, VCTIME ve, UInt32 fOptions);
void FormatTodayInfoRtf(VCTIME gTodayAct, CLocationRef & gMyLocation, TString &str);
void AvcGetTodayInfo(VCTIME vc, CLocationRef & loc, TString &str);
int CalcCalendar(TResultCalendar &daybuff, CLocationRef & loc, VCTIME date, int nDaysCount);
int FormatCalendarOld(TResultCalendar &, TString &);
int FormatCalendarRtf(TResultCalendar &, TString &);
int FormatCalendarICAL(TResultCalendar &calendar, TString &str);
int FormatCalendarVCAL(TResultCalendar &calendar, TString &str);
int FormatCalendarCSV(TResultCalendar &daybuff, TString &m_text);
int FormatEventsText(TResultEvents &, TString &);
int FormatEventsXML(TResultEvents &, TString &);
int FormatEventsRtf(TResultEvents &, TString &);

int AvcProgressWindowCreate();
int AvcProgressWindowSetRange(int nMin, int nMax);
int AvcProgressWindowSetPos(int nPos);
int AvcProgressWindowClose();

#endif