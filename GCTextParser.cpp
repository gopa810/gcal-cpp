#include "stdafx.h"
#include "GCTextParser.h"


GCTextParser::GCTextParser(void)
{

}


GCTextParser::~GCTextParser(void)
{
}

void GCTextParser::SetTarget(TString * t)
{
	target = t;
	currentPosition = 0;
}

bool GCTextParser::GetNextLine(TString &strLine)
{
	int index = currentPosition, length;
	
	if (target == NULL)
		return false;

	if (index >= target->GetLength())
		return false;

	TCHAR tc;
	strLine.Empty();
	length = target->GetLength();

	while(1)
	{
		if (index >= length)
			break;

		tc = target->GetAt(index);
		if (tc == '\r');
		else if (tc == '\n')
		{
			index++;
			break;
		}
		else
		{
			strLine += tc;
		}
		index++;
	}

	currentPosition = index;

	return true;
}


