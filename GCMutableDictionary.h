#pragma once

#include "NSMutableArray.h"

class GCMutableDictionary
{
	class GCMDBase
	{
		char * key;
		char * value;
	public:
		GCMDBase() { key = NULL; value = NULL; }
		~GCMDBase() { setKey(NULL); setValue(NULL); }
		void setKey(const char *nk)
		{
			if (nk == key) return;
			if (key != NULL) delete key;
			key = NULL;
			if (nk != NULL)
			{
				key = new char [strlen(nk) + 1];
				strcpy(key, nk);
			}
		}
		void setValue(const char *nk)
		{
			if (nk == value) return;
			if (value != NULL) delete value;
			value = NULL;
			if (nk != NULL)
			{
				value = new char [strlen(nk) + 1];
				strcpy(value, nk);
			}
		}
		char * getKey()
		{
			return key;
		}
		char * getValue()
		{
			return value;
		}
	};

	NSMutableArray data;
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

