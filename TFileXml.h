#pragma once

#include "TString.h"
#include "GCDayHours.h"
#include "GCGregorianTime.h"

class TFileXml  
{
	FILE * fout;
public:
	void initWithFile(FILE * file) { fout = file; }
	void write(VCTIME vc);
	void write(TString &s);
	void write(const char * s);
	void write(DAYTIME dt);
	void write(int n);
	void write(double d);
	void write(TCHAR * p);
	TFileXml();
	virtual ~TFileXml();

};

