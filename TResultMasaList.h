// TResultMasaList.h: interface for the TResultMasaList class.
//
//////////////////////////////////////////////////////////////////////

#include "LocationRef.h"
#include "TString.h"

#pragma once


class TResultMasaList  
{
public:
	int n_allocated;
	class TResultMasa
	{
	public:
		int masa;
		int year;
		VCTIME vc_start;
		VCTIME vc_end;
	};
	TResultMasa * arr;
	VCTIME vc_end;
	VCTIME vc_start;
	int n_countYears;
	int n_countMasa;
	int n_startYear;
	CLocationRef m_location;

	TResultMasaList();
	virtual ~TResultMasaList();
	
	int CalculateMasaList(CLocationRef &loc, int nYear, int nCount);
	bool alloc(int nCountYears);
	int formatText(TString &str);
	int formatRtf(TString &str);
	int writeHtml(FILE * f);

};

