#pragma once

#include "LocationRef.h"
#include "GCGregorianTime.h"
#include "LangFileInfo.h"
#include "CustomEvent.h"

class GCalApp;
class CLocationList;

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
	static CLocationList locationsList;

	static const char * getFileName(int n);
	static int initFolders();
};

