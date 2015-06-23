#pragma once

#include "LocationRef.h"
#include "GCGregorianTime.h"
#include "LangFileInfo.h"
#include "CustomEvent.h"

class GCalApp;

class GCGlobal
{
public:
	GCGlobal(void);
	~GCGlobal(void);
	static CLocationRef myLocation;
	static CLocationRef lastLocation;
	static VCTIME dateTimeShown;
	static VCTIME dateTimeToday;
	static VCTIME dateTimeTomorrow;
	static VCTIME dateTimeYesterday;
	static TLangFileList languagesList;
	static TString applicationStrings[32];
	static CCustomEventList customEventList;
	static int customEventListModified;
	static GCalApp application;
	static bool GetLangFileForAcr(const char * pszAcr, TString &strFile);

	static const char * getFileName(int n);
	static int initFolders();
	static void OpenFile(const char * fileName);
	static void SaveFile(const char * fileName);
	static void LoadInstanceData(void);
	static void SaveInstanceData(void);
};

