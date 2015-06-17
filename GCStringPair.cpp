#include "stdafx.h"
#include "GCStringPair.h"


GCStringPair::GCStringPair(void)
{
	key = NULL; 
	value = NULL;
}


GCStringPair::~GCStringPair(void)
{
	setKey(NULL); 
	setValue(NULL);
}

void GCStringPair::setKey(const char *nk)
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
void GCStringPair::setValue(const char *nk)
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
char * GCStringPair::getKey()
{
	return key;
}
char * GCStringPair::getValue()
{
	return value;
}
