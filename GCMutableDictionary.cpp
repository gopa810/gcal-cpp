#include "stdafx.h"
#include "GCMutableDictionary.h"
#include "GCStringPair.h"

GCMutableDictionary::GCMutableDictionary(void)
{
}


GCMutableDictionary::~GCMutableDictionary(void)
{
	data.RemoveAllObjects();
}

int GCMutableDictionary::findRecordIndex(const char * key)
{
	for(int i = 0; i < data.Count(); i++)
	{
		GCStringPair * p = data.ObjectAtIndex(i);
		if (strcmp(key, p->getKey()) == 0)
		{
			return i;
		}
	}

	return -1;
}

bool GCMutableDictionary::containsKey(const char * key)
{
	return findRecordIndex(key) >= 0;
}

void GCMutableDictionary::setIntForKey(const char * key, int value)
{
	char sz[32];
	sprintf(sz, "%d", value);
	setStringForKey(key, sz);
}

void GCMutableDictionary::setDoubleForKey(const char * key, double value)
{
	char sz[32];
	sprintf(sz, "%f", value);
	setStringForKey(key, sz);
}

void GCMutableDictionary::setStringForKey(const char * key, const char * value)
{
	int i = findRecordIndex(key);
	if (i < 0)
	{
		if (value != NULL)
		{
			GCStringPair * p = new GCStringPair();
			p->setKey(key);
			p->setValue(value);
			data.AddObject(p);
		}
	}
	else
	{
		if (value != NULL)
		{
			GCStringPair * p = data.ObjectAtIndex(i);
			p->setValue(value);
		}
		else
		{
			data.RemoveObject(i);
		}
	}
}

const char * GCMutableDictionary::stringForKey(const char * key)
{
	int i = findRecordIndex(key);
	if (i >= 0)
	{
		GCStringPair * p = data.ObjectAtIndex(i);
		return p->getValue();
	}

	return NULL;
}

int GCMutableDictionary::intForKey(const char * key)
{
	const char * v = stringForKey(key);
	return v == NULL ? 0 : atoi(v);
}

double GCMutableDictionary::doubleForKey(const char * key)
{
	const char * v = stringForKey(key);
	return v == NULL ? 0.0 : atof(v);
}

// returns -1 : s1 < s2
//          0 : s1 = s2
//         +1 : s1 > s2
int GCMutableDictionary::compareStrings(const char * s1, const char * s2)
{
	if (s1 == NULL)
	{
		if (s2 == NULL)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if (s2 == NULL)
		{
			return 1;
		}
		else
		{
			int cm = strcmp(s1, s2);
			return (cm > 0 ? 1 : (cm < 0 ? -1 : 0));
		}
	}
}

