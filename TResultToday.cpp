#include "stdafx.h"
#include "TResultToday.h"
#include "GCStrings.h"
#include "GCStringBuilder.h"
#include "GCDisplaySettings.h"
#include "TTimeZone.h"
#include "GCLayoutData.h"

TResultToday::TResultToday(void)
{
	calendar = new TResultCalendar();
}


TResultToday::~TResultToday(void)
{
	if (calendar != NULL)
	{
		delete calendar;
		calendar = NULL;
	}
}


void TResultToday::Calculate(VCTIME dateTime, CLocationRef& location)
{
	VCTIME vc2;
	currentDay.Set(dateTime);
	currentDay.InitWeekDay();
	vc2.Set(currentDay);

	vc2.tzone = location.m_fTimezone;
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	calendar->CalculateCalendar(location, vc2, 9);


}

VAISNAVADAY * TResultToday::GetCurrentDay(void)
{
	int i = calendar->FindDate(currentDay);
	return calendar->GetDay(i);
}

void TResultToday::formatPlain(TString &str)
{
	TString str2, str3, str4;

	int k = 0;

	VAISNAVADAY * p = GetCurrentDay();
	CLocationRef &loc = calendar->m_Location;
	VCTIME &vc = p->date;

	GCStringBuilder sb;

	sb.Format = SBTF_TEXT;
	sb.Target = &str;

	if (p == NULL)
		return;

	str2.Format("%s (%s, %s, Timezone: %s)\r\n\r\n[%d %s %d - %s]\r\n  %s, %s %s\r\n  %s %s, %d Gaurabda\r\n\r\n",
		loc.m_strName.c_str(), EARTHDATA::GetTextLatitude(loc.m_fLatitude), EARTHDATA::GetTextLongitude(loc.m_fLongitude),
		TTimeZone::GetTimeZoneName(loc.m_nDST),
		vc.day, GCStrings::GetMonthAbreviation(vc.month), vc.year, GCStrings::getString(vc.dayOfWeek).c_str(),
		GCStrings::GetTithiName(p->astrodata.nTithi), GCStrings::GetPaksaName(p->astrodata.nPaksa), GCStrings::getString(20).c_str(), 
		GCStrings::GetMasaName(p->astrodata.nMasa), GCStrings::getString(22).c_str(), p->astrodata.nGaurabdaYear);

	str = str2;

	for(int i = 0; i < p->dayEvents.Count(); i++)
	{
		GCMutableDictionary * ed = p->dayEvents.ObjectAtIndex(i);
		int disp = ed->intForKey("disp");
		if (!ed->containsKey("disp") || disp == -1 || GCDisplaySettings::getValue(disp))
		{
			if (ed->containsKey("spec"))
			{
				str2 = ed->stringForKey("text");
				int length = str2.GetLength();
				length = (80 - length) / 2;
				sb.AppendSeparatorWithWidth(length);
				str += str2;
				sb.AppendSeparatorWithWidth(length);
				str += "\r\n";
			}
			else
			{
				str += ed->stringForKey("text");
				str += "\r\n";
			}
		}
	}

	str += "\r\n";


	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GCDisplaySettings::getValue(45))
	{
		tdA.Set(p->astrodata.sun.rise);
		tdB.Set(p->astrodata.sun.rise);
		tdA.AddMinutes(-96);
		tdB.AddMinutes(-48);
		str2.Format("\r\nBrahma Muhurta %2d:%02d - %2d:%02d (%s)", 
			tdA.hour, tdA.min, tdB.hour, tdB.min, GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(29))
	{
		str2.Format("\r\n%s %2d:%02d ",
			GCStrings::getString(51).c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.rise);
			tdB.Set(p->astrodata.sun.rise);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(30))
	{
		str2.Format("%s    %2d:%02d ", GCStrings::getString(857).c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.noon);
			tdB.Set(p->astrodata.sun.noon);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(31))
	{
		str2.Format("%s  %2d:%02d ", GCStrings::getString(52).c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.set);
			tdB.Set(p->astrodata.sun.set);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(33))
	{
		str2.Format("\r\n%s info\r\n   Moon in %s %s",
			GCStrings::getString(51).c_str(), 
			GCStrings::GetNaksatraName(p->astrodata.nNaksatra), GCStrings::getString(15).c_str());
		str += str2;
		if (GCDisplaySettings::getValue(47))
		{
			str2.Format(", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, GCStrings::getString(811+int(p->astrodata.nNaksatraElapse/25)).c_str());
			str += str2;
		}
		if (GCDisplaySettings::getValue(46))
		{
			str2.Format(", Moon in %s %s", GCStrings::GetSankrantiName(p->astrodata.nMoonRasi), GCStrings::getString(105).c_str());
			str += str2;
		}
		str2.Format(", %s %s\r\n   Sun in %s %s.\r\n",
			GCStrings::GetYogaName(p->astrodata.nYoga), GCStrings::getString(104).c_str(),
			GCStrings::GetSankrantiName(p->astrodata.nSunRasi), GCStrings::getString(105).c_str());
		str += str2;
	}


	sb.AppendNote();
	sb.AppendDocumentTail();


}

void TResultToday::formatRtf(TString &str)
{
	TString str2, str3, str4;
	TString str5;

	int k = 0;

	VAISNAVADAY * p = GetCurrentDay();
	CLocationRef &loc = calendar->m_Location;
	VCTIME &vc = p->date;

	GCStringBuilder sb;

	sb.Format = SBTF_RTF;
	sb.fontSizeH1 = GCLayoutData::textSizeH1;
	sb.fontSizeH2 = GCLayoutData::textSizeH2;
	sb.fontSizeText = GCLayoutData::textSizeText;
	sb.fontSizeNote = GCLayoutData::textSizeNote;
	sb.Target = &str;

	str = "";

	sb.AppendDocumentHeader();

	VCTIME::GetDateTextWithTodayExt(str3, vc);
	str2.Format("\\f2\\fs%d %s ", GCLayoutData::textSizeH1, str3.c_str());
	str += str2;

	str2.Format("\\par\\f2\\fs%d {\\fs%d %s}\\line %s (%s, %s, Timezone: %s)\\par\r\n\\par\r\n  %s, %s %s\\par\r\n  %s %s, %d Gaurabda\\par\r\n\\par\r\n",
		GCLayoutData::textSizeText, GCLayoutData::textSizeText+4, GCStrings::getString(p->date.dayOfWeek).c_str(),
		loc.m_strName.c_str(), EARTHDATA::GetTextLatitude(loc.m_fLatitude), EARTHDATA::GetTextLongitude(loc.m_fLongitude),
		TTimeZone::GetTimeZoneName(loc.m_nDST),
		GCStrings::GetTithiName(p->astrodata.nTithi), GCStrings::GetPaksaName(p->astrodata.nPaksa), GCStrings::getString(20).c_str(), 
		GCStrings::GetMasaName(p->astrodata.nMasa), GCStrings::getString(22).c_str(), p->astrodata.nGaurabdaYear);

	str += str2;

	for(int i = 0; i < p->dayEvents.Count(); i++)
	{
		GCMutableDictionary * ed = p->dayEvents.ObjectAtIndex(i);
		int disp = ed->intForKey("disp");
		if (!ed->containsKey("disp") || disp == -1 || GCDisplaySettings::getValue(disp))
		{
			if (ed->containsKey("spec"))
			{
				str2 = ed->stringForKey("text");
				int length = str2.GetLength();
				length = (80 - length) / 2;
				sb.AppendSeparatorWithWidth(length);
				str += str2;
				sb.AppendSeparatorWithWidth(length);
				str += "\\par\r\n";
			}
			else
			{
				str += "\\tab ";
				str += ed->stringForKey("text");
				str += "\\par\r\n";
			}
		}
	}

	str += "\\par\r\n";

	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GCDisplaySettings::getValue(45))
	{
		tdA.Set(p->astrodata.sun.rise);
		tdB.Set(p->astrodata.sun.rise);
		tdA.AddMinutes(-96);
		tdB.AddMinutes(-48);
		str2.Format("\\par\r\nBrahma Muhurta %2d:%02d - %2d:%02d (%s)", 
			tdA.hour, tdA.min, tdB.hour, tdB.min, GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}

	if (GCDisplaySettings::getValue(29))
	{
		str2.Format("\\par\r\n%s %2d:%02d ",
			GCStrings::getString(51).c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.rise);
			tdB.Set(p->astrodata.sun.rise);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(30))
	{
		str2.Format("%s    %2d:%02d ", GCStrings::getString(857).c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.noon);
			tdB.Set(p->astrodata.sun.noon);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(31))
	{
		str2.Format("%s  %2d:%02d ", GCStrings::getString(52).c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		str += str2;
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.set);
			tdB.Set(p->astrodata.sun.set);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			str2.Format(" sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
			str += str2;
		}
		str2.Format(" (%s)\\par\r\n", GCStrings::GetDSTSignature(p->nDST));
		str += str2;
	}
	if (GCDisplaySettings::getValue(33))
	{
		str2.Format("\\par\r\n%s info\\par\r\n   Moon in %s %s",
			GCStrings::getString(51).c_str(), 
			GCStrings::GetNaksatraName(p->astrodata.nNaksatra), GCStrings::getString(15).c_str());
		str += str2;
		if (GCDisplaySettings::getValue(47))
		{
			str2.Format(", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, GCStrings::getString(811+int(p->astrodata.nNaksatraElapse/25)).c_str());
			str += str2;
		}
		if (GCDisplaySettings::getValue(46))
		{
			str2.Format(", Moon in %s %s", GCStrings::GetSankrantiName(p->astrodata.nMoonRasi), GCStrings::getString(105).c_str());
			str += str2;
		}
		str2.Format(", %s %s\\par\r\n   Sun in %s %s,\\par\r\n",
			GCStrings::GetYogaName(p->astrodata.nYoga), GCStrings::getString(104).c_str(),
			GCStrings::GetSankrantiName(p->astrodata.nSunRasi), GCStrings::getString(105).c_str());
		str += str2;
	}
	/* END GCAL 1.4.3 */


	str += "\n\n \\par";

	sb.AppendNote();
}



/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/


void TResultToday::writeHtml(FILE * f)
{
	TString str2, str3, str4;

	int k = 0;

	VAISNAVADAY * p = GetCurrentDay();
	CLocationRef &loc = calendar->m_Location;
	VCTIME &vc = p->date;

	if (p == NULL)
		return;

	fprintf(f, "<html>\n<head>\n<title></title>");
	fprintf(f, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:9.5pt;\n}\n\ntd.hed {\n  font-size:9.5pt;\n  font-weight:bold;\n");
	fprintf(f, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(f, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(f, "</head>\n");
	fprintf(f, "<body>\n");
	VCTIME::GetDateTextWithTodayExt(str2, vc);
	fprintf(f, "<h2>%s</h2>\n", str2.c_str());
	fprintf(f, "<h4>%s</h4>\n", loc.m_strFullName.c_str());
	fprintf(f, "<p>  %s, %s %s<br>  %s %s, %d Gaurabda</p>",
		GCStrings::GetTithiName(p->astrodata.nTithi), GCStrings::GetPaksaName(p->astrodata.nPaksa), GCStrings::getString(20).c_str(), 
		GCStrings::GetMasaName(p->astrodata.nMasa), GCStrings::getString(22).c_str(), p->astrodata.nGaurabdaYear);

	int prevCountFest = 0;

	if (p->dayEvents.Count() > 0)
	{
		fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", TResultCalendar::getDayBkgColorCode(p));
		for(int i = 0; i < p->dayEvents.Count(); i++)
		{
			GCMutableDictionary * ed = p->dayEvents.ObjectAtIndex(i);
			int disp = ed->intForKey("disp");
			if (!ed->containsKey("disp") || disp == -1 || GCDisplaySettings::getValue(disp))
			{
				if (ed->containsKey("spec"))
					fprintf(f, "<span style=\'color:#110033\'>");
				fprintf(f, "<br>%s", ed->stringForKey("text"));
				if (ed->containsKey("spec"))
					fprintf(f, "</span>");
			}
		}
		fprintf(f, "</td></tr></table>\n");
	}


	fprintf(f, "<p>");


	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GCDisplaySettings::getValue(29))
	{
		fprintf(f, "<br>%s %2d:%02d ",
			GCStrings::getString(51).c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.rise);
			tdB.Set(p->astrodata.sun.rise);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)", GCStrings::GetDSTSignature(p->nDST));
	}
	if (GCDisplaySettings::getValue(30))
	{
		fprintf(f, "<br>%s    %2d:%02d ", GCStrings::getString(857).c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.noon);
			tdB.Set(p->astrodata.sun.noon);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
	}
	if (GCDisplaySettings::getValue(31))
	{
		fprintf(f, "<br>%s  %2d:%02d ", GCStrings::getString(52).c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		if (GCDisplaySettings::getValue(32))
		{
			tdA.Set(p->astrodata.sun.set);
			tdB.Set(p->astrodata.sun.set);
			tdA.AddMinutes(-24);
			tdB.AddMinutes(+24);
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)\r\n", GCStrings::GetDSTSignature(p->nDST));
	}
	if (GCDisplaySettings::getValue(33))
	{
		fprintf(f,"<br>%s info  Moon in %s %s",
			GCStrings::getString(51).c_str(), 
			GCStrings::GetNaksatraName(p->astrodata.nNaksatra), GCStrings::getString(15).c_str());
		if (GCDisplaySettings::getValue(47))
		{
			fprintf(f, ", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, GCStrings::getString(811+int(p->astrodata.nNaksatraElapse/25)).c_str());
		}
		if (GCDisplaySettings::getValue(46))
		{
			fprintf(f, ", Moon in the %s %s", GCStrings::GetSankrantiName(p->astrodata.nMoonRasi), GCStrings::getString(105).c_str());
		}
		fprintf(f, ", %s %s<br>Sun in %s %s.",
			GCStrings::GetYogaName(p->astrodata.nYoga), GCStrings::getString(104).c_str(),
			GCStrings::GetSankrantiName(p->astrodata.nSunRasi), GCStrings::getString(105).c_str());
	}
	fprintf(f, "</p>");
	fprintf(f, "</body>");
	fprintf(f, "</html>");
	/* END GCAL 1.4.3 */

}
