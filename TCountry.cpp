// TCountry.cpp: implementation of the TCountry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCountry.h"
#include "TFileRichList.h"
#include "resource.h"

// PORTABLE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char * TCountry::gcontinents[] = 
{
		"",
		"Europe", //1
		"Asia",   //2
		"Africa", //3
		"America",//4
		"Pacific",//5
		"Indiana",//6
		"Atlantic",//7
		""
};

int TCountry::_modified = 0;

NSMutableArray<TCountry> TCountry::gcountries;

int TCountry::InitWithFile(const char * strFile)
{
	const char * pCode = NULL;
	TFileRichList F;

	if (!TFile::FileExists(strFile))
	{
		TFile::CreateFileFromResource(IDR_FILE_COUNTRY, strFile);
	}

	if (F.Open(strFile, "rt"))
	{
		//
		// init from file
		//
		while(F.ReadLine())
		{
			if (atoi(F.GetTag()) == 77)
			{
				AddCountry(F.GetField(0), F.GetField(1), atoi(F.GetField(2)));
			}
		}
		F.Close();
	}

	return gcountries.Count();
}

const char * TCountry::GetCountryName(WORD w)
{
	for(int i = 0; i < gcountries.Count(); i++)
	{
		if (gcountries.ObjectAtIndex(i)->code == w)
			return gcountries.ObjectAtIndex(i)->name.c_str();
	}

	return "";
}

const char * TCountry::GetCountryContinentName(WORD w)
{
	for(int i = 0; i < gcountries.Count(); i++)
	{
		if (gcountries.ObjectAtIndex(i)->code == w)
			return gcontinents[gcountries.ObjectAtIndex(i)->continent];
	}

	return "";
}

int TCountry::GetCountryCount()
{
	return gcountries.Count();
}

const char * TCountry::GetCountryNameByIndex(int nIndex)
{
	return gcountries.ObjectAtIndex(nIndex)->name.c_str();
}

const char * TCountry::GetCountryContinentNameByIndex(int nIndex)
{
	return gcontinents[gcountries.ObjectAtIndex(nIndex)->continent];
}

const char * TCountry::GetCountryAcronymByIndex(int nIndex)
{
	return gcountries.ObjectAtIndex(nIndex)->abbreviatedName.c_str();
}

int TCountry::SaveToFile(const char *szFile)
{
	TFileRichList F;

	F.Open(szFile, "wt");
	if (F.m_fHandle == NULL)
		return 0;

	int i;
	for(i = 0 ; i < gcountries.Count(); i++)
	{
		F.Clear();
		F.AddTag(77);
		F.AddText(gcountries.ObjectAtIndex(i)->abbreviatedName.c_str());
		F.AddText(gcountries.ObjectAtIndex(i)->name.c_str());
		F.AddInt(gcountries.ObjectAtIndex(i)->continent);
		F.WriteLine();
	}

	return 1;
}

int TCountry::AddCountry(LPCTSTR pszCode, LPCTSTR pszName, int nContinent)
{
	TCountry * country = new TCountry();

	country->abbreviatedName = pszCode;
	country->code = ((unsigned char)(pszCode[0]))*256 + (unsigned char)pszCode[1];
	country->name = pszName;
	country->continent = nContinent;

	gcountries.AddObject(country);

	return gcountries.Count();
}

int TCountry::SetCountryName(int nSelected, const char *psz)
{
	gcountries.ObjectAtIndex(nSelected)->name = psz;
	_modified = 1;
	return 1;
}


unsigned short int TCountry::GetCountryCode(int nIndex)
{
	return gcountries.ObjectAtIndex(nIndex)->code;
}


bool TCountry::IsModified(void)
{
	return _modified;
}
