#include "stdafx.h"
#include "TResultEvent.h"
#include "GCDisplaySettings.h"
#include "TTimeZone.h"
#include "GCAyanamsha.h"
#include "GCConjunction.h"
#include "GCNaksatra.h"
#include "GCRasi.h"
#include "GCSankranti.h"
#include "GCTithi.h"
#include "GCStrings.h"
#include "GCStringBuilder.h"
#include "GCLayoutData.h"

// PORTABLE

TResultEvents::TResultEvents()
{
	b_sorted = true;
	p_events = NULL;
	n_count = 0;
	n_size = 0;
	e_ptr = NULL;
	m_options = 0;
}

TResultEvents::~TResultEvents()
{
	if (p_events != NULL)
	{
		delete [] p_events;
		p_events = NULL;
	}
	if (e_ptr != NULL)
	{
		delete [] e_ptr;
		e_ptr = NULL;
	}
}
Boolean TResultEvents::AddEvent(VCTIME inTime, int inType, int inData, int inDst)
{
	if (p_events == NULL)
	{
		p_events = new TDayEvent[64];
		if (p_events == NULL)
			return false;
		n_size = 64;
	}
	if (n_count >= n_size)
	{
		TDayEvent * p = new TDayEvent[n_size + 64];
		if (p == NULL)
			return false;
		for(unsigned int i = 0; i < n_count; i++)
			p[i] = p_events[i];
		delete [] p_events;
		n_size += 64;
		p_events = p;
	}
	switch(inDst)
	{
	case 0:
		p_events[n_count].nDst = 0;
		break;
	case 1:
		if (inTime.shour >= 2/24.0)
		{
			inTime.shour += 1/24.0;
			inTime.NormalizeValues();
			p_events[n_count].nDst = 1;
		}
		else
		{
			p_events[n_count].nDst = 0;
		}
		break;
	case 2:
		inTime.shour += 1/24.0;
		inTime.NormalizeValues();
		p_events[n_count].nDst = 1;
		break;
	case 3:
		if (inTime.shour <= 2/24.0)
		{
			inTime.shour += 1/24.0;
			inTime.NormalizeValues();
			p_events[n_count].nDst = 1;
		}
		else
		{
			p_events[n_count].nDst = 0;
		}
		break;
	default:
		p_events[n_count].nDst = 0;
	}
	p_events[n_count].Time = inTime;
	p_events[n_count].nData = inData;
	p_events[n_count].nType = inType;
	n_count++;
	return true;
}

void TResultEvents::Sort(Boolean inSort)
{
	unsigned int i, j;
	TDayEvent * p;

	b_sorted = inSort;
	if (inSort == false)
		return;

	if (e_ptr != NULL)
	{
		delete [] e_ptr;
		e_ptr = NULL;
	}

	e_ptr = new TDayEventPtr[n_size];
	if (e_ptr == NULL)
	{
		b_sorted = false;
		return;
	}

	for(i = 0; i < n_count; i++)
		e_ptr[i] = &(p_events[i]);

	for(i = 0; i < n_count - 1; i++)
	{
		for(j = i+1; j < n_count; j++)
		{
			if ((e_ptr[i]->Time.GetDayInteger() + e_ptr[i]->Time.shour + e_ptr[i]->nDst/24.0) 
				> (e_ptr[j]->Time.GetDayInteger() + e_ptr[j]->Time.shour + e_ptr[j]->nDst/24.0))
			{
				p = e_ptr[i];
				e_ptr[i] = e_ptr[j];
				e_ptr[j] = p;
			}
		}
	}
}

void TResultEvents::clear()
{
	n_count = 0;
	if (e_ptr != NULL)
	{
		delete [] e_ptr;
		e_ptr = NULL;
	}
}

void TResultEvents::CalculateEvents(CLocationRef &loc, VCTIME vcStart, VCTIME vcEnd)
{
	VCTIME vc;
	SUNDATA sun;
	int ndst = 0;
	int nData;
	
	TResultEvents &inEvents = *this;
	this->clear();
	this->m_location = loc;
	this->m_vcStart = vcStart;
	this->m_vcEnd = vcEnd;
	
	VCTIME vcAdd, vcNext;
	EARTHDATA earth = (EARTHDATA)loc;

	vc = vcStart;

	vcAdd = vc;
	vcAdd.InitWeekDay();

	double sunRise, sunSet;
	double r1, r2;
	double previousSunriseHour, todaySunriseHour;
	double previousLongitude = -100;
	double todayLongitude = 0;
	double fromTimeLimit = 0;

	while(vcAdd.IsBeforeThis(vcEnd))
	{
		if (GCDisplaySettings::getValue(COREEVENTS_SUN))
		{
			ndst = TTimeZone::determineDaylightChange(vcAdd, loc.m_nDST);
			sun.SunCalc(vcAdd, earth);

			vcAdd.shour = sun.arunodaya.GetDayTime();
			inEvents.AddEvent(vcAdd, CCTYPE_S_ARUN, 0, ndst);

			vcAdd.shour = sunRise = sun.rise.GetDayTime();
			inEvents.AddEvent(vcAdd, CCTYPE_S_RISE, 0, ndst);

			vcAdd.shour = sun.noon.GetDayTime();
			inEvents.AddEvent(vcAdd, CCTYPE_S_NOON, 0, ndst);

			vcAdd.shour = sunSet = sun.set.GetDayTime();
			inEvents.AddEvent(vcAdd, CCTYPE_S_SET, 0, ndst);
		}
		else
		{
			ndst = TTimeZone::determineDaylightChange(vcAdd, loc.m_nDST);
			sun.SunCalc(vcAdd, earth);
			sunRise = sun.rise.GetDayTime();
			sunSet = sun.set.GetDayTime();
		}

		if (GCDisplaySettings::getValue(COREEVENTS_ASCENDENT))
		{
			todayLongitude = sun.longitude_deg;
			vcAdd.shour = sunRise;
			todaySunriseHour = sunRise;
			if (previousLongitude < -10)
			{
				VCTIME prevSunrise = vcAdd;
				prevSunrise.PreviousDay();
				sun.SunCalc(prevSunrise, earth);
				previousSunriseHour = sun.rise.GetDayTime() - 1;
				previousLongitude = sun.longitude_deg;
				fromTimeLimit = 0;
			}

			double a, b;
			double jd = vcAdd.GetJulianComplete();
			double ayan = GCAyanamsha::GetAyanamsa(jd);
			r1 = put_in_360(previousLongitude - ayan) / 30;
			r2 = put_in_360(todayLongitude - ayan) / 30;

			while(r2 > r1 + 13)
			{
				r2 -= 12.0;
			}
			while(r2 < r1 + 11)
			{
				r2 += 12.0;
			}

			a = (r2 - r1) / (todaySunriseHour - previousSunriseHour);
			b = r2 - a * todaySunriseHour;

			for(double tr = ceil(r1); tr < r2; tr += 1.0)
			{
				double tm = ( tr - b ) / a;
				if (tm > fromTimeLimit)
				{
					vcNext = vcAdd;
					vcNext.shour = tm;
					vcNext.NormalizeValues();
					inEvents.AddEvent(vcNext, CCTYPE_ASCENDENT, (int)tr, ndst);
				}
			}

			previousLongitude = todayLongitude;
			previousSunriseHour = todaySunriseHour - 1;
			fromTimeLimit = previousSunriseHour;
		}

		if (GCDisplaySettings::getValue(COREEVENTS_RAHUKALAM))
		{
			SUNDATA::CalculateKala(sunRise, sunSet, vcAdd.dayOfWeek, &r1, &r2, KT_RAHU_KALAM);

			vcAdd.shour = r1;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_START, KT_RAHU_KALAM, ndst); 

			vcAdd.shour = r2;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_END, KT_RAHU_KALAM, ndst);
		}

		if (GCDisplaySettings::getValue(COREEVENTS_YAMAGHANTI))
		{
			SUNDATA::CalculateKala(sunRise, sunSet, vcAdd.dayOfWeek, &r1, &r2, KT_YAMA_GHANTI);

			vcAdd.shour = r1;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_START, KT_YAMA_GHANTI, ndst); 

			vcAdd.shour = r2;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_END, KT_YAMA_GHANTI, ndst);
		}

		if (GCDisplaySettings::getValue(COREEVENTS_GULIKALAM))
		{
			SUNDATA::CalculateKala(sunRise, sunSet, vcAdd.dayOfWeek, &r1, &r2, KT_GULI_KALAM);

			vcAdd.shour = r1;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_START, KT_GULI_KALAM, ndst); 

			vcAdd.shour = r2;
			inEvents.AddEvent(vcAdd, CCTYPE_KALA_END, KT_GULI_KALAM, ndst);
		}

		vcAdd.NextDay();
	}

	if (GCDisplaySettings::getValue(COREEVENTS_TITHI))
	{
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			nData = GCTithi::GetNextTithiStart(earth, vcAdd, vcNext);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_TITHI, nData, ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.shour += 0.2;
			if (vcAdd.shour >= 1.0)
			{
				vcAdd.shour -= 1.0;
				vcAdd.NextDay();
			}
		}
	}

	if (GCDisplaySettings::getValue(COREEVENTS_NAKSATRA))
	{
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			nData = GCNaksatra::GetNextNaksatra(earth, vcAdd, vcNext);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_NAKS, nData, ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.shour += 0.2;
			if (vcAdd.shour >= 1.0)
			{
				vcAdd.shour -= 1.0;
				vcAdd.NextDay();
			}
		}
	}

	if (GCDisplaySettings::getValue(COREEVENTS_SANKRANTI))
	{
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			vcNext = GCSankranti::GetNextSankranti(vcAdd, nData);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_SANK, nData, ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.NextDay();
		}
	}

	if (GCDisplaySettings::getValue(COREEVENTS_MOONRASI))
	{
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			nData = MOONDATA::GetNextMoonRasi(earth, vcAdd, vcNext);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_M_RASI, nData, ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.shour += 0.5;
			vcAdd.NormalizeValues();
		}

	}
	if (GCDisplaySettings::getValue(COREEVENTS_CONJUNCTION))
	{
		double dlong;
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			dlong = GCConjunction::GetNextConjunction(vcAdd, vcNext, true, earth);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_CONJ, GCRasi::GetRasi(dlong, GCAyanamsha::GetAyanamsa(vcNext.GetJulianComplete())), ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.NextDay();
		}
	}

	if (GCDisplaySettings::getValue(COREEVENTS_MOON))
	{
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			vcNext = MOONDATA::GetNextRise(earth, vcAdd, true);
			inEvents.AddEvent(vcNext, CCTYPE_M_RISE, 0, ndst);

			vcNext = MOONDATA::GetNextRise(earth, vcNext, false);
			inEvents.AddEvent(vcNext, CCTYPE_M_SET, 0, ndst);

			vcNext.shour += 0.05;
			vcNext.NormalizeValues();
			vcAdd = vcNext;
		}
	}

	if (GCDisplaySettings::getValue(COREEVENTS_ASCENDENT))
	{/*
		vcAdd = vc;
		vcAdd.shour = 0.0;
		while(vcAdd.IsBeforeThis(vcEnd))
		{
			nData = earth.GetNextAscendentStart(vcAdd, vcNext);
			if (vcNext.GetDayInteger() < vcEnd.GetDayInteger())
			{
				vcNext.InitWeekDay();
				ndst = TTimeZone::determineDaylightChange(vcNext, loc.m_nDST);
				inEvents.AddEvent(vcNext, CCTYPE_ASCENDENT, nData, ndst);
			}
			else
			{
				break;
			}
			vcAdd = vcNext;
			vcAdd.shour += 1/24.0;
			if (vcAdd.shour >= 1.0)
			{
				vcAdd.shour -= 1.0;
				vcAdd.NextDay();
			}
		}

		*/

	}

	if (GCDisplaySettings::getValue(COREEVENTS_SORT))
		inEvents.Sort(true);
	else
		inEvents.b_sorted = false;
}

int TResultEvents::formatText(TString &res)
{

	unsigned int i;
	TString str, temp;

	TResultEvents &inEvents = *this;

	res.Format("Events from %d %s %d to %d %s %d.\r\n\r\n", 
		inEvents.m_vcStart.day,
		GCStrings::GetMonthAbreviation(inEvents.m_vcStart.month),
		inEvents.m_vcStart.year,
		inEvents.m_vcEnd.day,
		GCStrings::GetMonthAbreviation(inEvents.m_vcEnd.month),
		inEvents.m_vcEnd.year);

	res += inEvents.m_location.m_strFullName;
	res += "\r\n\r\n";

	VCTIME prevd;
	int prevt = -1;

	prevd.day = 0;
	prevd.month = 0;
	prevd.year = 0;
	for(i = 0; i < inEvents.n_count; i++)
	{
		TDayEvent & dnr = inEvents[i];

		if (inEvents.b_sorted)
		{
			if (prevd.day != dnr.Time.day || prevd.month != dnr.Time.month || prevd.year != dnr.Time.year)
			{
				str.Format("\r\n ===========  %d %s %d  - %s ====================================\r\n\r\n", dnr.Time.day, GCStrings::GetMonthAbreviation(dnr.Time.month), dnr.Time.year,
					GCStrings::GetDayOfWeek(dnr.Time.dayOfWeek));
				res += str;
			}
			prevd = dnr.Time;
		}
		else
		{
			if (prevt != dnr.nType)
			{
				switch(dnr.nType)
				{
				case CCTYPE_S_RISE:
				case CCTYPE_S_SET:
				case CCTYPE_S_NOON:
				case CCTYPE_S_ARUN:
					if (prevt < 10 || prevt >= 14)
					{
						res += "\r\n ========== SUNRISE, SUNSET ==========================================\r\n\r\n";
					}
					break;
				case CCTYPE_M_RISE:
				case CCTYPE_M_SET:
					if (prevt < 40 || prevt >= 42)
					{
						res += "\r\n ========== MOONRISE, MOONSET ==========================================\r\n\r\n";
					}
					break;
				case CCTYPE_TITHI:
					res += "\r\n ========== TITHIS ===================================================\r\n\r\n";
					break;
				case CCTYPE_NAKS:
					res += "\r\n ========== NAKSATRAS ================================================\r\n\r\n";
					break;
				case CCTYPE_SANK:
					res += "\r\n ========== SANKRANTIS ===============================================\r\n\r\n";
					break;
				case CCTYPE_ASCENDENT:
					res += "\r\n ========== ASCENDENTS ===============================================\r\n\r\n";
					break;
				case CCTYPE_CONJ:
					res += "\r\n ========== SUN-MOON CONJUNCTIONS ====================================\r\n\r\n";
					break;
				case CCTYPE_KALA_START:
				case CCTYPE_KALA_END:
					res += "\r\n =========== KALAS ===================================================\r\n\r\n";
					break;
				}
			}
			prevt = dnr.nType;
		}

		switch(dnr.nType)
		{
		case 10:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   arunodaya\r\n";
			break;
		case 11:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   sunrise\r\n";
			break;
		case 12:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   noon\r\n";
			break;
		case 13:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   sunset\r\n";
			break;
		case 20:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetTithiName(dnr.nData);
			res += " Tithi starts";
			res += "\r\n";
			break;
		case 21:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetNaksatraName(dnr.nData);
			res += " Naksatra starts";
			res += "\r\n";
			break;
		case 22:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   Sun enters ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ";
			res += "\r\n";
			break;
		case 23:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   conjunction in ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " rasi";
			res += "\r\n";
			break;
		case CCTYPE_KALA_START:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetKalaName((KalaType)dnr.nData);
			res += " starts";
			res += "\r\n";
			break;
		case CCTYPE_KALA_END:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetKalaName((KalaType)dnr.nData);
			res += " ends";
			res += "\r\n";
			break;
		case CCTYPE_M_RISE:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   moonrise\r\n";
			break;
		case CCTYPE_M_SET:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   moonset\r\n";
			break;
		case CCTYPE_ASCENDENT:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ascendent";
			res += "\r\n";
			break;
		case CCTYPE_M_RASI:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   Moon enters ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ";
			res += "\r\n";
			break;
		default:
			break;
		}
	}

	res += "\r\n";

	return 1;
}

int TResultEvents::formatXml(TString &strXml)
{
	TResultEvents &inEvents = *this;
	unsigned int i;
	TString str, temp;

	strXml.Format("<xml>\r\n<program version=\"%s\">\r\n<location longitude=\"%f\" latitude=\"%f\" timezone=\"%f\" dst=\"%s\" />\n"
		, GCStrings::getString(130).c_str(), inEvents.m_location.m_fLongitude, inEvents.m_location.m_fLatitude
		, inEvents.m_location.m_fTimezone, TTimeZone::GetTimeZoneName(inEvents.m_location.m_nDST));
	VCTIME prevd;
	int prevt = -1;

	prevd.day = 0;
	prevd.month = 0;
	prevd.year = 0;
	for(i = 0; i < inEvents.n_count; i++)
	{
		TDayEvent & dnr = inEvents[i];

		if (inEvents.b_sorted)
		{
			if (prevd.day != dnr.Time.day || prevd.month != dnr.Time.month || prevd.year != dnr.Time.year)
			{
				str.Format("\t<day date=\"%d/%d/%d\" />\n", dnr.Time.day, dnr.Time.month, dnr.Time.year);
				strXml += str;
			}
			prevd = dnr.Time;
		}

		switch(dnr.nType)
		{
		case 10:
			str.Format("\t<event type=\"arunodaya\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 11:
			str.Format("\t<event type=\"sunrise\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 12:
			str.Format("\t<event type=\"noon\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 13:
			str.Format("\t<event type=\"sunset\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 20:
			str.Format("\t<event type=\"%s Tithi\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", GCStrings::GetTithiName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 21:
			str.Format("\t<event type=\"%s Naksatra\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", GCStrings::GetNaksatraName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 22:
			str.Format("\t<event type=\"%s Sankranti\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", GCStrings::GetSankrantiName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		case 23:
			str.Format("\t<event type=\"conjunction in %s rasi\" time=\"%02d:%02d:%02d\" dst=\"%d\" />\n", GCStrings::GetSankrantiName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond(), dnr.nDst);
			strXml += str;
			break;
		default:
			break;
		}
	}

	strXml += "</xml>\n";

	return 1;
}

int TResultEvents::formatRtf(TString &res)
{
	unsigned int i;
	TString str, temp;
	TResultEvents &inEvents = *this;
	str = "";

	GCStringBuilder sb;

	sb.Target = &res;
	sb.Format = SBTF_RTF;

	sb.AppendDocumentHeader();

	sb.AppendHeader1("Events");

	str.Format("\\par from %d %s %d to %d %s %d.\\par \r\n\r\n", 
		inEvents.m_vcStart.day,
		GCStrings::GetMonthAbreviation(inEvents.m_vcStart.month),
		inEvents.m_vcStart.year,
		inEvents.m_vcEnd.day,
		GCStrings::GetMonthAbreviation(inEvents.m_vcEnd.month),
		inEvents.m_vcEnd.year);

	str += inEvents.m_location.m_strFullName;
	str += "\\par\r\n\\par\r\n";

	res += str;

	VCTIME prevd;
	int prevt = -1;

	prevd.day = 0;
	prevd.month = 0;
	prevd.year = 0;
	for(i = 0; i < inEvents.n_count; i++)
	{
		TDayEvent & dnr = inEvents[i];

		if (inEvents.b_sorted)
		{
			if (prevd.day != dnr.Time.day || prevd.month != dnr.Time.month || prevd.year != dnr.Time.year)
			{
				str.Format("\\par\r\n ===========  %d %s %d  == %s ===================================\\par\\par\r\n\r\n", dnr.Time.day, GCStrings::GetMonthAbreviation(dnr.Time.month), dnr.Time.year,
					GCStrings::GetDayOfWeek(dnr.Time.dayOfWeek));
				res += str;
			}
			prevd = dnr.Time;
		}
		else
		{
			if (prevt != dnr.nType)
			{
				switch(dnr.nType)
				{
				case 10:
				case 11:
				case 12:
				case 13:
					if (prevt < 10 || prevt >= 14)
					{
						res += "\\par\r\n ========== SUNRISE, SUNSET ==========================================\\par\r\n\\par\r\n";
					}
					break;
				case 20:
					res += "\\par\r\n ========== TITHIS ===================================================\\par\r\n\\par\r\n";
					break;
				case 21:
					res += "\\par\r\n ========== NAKSATRAS ================================================\\par\r\n\\par\r\n";
					break;
				case 22:
					res += "\\par\r\n ========== SANKRANTIS ===============================================\\par\r\n\\par\r\n";
					break;
				case 23:
					res += "\\par\r\n ========== SUN-MOON CONJUNCTIONS ====================================\\par\r\n\\par\r\n";
					break;
				}
			}
			prevt = dnr.nType;
		}

		switch(dnr.nType)
		{
		case CCTYPE_S_ARUN:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   arunodaya\\par\r\n";
			break;
		case CCTYPE_S_RISE:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   sunrise\\par\r\n";
			break;
		case CCTYPE_S_NOON:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   noon\\par\r\n";
			break;
		case CCTYPE_S_SET:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   sunset\\par\r\n";
			break;
		case CCTYPE_TITHI:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetTithiName(dnr.nData);
			res += " Tithi starts";
			res += "\\par\r\n";
			break;
		case CCTYPE_NAKS:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetNaksatraName(dnr.nData);
			res += " Naksatra starts";
			res += "\\par\r\n";
			break;
		case CCTYPE_SANK:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   Sun enters ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ";
			res += "\\par\r\n";
			break;
		case CCTYPE_CONJ:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   conjunction in ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " rasi";
			res += "\\par\r\n";
			break;
		case CCTYPE_KALA_START:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetKalaName((KalaType)dnr.nData);
			res += " starts";
			res += "\\par\r\n";
			break;
		case CCTYPE_KALA_END:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetKalaName((KalaType)dnr.nData);
			res += " ends";
			res += "\\par\r\n";
			break;
		case CCTYPE_M_RISE:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   moonrise\r\n";
			res += "\\par\r\n";
			break;
		case CCTYPE_M_SET:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   moonset\r\n";
			res += "\\par\r\n";
			break;
		case CCTYPE_ASCENDENT:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ascendent";
			res += "\\par\r\n";
			break;
		case CCTYPE_M_RASI:
			res += "            ";
			res += dnr.Time.c_str();
			res += " ";
			res += GCStrings::GetDSTSignature(dnr.nDst);
			res += "   Moon enters ";
			res += GCStrings::GetSankrantiName(dnr.nData);
			res += " ";
			res += "\\par\r\n";
			break;
		default:
			break;
		}
	}

	res += "\\par\r\n";

	sb.AppendNote();
	sb.AppendDocumentTail();


	return 1;
}
