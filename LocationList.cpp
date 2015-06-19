
#include "stdafx.h"
#include "Location.h"
#include "TFile.h"
#include "TFileRichList.h"
#include "resource.h"
#include "TCountry.h"
#include "TTimeZone.h"


NSMutableArray<CLocation> CLocationList::locationList;

bool CLocationList::m_bModified;

CLocationList::CLocationList()
{
	m_bModified = true;
}

CLocationList::~CLocationList()
{
	RemoveAll();
}

bool CLocationList::OpenFile(const char * pszFileList)
{
	TString strA, strB;
	TFileRichList file;
	CLocation * loc = NULL;
	int notNullCountry = 0;

	if (!TFile::FileExists(pszFileList))
	{
		TFile::CreateFileFromResource(IDR_FILE_LOCATIONS, pszFileList);
	}

	// try to open
	if (file.Open(pszFileList, "r"))
	{
		while(file.ReadLine())
		{
			loc = new CLocation;
			if (loc != NULL)
			{
				loc->m_strCity = file.GetField(0);
				loc->m_strCountry = file.GetField(1);
				if (loc->m_strCountry.GetLength() > 0)
					notNullCountry ++;
				loc->m_fLongitude = atof(file.GetField(2));
				loc->m_fLatitude = atof(file.GetField(3));
				loc->m_fTimezone = atof(file.GetField(4));
				loc->m_nTimezoneId = atoi(file.GetField(5));

				locationList.AddObject(loc);
			}
		}

		file.Close();
	}

	return true;
}


bool CLocationList::IsModified(void)
{
	return m_bModified;
}

bool CLocationList::SaveAs(const char * lpszFileName, int nType)
{
	TString key, val, str, strTemp;
	TFile f;
	int i, ni;

	if (f.Open(lpszFileName, "w") == FALSE)
	{
		return FALSE;
	}

	switch(nType)
	{
	case 1:
		f.WriteString("<xml>\n");
		f.WriteString("\t<countries>\n");
		ni = TCountry::GetCountryCount();
		for(i = 0; i < ni; i++)
		{
			str.Format("\t<ccn country=\"%s\" continent=\"%s\" />\n", TCountry::GetCountryNameByIndex(i),
				TCountry::GetCountryContinentNameByIndex(i));
			f.WriteString(str);
		}
		f.WriteString("\t</countries>\n");
		f.WriteString("\t<dsts>\n");
		ni = TTimeZone::GetTimeZoneCount();
		for(i = 1; i < ni; i++)
		{
			TTimeZone::GetXMLString(str, i);
			f.WriteString(str);
		}
		f.WriteString("\t</dsts>\n");
		f.WriteString("\t<cities>\n");
		for(int i = 0; i < locationList.Count(); i++)
		{
			CLocation * lc = locationList.ObjectAtIndex(i);
			str.Format("\t<loc city=\"%s\" lon=\"%f\" lat=\"%f\" tzone=\"%f\"\n\t\tcountry=\"%s\" />\n",
				lc->m_strCity.c_str(), lc->m_fLongitude, lc->m_fLatitude,
				lc->m_fTimezone, lc->m_strCountry.c_str());
			str.Replace("&", "&amp;");
			f.WriteString(str);
		}
		f.WriteString("\t</cities>\n");
		f.WriteString("</xml>");
		break;
	case 2:
		f.WriteString("Countries:\n");
		ni = TCountry::GetCountryCount();
		for(i = 0; i < ni; i++)
		{
			str.Format("%s, %s\n", TCountry::GetCountryNameByIndex(i),
				TCountry::GetCountryContinentNameByIndex(i));
			f.WriteString(str);
		}
		f.WriteString("Daylight Saving Time Systems:\n");
		ni = TTimeZone::GetTimeZoneCount();
		for(i = 1; i < ni; i++)
		{
			str.Format("\t%s\n", TTimeZone::GetTimeZoneName(i));
			f.WriteString(str);
		}
		f.WriteString("Cities:\n");
		for(int i = 0; i < locationList.Count(); i++)
		{
			CLocation * lc = locationList.ObjectAtIndex(i);
			str.Format("\t%-23s%-17s %+03.6f %+03.6f %+02.6f\n",
				lc->m_strCity.c_str(),
				lc->m_strCountry.c_str(),
				lc->m_fLongitude,
				lc->m_fLatitude,
				lc->m_fTimezone);
			f.WriteString(str);
		}
		break;
	case 3:
		for(int i = 0; i < locationList.Count(); i++)
		{
			CLocation * lc = locationList.ObjectAtIndex(i);
			// city
			f.WriteString("@city=");
			f.WriteString(lc->m_strCity);
			f.WriteString("\n");

			f.WriteString("@country=");
			f.WriteString(lc->m_strCountry);
			f.WriteString("\n");

			f.WriteString("@lat=");
			strTemp.Format("%f", lc->m_fLatitude);
			f.WriteString(strTemp);
			f.WriteString("\n");

			f.WriteString("@long=");
			strTemp.Format("%f", lc->m_fLongitude);
			f.WriteString(strTemp);
			f.WriteString("\n");

			f.WriteString("@timezone=");
			strTemp.Format("%f", lc->m_fTimezone);
			f.WriteString(strTemp);
			f.WriteString("\n");

			f.WriteString("@dst=");
			strTemp.Format("%d", lc->m_nTimezoneId);
			f.WriteString(strTemp);
			f.WriteString("\n@create\n\n");

		}
		break;
	case 4:
		for(int i = 0; i < locationList.Count(); i++)
		{
			CLocation * lc = locationList.ObjectAtIndex(i);
			strTemp.Format("26700 %s|%s|%f|%f|%.2f|%d\n", lc->m_strCity.c_str(), lc->m_strCountry.c_str(), lc->m_fLongitude,
				lc->m_fLatitude, lc->m_fTimezone, lc->m_nTimezoneId);
			f.WriteString(strTemp);
		}
	default:
		break;
	}

	f.Close();

	return TRUE;
}

bool CLocationList::ImportFile(const char * pszFile, bool bDeleteCurrent)
{
	if (bDeleteCurrent)
	{
		RemoveAll();
	}

	m_bModified = true;

	return OpenFile(pszFile);
}

void CLocationList::RemoveAll()
{
	locationList.RemoveAllObjects();
	m_bModified = true;
}

void CLocationList::Add(CLocation * loc)
{
	locationList.AddObject(loc);
	m_bModified = true;
}

void CLocationList::RemoveAt(int index)
{
	locationList.RemoveObject(index);
	m_bModified = true;
}

void CLocationList::InitInternal(const char * fileName)
{
	TFile::DeleteFileA(fileName);
	OpenFile(fileName);
}

int CLocationList::RenameCountry(const char *pszOld, const char *pszNew)
{
	for(int i = 0; i < locationList.Count(); i++)
	{
		CLocation * L = locationList.ObjectAtIndex(i);
		if (strcmp(L->m_strCountry.c_str(), pszOld)==0)
			L->m_strCountry = pszNew;
	}
	return 1;
}


int CLocationList::LocationCount(void)
{
	return locationList.Count();
}


CLocation * CLocationList::LocationAtIndex(int index)
{
	return locationList.ObjectAtIndex(index);
}
