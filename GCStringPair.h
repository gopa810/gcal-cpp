#pragma once
class GCStringPair
{
	char * key;
	char * value;
public:
	GCStringPair(void);
	~GCStringPair(void);
	void setKey(const char *nk);
	void setValue(const char *nk);
	char * getKey();
	char * getValue();

};

