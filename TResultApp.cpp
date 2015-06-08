#include "stdafx.h"
#include "TResultApp.h"
#include "GCStrings.h"
#include "TTimeZone.h"
#include "GCAyanamsha.h"
#include "GCRasi.h"
#include "GCCalendar.h"
#include "GCDisplaySettings.h"
#include "GCStringBuilder.h"
#include "GCLayoutData.h"


void TResultApp::calculateAppDay(CLocationRef &location, VCTIME eventDate)
{
	//MOONDATA moon;
	//SUNDATA sun;
	DAYDATA &d = this->details;
	double dd;
	TString str;
	VCTIME vc = eventDate;
	VCTIME vcsun = eventDate;
	VCTIME dprev, dnext;
	EARTHDATA m_earth = (EARTHDATA)location;

	this->b_adhika = false;
	this->event = eventDate;
	this->location = location;
	
	//d.nTithi = GetPrevTithiStart(m_earth, vc, dprev);
	//GetNextTithiStart(m_earth, vc, dnext);
	vcsun.shour -= vcsun.tzone/24.0;
	vcsun.NormalizeValues();
	vcsun.tzone = 0.0;
	d.sun.SunPosition(vcsun, m_earth, vcsun.shour - 0.5);
	d.moon.Calculate(vcsun.GetJulianComplete(), m_earth);
	d.msDistance = put_in_360( d.moon.longitude_deg - d.sun.longitude_deg - 180.0);
	d.msAyanamsa = GCAyanamsha::GetAyanamsa( vc.GetJulianComplete() );

	// tithi
	dd = d.msDistance / 12.0;
	d.nTithi = int(floor(dd));
	d.nTithiElapse = (dd - floor(dd)) * 100.0;
	d.nPaksa = (d.nTithi >= 15) ? 1 : 0;


	// naksatra
	dd = put_in_360( d.moon.longitude_deg - d.msAyanamsa );
	dd = ( dd * 3.0) / 40.0;
	d.nNaksatra = int(floor(dd));
	d.nNaksatraElapse = (dd - floor(dd)) * 100.0;
	d.nMasa = d.MasaCalc(vc, m_earth);
	d.nMoonRasi = GCRasi::GetRasi(d.moon.longitude_deg, d.msAyanamsa);
	d.nSunRasi = GCRasi::GetRasi(d.sun.longitude_deg, d.msAyanamsa);

	if (d.nMasa == ADHIKA_MASA)
	{
		d.nMasa = GCRasi::GetRasi(d.sun.longitude_deg, d.msAyanamsa);
		this->b_adhika = true;
	}

	vc.Today();
	vc.tzone = m_earth.tzone;
	int m = 0;
	VATIME va;
	VCTIME vctemp;

	va.tithi = d.nTithi;
	va.masa  = d.nMasa;
	va.gyear = GCCalendar::GetGaurabdaYear(vc, m_earth);
	if (va.gyear < d.nGaurabdaYear)
		va.gyear = d.nGaurabdaYear;


	for(int i = 0; i < 6; i++)
	{
		GCCalendar::VATIMEtoVCTIME(va, vctemp, m_earth);
		if (va.gyear > d.nGaurabdaYear)
		{
			if (m < TRESULT_APP_CELEBS)
			{
				this->celeb_date[m] = vctemp;
				this->celeb_gy[m] = va.gyear;
				m++;
			}
		}
		va.gyear++;
	}
}

void TResultApp::formatPlainText(TString &strResult)
{
	//MOONDATA moon;
	//SUNDATA sun;
	TResultApp & app = *this;
	DAYDATA &d = app.details;
	TString str;
	VCTIME vc = app.event;
	EARTHDATA m_earth = (EARTHDATA)app.location;
	TString & strText = strResult;
//	int nHour, nMin;

	GCStringBuilder sb;
	sb.Format = SBTF_TEXT;
	sb.Target = &strResult;
	
//	nHour = int(vc.shour * 24);
//	nMin = int((vc.shour*24 - nHour)*60);

	sb.AppendLine(GCStrings::getString(25));
	sb.AppendLine("");

	str.Format("%15s : %d %s %d", GCStrings::getString(7).c_str(), vc.day, GCStrings::GetMonthAbreviation(vc.month), vc.year);
	sb.AppendLine(str);

	str.Format("%15s : %d:%02d", GCStrings::getString(8).c_str(), vc.GetHour(), vc.GetMinuteRound());
	sb.AppendLine(str);
	sb.AppendLine("");

	str.Format("%15s : %s", GCStrings::getString(9).c_str(), app.location.m_strName);
	sb.AppendLine(str);
	str.Format("%15s : %s", GCStrings::getString(10).c_str(), app.location.m_strLatitude);
	sb.AppendLine(str);
	str.Format("%15s : %s", GCStrings::getString(11).c_str(), app.location.m_strLongitude);
	sb.AppendLine(str);
	str.Format("%15s : ", GCStrings::getString(12).c_str());
	str += app.location.m_strTimeZone;
	str += "  ";
	str += TTimeZone::GetTimeZoneName(app.location.m_nDST);
	sb.AppendLine(str);
	str.Format("%15s : N/A", "DST");
	sb.AppendLine(str);
	sb.AppendLine("");

	str.Format("%15s : %s", GCStrings::getString(13).c_str(), GCStrings::GetTithiName(d.nTithi));
	sb.AppendLine(str);
	str.Format("%15s : %.2f %%", GCStrings::getString(14).c_str(), d.nTithiElapse);
	sb.AppendLine(str);
	str.Format("%15s : %s", GCStrings::getString(15).c_str(), GCStrings::GetNaksatraName(d.nNaksatra));
	sb.AppendLine(str);
	str.Format("%15s : %.2f %% (%s pada)", GCStrings::getString(16).c_str(), d.nNaksatraElapse,
		GCStrings::getString(811+int(d.nNaksatraElapse/25.0)).c_str());
	sb.AppendLine(str);
	str.Format("%15s : %s (%s)", "Moon Rasi", GCStrings::GetSankrantiName(d.nMoonRasi), GCStrings::GetSankrantiNameEn(d.nMoonRasi));
	sb.AppendLine(str);
	str.Format("%15s : %s (%s)", "Sun Rasi", GCStrings::GetSankrantiName(d.nSunRasi), GCStrings::GetSankrantiNameEn(d.nSunRasi));
	sb.AppendLine(str);

	str.Format("%15s : %s", GCStrings::getString(20).c_str(), GCStrings::GetPaksaName(d.nPaksa));
	sb.AppendLine(str);
	if (app.b_adhika == true)
	{
		str.Format("%15s : %s %s", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa), GCStrings::getString(21).c_str());
	}
	else
		str.Format("%15s : %s", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa));
	sb.AppendLine(str);
	str.Format("%15s : %d", GCStrings::getString(23).c_str(), d.nGaurabdaYear);
	sb.AppendLine(str);

	if (GCDisplaySettings::getValue(48))
	{
		sb.AppendLine();
		sb.AppendLine(GCStrings::getString(17).c_str());
		sb.AppendLine();

		str.Format("%25s : %s... ", GCStrings::getString(18).c_str() , GCStrings::GetNaksatraChildSylable(d.nNaksatra, int(d.nNaksatraElapse/25.0)));
		sb.AppendLine(str);
		str.Format("%25s : %s... ", GCStrings::getString(19).c_str(), GCStrings::GetRasiChildSylable(d.nMoonRasi));
		sb.AppendLine(str);
	}

	sb.AppendLine();
	sb.AppendLine(GCStrings::getString(24).c_str());
	sb.AppendLine();

	for(int o = 0; o < TRESULT_APP_CELEBS; o++)
	{
		str.Format("   Gaurabda %3d : %d %s %d", app.celeb_gy[o] , app.celeb_date[o].day,
			GCStrings::GetMonthAbreviation(app.celeb_date[o].month), 
			app.celeb_date[o].year);
		sb.AppendLine(str);
	}

}

void TResultApp::formatRtf(TString &strResult)
{
	TResultApp & app = *this;
	//MOONDATA moon;
	//SUNDATA sun;
	DAYDATA &d = app.details;
	TString str;
	VCTIME vc = app.event;
	EARTHDATA m_earth = (EARTHDATA)app.location;
	TString & strText = strResult;
//	int nHour, nMin;
	
	GCStringBuilder sb;

	sb.fontSizeH1 = GCLayoutData::textSizeH1;
	sb.fontSizeH2 = GCLayoutData::textSizeH2;
	sb.fontSizeText = GCLayoutData::textSizeText;
	sb.Format = SBTF_RTF;
	sb.Target = &strText;

//	nHour = int(vc.shour * 24);
//	nMin = int((vc.shour*24 - nHour)*60);

	strText = "";
	sb.AppendDocumentHeader();

	sb.AppendHeader1(GCStrings::getString(25));


	str.Format("\\tab %s : {\\b %d %s %d}", GCStrings::getString(7).c_str(), vc.day, GCStrings::GetMonthAbreviation(vc.month), vc.year);
	sb.AppendLine(str);

	str.Format("\\tab %s : {\\b %d:%02d}", GCStrings::getString(8).c_str(), vc.GetHour(), vc.GetMinuteRound());
	sb.AppendLine(str);
	sb.AppendLine("");

	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(9).c_str(), app.location.m_strName);
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(10).c_str(), app.location.m_strLatitude);
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(11).c_str(), app.location.m_strLongitude);
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b ", GCStrings::getString(12).c_str());
	str += TTimeZone::GetTimeZoneName(app.location.m_nDST);
	str += "}";
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b N/A}", "DST");
	sb.AppendLine(str);
	sb.AppendLine("");

	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(13).c_str(), GCStrings::GetTithiName(d.nTithi));
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %.2f %%}", GCStrings::getString(14).c_str(), d.nTithiElapse);
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(15).c_str(), GCStrings::GetNaksatraName(d.nNaksatra));
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %.2f %% (%s pada)}", GCStrings::getString(16).c_str(), d.nNaksatraElapse,
		GCStrings::getString(811+int(d.nNaksatraElapse/25.0)).c_str());
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %s (%s)}", "Moon Rasi", GCStrings::GetSankrantiName(d.nMoonRasi), GCStrings::GetSankrantiNameEn(d.nMoonRasi));
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %s (%s)}", "Sun Rasi", GCStrings::GetSankrantiName(d.nSunRasi), GCStrings::GetSankrantiNameEn(d.nSunRasi));
	sb.AppendLine(str);

	str.Format("\\tab %s : {\\b %s}", GCStrings::getString(20).c_str(), GCStrings::GetPaksaName(d.nPaksa));
	sb.AppendLine(str);
	if (app.b_adhika == true)
	{
		str.Format("\\tab %s : {\\b %s %s}", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa), GCStrings::getString(21).c_str());
	}
	else
		str.Format("\\tab %s : {\\b %s}", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa));
	sb.AppendLine(str);
	str.Format("\\tab %s : {\\b %d}", GCStrings::getString(23).c_str(), d.nGaurabdaYear);
	sb.AppendLine(str);

	if (GCDisplaySettings::getValue(48))
	{
		sb.AppendLine("");
		sb.AppendHeader2(GCStrings::getString(17).c_str());
		sb.AppendLine("");
		str.Format("\\tab %s : {\\b %s... }", GCStrings::getString(18).c_str() , GCStrings::GetNaksatraChildSylable(d.nNaksatra, int(d.nNaksatraElapse/25.0)));
		sb.AppendLine(str);
		str.Format("\\tab %s : {\\b %s... }", GCStrings::getString(19).c_str(), GCStrings::GetRasiChildSylable(d.nMoonRasi));
		sb.AppendLine(str);
	}

	sb.AppendLine("");
	str.Format("{\\fs%d %s}", GCLayoutData::textSizeH2, GCStrings::getString(24).c_str());
	sb.AppendLine(str);
	sb.AppendLine("");

	for(int o = 0; o < TRESULT_APP_CELEBS; o++)
	{
		str.Format("\\tab Gaurabda %3d : {\\b %d %s %d}", app.celeb_gy[o] , app.celeb_date[o].day,
			GCStrings::GetMonthAbreviation(app.celeb_date[o].month), 
			app.celeb_date[o].year);
		sb.AppendLine(str);
	}

	sb.AppendDocumentTail();
}

void TResultApp::formatXml(TString &strResult)
{
	TResultApp & app = *this;
	DAYDATA &d = this->details;
	TString str;
	VCTIME vc = this->event;
	EARTHDATA m_earth = (EARTHDATA)this->location;
	CLocationRef & loc = this->location;
	TString & strText = strResult;
	int npada;
	BOOL bDuringAdhika = FALSE;

	strText.Format(
		"<xml>\n"
		"\t<request name=\"AppDay\" version=\"%s\">\n"
		"\t\t<arg name=\"longitude\" value=\"%+f\" />\n"
		"\t\t<arg name=\"latitude\" value=\"%+f\" />\n"
		"\t\t<arg name=\"timezone\" value=\"%+f\" />\n"
		"\t\t<arg name=\"year\" value=\"%d\" />\n"
		"\t\t<arg name=\"month\" value=\"%d\" />\n"
		"\t\t<arg name=\"day\" value=\"%d\" />\n"
		"\t\t<arg name=\"hour\" value=\"%d\" />\n"
		"\t\t<arg name=\"minute\" value=\"%d\" />\n"
		"\t</request>\n", GCStrings::getString(130).c_str(),
		loc.m_fLongitude, loc.m_fLatitude, loc.m_fTimezone,
		app.event.year, app.event.month, app.event.day, app.event.GetHour(), app.event.GetMinuteRound()
		);


	npada = int (d.nNaksatraElapse / 25.0) + 1;
	if (npada > 4)
		npada = 4;

	str.Format("\t<result name=\"AppDay\" >\n"
		"\t\t<tithi name=\"%s\" elapse=\"%f\" />\n"
		"\t\t<naksatra name=\"%s\" elapse=\"%f\" pada=\"%d\"/>\n"
		"\t\t<paksa name=\"%s\" />\n"
		"\t\t<masa name=\"%s\" adhikamasa=\"%s\"/>\n"
		"\t\t<gaurabda value=\"%d\" />\n"

		, GCStrings::GetTithiName(d.nTithi), d.nTithiElapse
		, GCStrings::GetNaksatraName(d.nNaksatra), d.nNaksatraElapse, npada
		, GCStrings::GetPaksaName(d.nPaksa)
		, GCStrings::GetMasaName(d.nMasa), (bDuringAdhika ? "yes" : "no")
		, d.nGaurabdaYear
		);

	strText += str;

	strText += "\t\t<celebrations>\n";
	for(int i =0; i < TRESULT_APP_CELEBS; i++)
	{
		str.Format("\t\t\t<celebration gaurabda=\"%d\" day=\"%d\" month=\"%d\" monthabr=\"%s\" year=\"%d\" />\n"
			, app.celeb_gy[i], app.celeb_date[i].day, app.celeb_date[i].month, GCStrings::GetMonthAbreviation(app.celeb_date[i].month), app.celeb_date[i].year);
		strText += str;
	}

	strText += "\t\t</celebrations>\n\t</result>\n</xml>\n";
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

void TResultApp::writeHtml(FILE *F)
{
	DAYDATA &d = this->details;
	TString str;
	VCTIME vc = this->event;
	EARTHDATA m_earth = (EARTHDATA)this->location;

	fprintf(F, "<html><head><title>Appearance day</title>");
	fprintf(F, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:11pt;\n}\n\ntd.hed {\n  font-size:11pt;\n  font-weight:bold;\n");
	fprintf(F, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(F, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(F, "</head>\n\n<body>\n");
	fprintf(F, "<h2 align=center>Appearance day Calculation</h2>");
	fprintf(F, "<table align=center><tr><td valign=top>\n\n");
	fprintf(F, "<table align=center>");
	fprintf(F, "<tr><td colspan=3 class=hed>Details</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d %s %d</td></tr>\n", GCStrings::getString(7).c_str(), vc.day, GCStrings::GetMonthAbreviation(vc.month), vc.year);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d:%02d</td></tr>\n\n", GCStrings::getString(8).c_str(), vc.GetHour(), vc.GetMinuteRound());
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(9).c_str(), this->location.m_strName.c_str());
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(10).c_str(), EARTHDATA::GetTextLatitude(this->location.m_fLatitude));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(11).c_str(), EARTHDATA::GetTextLongitude(this->location.m_fLongitude));
	fprintf(F, "<tr><td colspan=2>%s</td><td> ", GCStrings::getString(12).c_str());
	fprintf(F, TTimeZone::GetTimeZoneOffsetText(this->location.m_fTimezone));
	fprintf(F, "</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>DST</td><td>N/A</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(13).c_str(), GCStrings::GetTithiName(d.nTithi));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %.2f %%</td></tr>\n", GCStrings::getString(14).c_str(), d.nTithiElapse);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(15).c_str(), GCStrings::GetNaksatraName(d.nNaksatra));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %.2f %%</td></tr>\n", GCStrings::getString(16).c_str(), d.nNaksatraElapse);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(20).c_str(), GCStrings::GetPaksaName(d.nPaksa));
	if (this->b_adhika == true)
	{
		fprintf(F, "<tr><td colspan=2>%s</td><td> %s %s</td></tr>\n", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa), GCStrings::getString(21).c_str());
	}
	else
		fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", GCStrings::getString(22).c_str(), GCStrings::GetMasaName(d.nMasa));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d</td></tr>\n\n", GCStrings::getString(23).c_str(), d.nGaurabdaYear);

	fprintf(F, "</table></td><td valign=top><table>");
	fprintf(F, "<tr><td colspan=3 class=hed>%s</td></tr>\n", GCStrings::getString(24).c_str());

	//fprintf(F, "<table align=center>");
	for(int o = 0; o < TRESULT_APP_CELEBS; o++)
	{
		fprintf(F, "<tr><td>Gaurabda %3d</td><td>&nbsp;&nbsp;:&nbsp;&nbsp;</td><td><b>%d %s %d</b></td></tr>", this->celeb_gy[o] , this->celeb_date[o].day,
			GCStrings::GetMonthAbreviation(this->celeb_date[o].month), 
			this->celeb_date[o].year);
	}
	fprintf(F, "</table>");
	fprintf(F, "</td></tr></table>\n\n");
	fprintf(F, "<hr align=center width=\"50%%\">\n<p style=\'text-align:center;font-size:8pt\'>Generated by %s</p>", GCStrings::getString(130).c_str());
	fprintf(F, "</body></html>");

}
