#pragma once

#include "showset.h"

class GCDisplaySettings
{
protected:
	static CShowSetting gss[];

public:
	static int getCount();
	static int getCountChanged();
	static char * getSettingName(int i);
	static int getValue(int i);
	static void setValue(int i, int val);
	static void setValue(const char * pszSign, int val);
	static void readFile(const char * psz);

	GCDisplaySettings(void);
	~GCDisplaySettings(void);
};

