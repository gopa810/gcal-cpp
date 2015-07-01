// TResultMasaList.cpp: implementation of the TResultMasaList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vcal5beta.h"
#include "TResultMasaList.h"
#include "GCStrings.h"
#include "GCStringBuilder.h"
#include "GCLayoutData.h"
#include "GCDayData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// PORTABLE

TResultMasaList::TResultMasaList()
{
	n_countMasa = 0;
	n_countYears = 0;
	n_startYear = 0;
	n_allocated = 0;
	arr = NULL;
}

TResultMasaList::~TResultMasaList()
{
	if (arr != NULL)
	{
		delete [] arr;
		arr = NULL;
	}
}

bool TResultMasaList::alloc(int nCountYears)
{
	if (n_allocated < nCountYears)
	{
		if (arr != NULL)
			delete [] arr;
		arr = NULL;
	}
	arr = new TResultMasa[nCountYears*14];
	n_allocated = nCountYears;
	return arr ? true : false;
}

int TResultMasaList::CalculateMasaList(CLocationRef &loc, int nYear, int nCount)
{
	DAYDATA day;
	VCTIME d, de, t;
	int lm = -1;
	TResultMasaList &mlist = *this;

	mlist.n_startYear = nYear;
	mlist.n_countYears = nCount;
	mlist.vc_start = d  = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, nYear);
	mlist.vc_end   = de = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, nYear + nCount);
	mlist.m_location = loc;

	if (mlist.alloc(nCount) == false)
		return 0;

	int i = 0;
	int prev_paksa = -1;
	int prev_gyear = -1;
	int current = 0;


	EARTHDATA earth = (EARTHDATA)loc;
	while(d.IsBeforeThis(de))
	{
		day.DayCalc(d, earth);
		if (prev_paksa != day.nPaksa)
		{
			day.nMasa = day.MasaCalc(d, earth);
			
			if (lm != day.nMasa)
			{
				if (lm >= 0)
				{
					t = d;
					t.PreviousDay();
					mlist.arr[current].vc_end = t;
					current++;
				}
				lm = day.nMasa;
				mlist.arr[current].masa = day.nMasa;
				mlist.arr[current].year = day.nGaurabdaYear;
				mlist.arr[current].vc_start = d;
			}
		}
		prev_paksa = day.nPaksa;
		d.NextDay();
		i ++;
	}

	t = d;
	mlist.arr[current].vc_end = t;
	current++;
	mlist.n_countMasa = current;

	return 1;
}

int TResultMasaList::formatText(TString &str)
{
	TString stt;
	TString stt2;
	TResultMasaList &mlist = *this;
	GCStringBuilder sb;

	sb.Target = &str;
	sb.Format = SBTF_TEXT;

	str.Format(" %s\r\n\r\n%s: %s\r\n", GCStrings::getString(39).c_str(), GCStrings::getString(40).c_str(), mlist.m_location.m_strFullName.c_str());
	stt.Format("%s %d %s %d %s %d %s %d\r\n", GCStrings::getString(41).c_str(), mlist.vc_start.day, GCStrings::GetMonthAbreviation(mlist.vc_start.month), mlist.vc_start.year
		, GCStrings::getString(42).c_str(), mlist.vc_end.day, GCStrings::GetMonthAbreviation(mlist.vc_end.month), mlist.vc_end.year);
	str += stt;
	str += "==================================================================\r\n\r\n";

	int i;

	for(i = 0; i < mlist.n_countMasa; i++)
	{
		stt.Format("%s %d                               ", GCStrings::GetMasaName(mlist.arr[i].masa), mlist.arr[i].year);
		stt.Left(30, stt2);
		str += stt2;
		stt.Format("   %d %s %d - ", mlist.arr[i].vc_start.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_start.month), mlist.arr[i].vc_start.year);
		stt.Right(16, stt2);
		str += stt2;
		stt.Format("   %d %s %d\r\n", mlist.arr[i].vc_end.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_end.month), mlist.arr[i].vc_end.year);
		stt.Right(13, stt2);
		str += stt2;
	}

	sb.AppendNote();
	sb.AppendDocumentTail();

	return 1;
}

int TResultMasaList::formatRtf(TString &str)
{
	TString stt;
	TString stt2;
	TResultMasaList &mlist = *this;

	str = "";
	GCStringBuilder sb;
	sb.Target = &str;
	sb.Format = SBTF_RTF;
	sb.fontSizeH1 = GCLayoutData::textSizeH1;
	sb.fontSizeH2 = GCLayoutData::textSizeH2;
	sb.fontSizeText = GCLayoutData::textSizeText;
	sb.fontSizeNote = GCLayoutData::textSizeNote;

	sb.AppendDocumentHeader();

	stt.Format("{\\fs%d\\f2 %s}\\par\\tx%d\\tx%d\\f2\\fs%d\r\n\\par\r\n%s: %s\\par\r\n"
		, GCLayoutData::textSizeH1
		, GCStrings::getString(39).c_str(), 1000*GCLayoutData::textSizeText/24, 4000*GCLayoutData::textSizeText/24
		, GCLayoutData::textSizeText, GCStrings::getString(40).c_str(), mlist.m_location.m_strFullName.c_str());
	str += stt;
	stt.Format("%s %d %s %d %s %d %s %d\\par\r\n", GCStrings::getString(41).c_str(), mlist.vc_start.day, GCStrings::GetMonthAbreviation(mlist.vc_start.month), mlist.vc_start.year
		, GCStrings::getString(42).c_str(), mlist.vc_end.day, GCStrings::GetMonthAbreviation(mlist.vc_end.month), mlist.vc_end.year);
	str += stt;
	str += "==================================================================\\par\r\n\\par\r\n";

	int i;

	for(i = 0; i < mlist.n_countMasa; i++)
	{
		stt2.Format("\\tab %s %d\\tab ", GCStrings::GetMasaName(mlist.arr[i].masa), mlist.arr[i].year);
		str += stt2;
		stt2.Format("%d %s %d - ", mlist.arr[i].vc_start.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_start.month), mlist.arr[i].vc_start.year);
		str += stt2;
		stt2.Format("%d %s %d\\par\r\n", mlist.arr[i].vc_end.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_end.month), mlist.arr[i].vc_end.year);
		str += stt2;
	}

	sb.AppendNote();
	sb.AppendDocumentTail();

	return 1;
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

int TResultMasaList::writeHtml(FILE * f)
{
	TString stt;
	TString stt2;
	TResultMasaList &mlist = *this;

	fprintf(f, "<html>\n<head>\n<title>Masa List</title>\n\n");
	fprintf(f, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:11pt;\n}\n\ntd.hed {\n  font-size:11pt;\n  font-weight:bold;\n");
	fprintf(f, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(f, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(f, "</head>\n");
	fprintf(f, "<body>\n\n");

	fprintf(f, "<p style=\'text-align:center\'><span style=\'font-size:14pt\'>Masa List</span></br>%s: %s</p>\n", GCStrings::getString(40).c_str(), mlist.m_location.m_strFullName.c_str());
	fprintf(f, "<p align=center>%s %d %s %d %s %d %s %d</p>\n", GCStrings::getString(41).c_str(), mlist.vc_start.day, GCStrings::GetMonthAbreviation(mlist.vc_start.month), mlist.vc_start.year
		, GCStrings::getString(42).c_str(), mlist.vc_end.day, GCStrings::GetMonthAbreviation(mlist.vc_end.month), mlist.vc_end.year);
	fprintf(f, "<hr width=\"50%%\">");

	fprintf(f, "<table align=center>");
	fprintf(f, "<tr><td class=\"hed\" style=\'text-align:left\'>MASA NAME&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td class=\"hed\">START</td><td class=\"hed\">END</td></tr>");
	int i;
	for(i = 0; i < mlist.n_countMasa; i++)
	{
		fprintf(f, "<tr>");
		fprintf(f, "<td>%s %d&nbsp;&nbsp;&nbsp;&nbsp;</td>", GCStrings::GetMasaName(mlist.arr[i].masa), mlist.arr[i].year);
		fprintf(f, "<td>%d %s %d</td>", mlist.arr[i].vc_start.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_start.month), mlist.arr[i].vc_start.year);
		fprintf(f, "<td>%d %s %d</td>", mlist.arr[i].vc_end.day, GCStrings::GetMonthAbreviation(mlist.arr[i].vc_end.month), mlist.arr[i].vc_end.year);
		fprintf(f, "</tr>");
	}
	fprintf(f, "</table>");
	fprintf(f, "<hr width=\"50%%\">\n<p align=center>Generated by %s</p>", GCStrings::getString(130).c_str());
	fprintf(f, "</body></html>");
	return 1;
}


