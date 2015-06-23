// PORTABLE
// level_5_days.h: interface for the TResultCalendar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL_5_DAYS_H__957DEA2E_87AF_4C5B_A359_4E0C8F511234__INCLUDED_)
#define AFX_LEVEL_5_DAYS_H__957DEA2E_87AF_4C5B_A359_4E0C8F511234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LocationRef.h"
#include "GCVaisnavaDay.h"
#include "GCGregorianTime.h"
#include "GCEarthData.h"

#define CDB_MAXDAYS 16
#define BEFORE_DAYS 8

class TResultCalendar  
{
public:
	VAISNAVADAY * m_pData;
	int m_nCount;
	int m_PureCount;
	CLocationRef m_Location;
	VCTIME m_vcStart;
	int m_vcCount;
	bool updateCalculationProgress;
public:
	void ResolveFestivalsFasting(int i);
	int CalculateEParana(VAISNAVADAY &s, VAISNAVADAY &t, double &begin, double &end, EARTHDATA earth);
	int FindDate(VCTIME vc);
	bool IsFestivalDay(VAISNAVADAY &yesterday, VAISNAVADAY &today, int nTithi);
	VAISNAVADAY * GetDay(int nIndex);
	int MahadvadasiCalc(int nIndex, EARTHDATA earth);
	int CompleteCalc(int nIndex, EARTHDATA earth);
	int EkadasiCalc(int nIndex, EARTHDATA earth);
	int ExtendedCalc(int nIndex, EARTHDATA earth);
	int CalculateCalendar(CLocationRef & loc, VCTIME begDate, int iCount);
	bool IsMhd58(int nIndex, int &nMahaType);
	bool NextNewFullIsVriddhi(int nIndex, EARTHDATA earth);
	int writeXml(FILE * fout);
	static int formatPlainTextDay(VAISNAVADAY * pvd, TString &dayText);
	int formatPlainText(TString &m_text);
	int formatRtf(TString &m_text);
	static int formatRtfDay(VAISNAVADAY * pvd, TString &dayText);
	int formatICal(TString &str);
	int formatVCal(TString &str);
	int formatCsv(TString &m_text);
	int writeHtml(FILE * fout);
	int writeTableHtml(FILE * fout);
	static const char * getDayBkgColorCode(VAISNAVADAY * p);

	int DAYS_TO_ENDWEEK(int lastMonthDay);
	int DAYS_FROM_BEGINWEEK(int firstMonthDay);
	int DAY_INDEX(int day);


	int nBeg;
	int nTop;
	TResultCalendar();
	virtual ~TResultCalendar();

	int top;
	VAISNAVADAY days[CDB_MAXDAYS];

	int Push(VAISNAVADAY &day);
};

#endif // !defined(AFX_LEVEL_5_DAYS_H__957DEA2E_87AF_4C5B_A359_4E0C8F511234__INCLUDED_)
