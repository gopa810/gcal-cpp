#include "stdafx.h"
#include "GCVaisnavaDay.h"
#include "GCStrings.h"
#include "GCRasi.h"
#include "GCTithi.h"
#include "GCNaksatra.h"
#include "GCDisplaySettings.h"
#include "GCMutableDictionary.h"
#include "math.h"

VAISNAVADAY::~VAISNAVADAY()
{

}

VAISNAVADAY::VAISNAVADAY() 
{
	// init
	//festivals = "";
	nFastType = FAST_NULL;
	nMhdType = EV_NULL;
	ekadasi_parana = false;
	ekadasi_vrata_name.Empty();
	eparana_time1 = eparana_time2 = 0.0;
	eparana_type1 = eparana_type2 = EP_TYPE_NULL;
	sankranti_zodiac = -1;
	nDST = 0;
	nCaturmasya = 0;
	moonrise.SetValue(0);
	moonset.SetValue(0);
}

void VAISNAVADAY::Clear()
{
		// init
		nFastType = FAST_NULL;
		nFeasting = FEAST_NULL;
		nMhdType = EV_NULL;
		ekadasi_parana = false;
		ekadasi_vrata_name.Empty();
		eparana_time1 = eparana_time2 = 0.0;
		sankranti_zodiac = -1;
		sankranti_day.day = 0;
		sankranti_day.shour = 0.0;
		sankranti_day.month = 0;
		sankranti_day.year = 0;
		nCaturmasya = 0;
		//moonset.SetValue(0);
		//moonrise.SetValue(0);
		//nDST = 0;
}

/* BEGIN GCAL 1.4.3 */
void VAISNAVADAY::GetTextA(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi)
{
//	static char * dow[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

	TString s1, s2;

	s1 = GetFullTithiName();

	GCStrings::getString(this->date.dayOfWeek).Left(2, s2);
	str.Format("%2d %s %d %s  %-34s%c ", this->date.day, GCStrings::GetMonthAbreviation(date.month), this->date.year
		, s2.c_str() ,s1.c_str(), (bPaksa ? GCStrings::GetPaksaChar(this->astrodata.nPaksa) : ' '));

	if (bYoga)
	{
		s2.Format("%-10s", GCStrings::GetYogaName(this->astrodata.nYoga));
		str += s2;
	}

	if (bNaks)
	{
		s2.Format("%-15s", GCStrings::GetNaksatraName(this->astrodata.nNaksatra));
		str += s2;
	}
	
	if (this->nFastType != FAST_NULL && bFlag)
		str += " *";
	else
		str += "  ";

	if (bRasi)
	{
		if (bRasi == 1)
			s2.Format("   %-15s", GCStrings::GetSankrantiName(GCRasi::GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		else
			s2.Format("   %-15s", GCStrings::GetSankrantiNameEn(GCRasi::GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		str += s2;
	}
}

void VAISNAVADAY::GetTextRtf(TString &str, int bPaksa, int bNaks, int bYoga, int bFlag, int bRasi)
{
	TString s1, s2;

	s1 = GetFullTithiName();

	GCStrings::getString(this->date.dayOfWeek).Left(2, s2);
	str.Format("\\par %2d %s %d %s\\tab %s\\tab %c ", this->date.day, GCStrings::GetMonthAbreviation(date.month), this->date.year
		, s2.c_str() ,s1.c_str(), (bPaksa ? GCStrings::GetPaksaChar(this->astrodata.nPaksa) : ' '));

	if (bYoga)
	{
		s2.Format("\\tab %s", GCStrings::GetYogaName(this->astrodata.nYoga));
		str += s2;
	}

	if (bNaks)
	{
		s2.Format("\\tab %s", GCStrings::GetNaksatraName(this->astrodata.nNaksatra));
		str += s2;
	}
	
	if (this->nFastType != FAST_NULL && bFlag)
		str += "\\tab *";
	else if (bFlag)
		str += "\\tab ";

	if (bRasi)
	{
		if (bRasi == 1)
			s2.Format("\\tab %s", GCStrings::GetSankrantiName(GCRasi::GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		else
			s2.Format("\\tab %s", GCStrings::GetSankrantiNameEn(GCRasi::GetRasi(this->astrodata.moon.longitude_deg, this->astrodata.msAyanamsa)));
		str += s2;
	}

	str += "\r\n";
}

/* END GCAL 1.4.3 */

void VAISNAVADAY::GetTextEP(TString &str)
{
	double h1, m1, h2, m2;
	m1 = modf(eparana_time1, &h1);
	if (eparana_time2 >= 0.0)
	{
		m2 = modf(eparana_time2, &h2);
		if (GCDisplaySettings::getValue(50))
			str.Format("%s %02d:%02d (%s) - %02d:%02d (%s) %s", GCStrings::getString(60).c_str(),
				int(h1), int(m1*60), GCStrings::GetParanaReasonText(eparana_type1), 
				int(h2), int(m2*60), GCStrings::GetParanaReasonText(eparana_type2),
				GCStrings::GetDSTSignature(nDST));
		else
			str.Format("%s %02d:%02d - %02d:%02d (%s)", GCStrings::getString(60).c_str(),
				int(h1), int(m1*60), int(h2), int(m2*60), GCStrings::GetDSTSignature(nDST));
	}
	else if (eparana_time1 >= 0.0)
	{
		if (GCDisplaySettings::getValue(50))
			str.Format("%s %02d:%02d (%s) %s", GCStrings::getString(61).c_str(),
				int(h1), int(m1*60), GCStrings::GetParanaReasonText(eparana_type1), GCStrings::GetDSTSignature(nDST) );
		else
			str.Format("%s %02d:%02d (%s)", GCStrings::getString(61).c_str(),
				int(h1), int(m1*60), GCStrings::GetDSTSignature(nDST) );
	}
	else
	{
		str = GCStrings::getString(62);
	}
}

bool VAISNAVADAY::GetNaksatraTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to)
{
	VCTIME start;

	start = date;
	start.shour = this->astrodata.sun.sunrise_deg / 360 + earth.tzone/24.0;

	GCNaksatra::GetNextNaksatra(earth, start, to);
	GCNaksatra::GetPrevNaksatra(earth, start, from);

	return TRUE;
}


bool VAISNAVADAY::GetTithiTimeRange(EARTHDATA earth, VCTIME &from, VCTIME &to)
{
	VCTIME start;

	start = date;
	start.shour = this->astrodata.sun.sunrise_deg / 360 + earth.tzone/24.0;

	GCTithi::GetNextTithiStart(earth, start, to);
	GCTithi::GetPrevTithiStart(earth, start, from);

	return TRUE;

}

GCMutableDictionary * VAISNAVADAY::AddEvent(int priority, int dispItem, const char * text)
{
	GCMutableDictionary * dc = new GCMutableDictionary();

	dc->setIntForKey("prio", priority);
	dc->setIntForKey("disp", dispItem);
	dc->setStringForKey("text", text);

	dayEvents.AddObject(dc);

	return dc;
}

bool VAISNAVADAY::hasEventsOfDisplayIndex(int dispIndex)
{
	for(int h = 0; h < dayEvents.Count(); h++)
	{
		GCMutableDictionary * md = dayEvents.ObjectAtIndex(h);
		if (md->intForKey("disp") == dispIndex)
			return true;
	}

	return false;
}

GCMutableDictionary * VAISNAVADAY::findEventsText(const char * text)
{
	GCMutableDictionary * md;

	for(int h = 0; h < dayEvents.Count(); h++)
	{
		md = dayEvents.ObjectAtIndex(h);
		const char * checkText = md->stringForKey("text");

		if (strstr(checkText, text) != NULL)
			return md;
	}

	return NULL;
}

bool VAISNAVADAY::AddSpecFestival(int nSpecialFestival, int nFestClass)
{
	TString str;

	if (GCStrings::GetSpecFestivalNameEx(str, nSpecialFestival))
	{
		AddEvent(PRIO_FESTIVALS_0 + (nFestClass-CAL_FEST_0)*100, nFestClass, str.c_str());

		return true;
	}

	return false;
}

TString & VAISNAVADAY::GetFullTithiName(void)
{
	static TString str;
	str = GCStrings::GetTithiName(astrodata.nTithi);

	if ((astrodata.nTithi == 10) || (astrodata.nTithi == 25) || (astrodata.nTithi == 11) || (astrodata.nTithi == 26))
	{
		if (ekadasi_parana == false)
		{
			str += " ";
			if (nMhdType == EV_NULL)
			{
				str += GCStrings::getString(58);
			}
			else
			{
				str += GCStrings::getString(59);
			}
		}
	}

	return str;
}
