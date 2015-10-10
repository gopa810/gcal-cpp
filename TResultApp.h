#pragma once

#include "locationref.h"
#include "GCDayData.h"


#define TRESULT_APP_CELEBS 3

class TResultApp
{
public:
	CLocationRef location;
	VCTIME eventTime;
	DAYDATA details;
	bool b_adhika;
	int celeb_gy[TRESULT_APP_CELEBS];
	VCTIME celeb_date[TRESULT_APP_CELEBS];

	void calculateAppDay(CLocationRef &location, VCTIME eventDate);
	void formatPlainText(TString &strResult);
	void formatRtf(TString &strResult);
	void formatXml(TString &strResult);
	void writeHtml(FILE *F);
};

