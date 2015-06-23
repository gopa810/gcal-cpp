
#pragma once

#include "GCVedicTime.h"
#include "GCGregorianTime.h"

class CCommandLineVCal : public CCommandLineInfo  
{
public:
	int GetArg_Interval(const char * pszText, int &A, int &B);
	int GetArg_Paksa(const char *  str);
	int GetArg_Date(const char *  str, VCTIME &vc);
	int GetArg_VaisnDate(const char *  str, VATIME &vc);
	int GetArg_Tithi(const char *  str);
	int GetArg_TimeZone(const char *  str, double &TimeZone);
	int GetArg_Time(const char *  str, VCTIME & vc);
	int GetArg_EarthPos(const char *  str, double &, double &);
	int MasaIndexToInternal(int nMasa);
	int GetArg_Masa(const char *  str);
	int GetArg_Int(const char *  str, int &nInteger);
	int GetArg_Year(const char *  str, int &nYear);
	int SetArgLastError(int i);
	int GetArgLastError();
	int gnLastError;
	int ParseCommandArguments(const char * m_lpCmdLine);

	CCommandLineVCal();
	virtual ~CCommandLineVCal();

};


