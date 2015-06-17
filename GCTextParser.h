#pragma once

#include "TString.h"

class GCTextParser
{
	TString * target;
	int currentPosition;
public:

	GCTextParser(void);
	~GCTextParser(void);

	void SetTarget(TString * t);
	bool GetNextLine(TString &tmp);
};

