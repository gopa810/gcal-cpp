// TCountry.h: interface for the TCountry class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


// PORTABLE
#include "NSMutableArray.h"
#include "TString.h"

class TCountry  
{
	static int _modified;
public:
	static const char *  gcontinents[];
	static NSMutableArray<TCountry> gcountries;

public:
	static int InitWithFile(const char * strFile);
	static int SetCountryName(int nSelected, const char * psz);
	static int AddCountry(LPCTSTR pszCode, LPCTSTR pszName, int nContinent);
	static int SaveToFile(const char * szFile);
	static int GetCountryCount();
	static const char * GetCountryAcronymByIndex(int nIndex);
	static const char * GetCountryNameByIndex(int nIndex);
	static const char * GetCountryContinentName(WORD w);
	static const char * GetCountryContinentNameByIndex(int nIndex);
	static const char * GetCountryName(WORD w);
	
	TString abbreviatedName;
	TString name;
	short unsigned int code;
	unsigned char continent;

	static unsigned short int GetCountryCode(int nIndex);
	static bool IsModified(void);
};

