// PORTABLE
// Location.h: interface for the CLocation class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "NSMutableArray.h"
#include "TString.h"

class CLocation  
{
public:
	void Empty();
	TString m_strCountry;
	TString m_strCity;
	double m_fLongitude;
	double m_fLatitude;
	double m_fTimezone;
	int m_nTimezoneId;

	CLocation();
	virtual ~CLocation();

};

class CLocationList
{
	static NSMutableArray<CLocation> locationList;
	static bool m_bModified;

public:
	static bool OpenFile(const char * pszList);
	static bool SaveAs(const char * lpszFileName, int nType);
	static bool ImportFile(const char * pszFile, bool bDeleteCurrent);
	static void RemoveAll();
	static void Add(CLocation * loc);
	static void RemoveAt(int index);
	static void InitInternal(const char *);
	static bool IsModified(void);
	static int RenameCountry(const char * pszOld, const char * pszNew);

	CLocationList();
	virtual ~CLocationList();

	static int LocationCount(void);
	static CLocation * LocationAtIndex(int index);
};


