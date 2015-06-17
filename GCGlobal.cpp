#include "stdafx.h"
#include "GCGlobal.h"
#include "enums.h"
#include "VCal5beta.h"
#include "Location.h"

GCGlobal::GCGlobal(void)
{
}

GCGlobal::~GCGlobal(void)
{
}

CLocationRef GCGlobal::myLocation;

CLocationRef GCGlobal::lastLocation;

VCTIME GCGlobal::dateTimeShown;

VCTIME GCGlobal::dateTimeToday;

VCTIME GCGlobal::dateTimeTomorrow;

VCTIME GCGlobal::dateTimeYesterday;

TLangFileList GCGlobal::languagesList;

TString GCGlobal::applicationStrings[32];

CCustomEventList GCGlobal::customEventList;

GCalApp GCGlobal::application;

int GCGlobal::customEventListModified;

CLocationList GCGlobal::locationsList;

int GCGlobal::initFolders()
{
	char pszBuffer[1024];
	int len, i;
	len = GetModuleFileName(AfxGetInstanceHandle(), pszBuffer, 1020);
	for(i = len-1; i > 0; i--)
	{
		if (pszBuffer[i] == '\\')
		{
			pszBuffer[i+1] = '\0';
			len = i+1;
			break;
		}
	}

	GCGlobal::applicationStrings[GSTR_APPFOLDER] = pszBuffer;

	GCGlobal::applicationStrings[GSTR_CONFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_CONFOLDER] += "config\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_CONFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_LANFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_LANFOLDER] += "lang\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_LANFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_TEMFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_TEMFOLDER] += "temp\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_TEMFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_CE_FILE].Format("%scevents.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_CONF_FILE].Format("%scurrent.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_LOC_FILE].Format("%slocations.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_SSET_FILE].Format("%sshowset.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_LOCX_FILE].Format("%sloc.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());//GCAL 3.0
	GCGlobal::applicationStrings[GSTR_CEX_FILE].Format("%scev3.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());//GCAL 3.0
	GCGlobal::applicationStrings[GSTR_CONFX_FILE].Format("%sset.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TZ_FILE].Format("%stz.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_COUNTRY_FILE].Format("%sctrs.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TEXT_FILE].Format("%sstrings.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TIPS_FILE].Format("%stips.txt", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_HELP_FILE].Format("%sgcal.chm", GCGlobal::applicationStrings[GSTR_APPFOLDER].c_str());
	return 1;
}

const char * GCGlobal::getFileName(int n)
{
	return GCGlobal::applicationStrings[n].c_str();
}
