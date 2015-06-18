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

TCountry TCountry::gcountries_var[512];

TStringStorage TCountry::gcountstr;

int TCountry::g_countries_var_count = 0;

int TCountry::InitWithFile(const char * strFile)
{
	int i, j;
	int retVal = 0;
	const char * pCode = NULL;
	int opened = 0;
	TFileRichList F;

	if (!PathFileExists(strFile))
	{
		HRSRC hsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_FILE_COUNTRY), "TEXT");
		HGLOBAL hfile = LoadResource(NULL, hsrc);
		LPVOID data = LockResource(hfile);
		DWORD sizerc = SizeofResource(NULL, hsrc);

		FILE * fileToWrite = fopen(strFile, "wb");
		if (fileToWrite != NULL)
		{
			fwrite(data, 1, sizerc, fileToWrite);
			fclose(fileToWrite);
		}
	}

	if (F.Open(strFile, "rt"))
	{
		//
		// init from file
		//
		retVal = 1;
		i = 0;
		while(F.ReadLine() && i <512)
		{
			if (atoi(F.GetTag()) == 77)
			{
				pCode = F.GetField(0);
				gcountries_var[i].pszAcr = gcountstr.add(pCode);
				gcountries_var[i].code = ((unsigned char)(pCode[0]))*256 + (unsigned char)pCode[1];
				gcountries_var[i].pszName = gcountstr.add(F.GetField(1));
				gcountries_var[i].continent = atoi(F.GetField(2));
				i++;
			}
		}
		opened = 1;
		g_countries_var_count = i;
		F.Close();
	}

	return retVal;
}

const char * TCountry::GetCountryName(WORD w)
{
	int i, j;

	i = 0;
	j = g_countries_var_count;

	for(i = 0; i < j; i++)
	{
		if (gcountries_var[i].code == w)
			return gcountries_var[i].pszName;
	}

	return "";
}

const char * TCountry::GetCountryContinentName(WORD w)
{
	int i, j;

	j = g_countries_var_count;

	for(i = 0; i < j; i++)
	{
		if (gcountries_var[i].code == w)
			return gcontinents[gcountries_var[i].continent];
	}

	return "";
}

int TCountry::GetCountryCount()
{
	return g_countries_var_count;
}

const char * TCountry::GetCountryNameByIndex(int nIndex)
{
	return gcountries_var[nIndex].pszName;
}

const char * TCountry::GetCountryContinentNameByIndex(int nIndex)
{
	return gcontinents[gcountries_var[nIndex].continent];
}

const char * TCountry::GetCountryAcronymByIndex(int nIndex)
{
	return gcountries_var[nIndex].pszAcr;
}

int TCountry::SaveToFile(const char *szFile)
{
	TFileRichList F;

	F.Open(szFile, "wt");
	if (F.m_fHandle == NULL)
		return 0;

	int i;
	for(i = 0 ; i < g_countries_var_count; i++)
	{
		F.Clear();
		F.AddTag(77);
		F.AddText(gcountries_var[i].pszAcr);
		F.AddText(gcountries_var[i].pszName);
		F.AddInt(gcountries_var[i].continent);
		F.WriteLine();
	}

	return 1;
}

int TCountry::AddCountry(LPCTSTR pszCode, LPCTSTR pszName, int nContinent)
{
	if(g_countries_var_count <512)
	{
		gcountries_var[g_countries_var_count].pszAcr = gcountstr.add(pszCode);
		gcountries_var[g_countries_var_count].code = ((unsigned char)(pszCode[0]))*256 + (unsigned char)pszCode[1];
		gcountries_var[g_countries_var_count].pszName = gcountstr.add(pszName);
		gcountries_var[g_countries_var_count].continent = nContinent;
		g_countries_var_count++;
	}

	return g_countries_var_count;
}

int TCountry::SetCountryName(int nSelected, const char *psz)
{
	gcountries_var[nSelected].pszName = gcountstr.add(psz);
	return 1;
}
