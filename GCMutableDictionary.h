#pragma once

#include "NSMutableArray.h"
#include "GCMutableStringPairArray.h"


class GCMutableDictionary
{
	GCMutableStringPairArray data;
	int findRecordIndex(const char * key);
public:
	GCMutableDictionary(void);
	~GCMutableDictionary(void);

	bool containsKey(const char * key);
	void setIntForKey(const char * key, int value);
	void setDoubleForKey(const char * key, double value);
	void setStringForKey(const char * key, const char * value);
	const char * stringForKey(const char * key);
	int intForKey(const char * key);
	double doubleForKey(const char * key);

	static int compareStrings(const char * a, const char * b);

};

