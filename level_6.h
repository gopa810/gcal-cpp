// PORTABLE
#ifndef _gcal_level_6__
#define _gcal_level_6__

#include "level_6.h"
#include "level_5_days.h"
#include "locationref.h"
#include "TResultEvent.h"




void FormatTodayInfoRtf(VCTIME gTodayAct, CLocationRef & gMyLocation, TString &str);
void AvcGetTodayInfo(VCTIME vc, CLocationRef & loc, TString &str);
int CalcCalendar(TResultCalendar &daybuff, CLocationRef & loc, VCTIME date, int nDaysCount);
int FormatCalendarOld(TResultCalendar &, TString &);
int FormatCalendarRtf(TResultCalendar &, TString &);
int FormatCalendarICAL(TResultCalendar &calendar, TString &str);
int FormatCalendarVCAL(TResultCalendar &calendar, TString &str);
int FormatCalendarCSV(TResultCalendar &daybuff, TString &m_text);

int AvcProgressWindowCreate();
int AvcProgressWindowSetRange(int nMin, int nMax);
int AvcProgressWindowSetPos(int nPos);
int AvcProgressWindowClose();

#endif