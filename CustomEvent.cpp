// CustomEvent.cpp: implementation of the CCustomEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vcal5beta.h"
#include "CustomEvent.h"
#include "TFile.h"
#include "TFileRichList.h"
#include "level_0.h"
#include "GCStrings.h"
#include "GCGlobal.h"

NSMutableArray<CCustomEvent> CCustomEventList::list;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomEvent::CCustomEvent()
{
	nClass = 0;
	nTithi = 0;
	nMasa = 0;
	nFastType = 0;
	nVisible = 1;
	nUsed = 1;
	nDeleted = 0;
	nSpec = 0;
}

//==============================================================
//
//==============================================================

CCustomEvent::~CCustomEvent()
{

}

CCustomEvent * CCustomEventList::add(void)
{
	CCustomEvent * p = new CCustomEvent();
	list.AddObject(p);
	return p;
}

//==============================================================
//
//==============================================================

int CCustomEventList::OpenFile(const char * pszFile)
{
	TFileRichList F;
	int tithi, masa;
	TString strA, strB, text;
	CCustomEvent * pce;
	int nRet = -1;

	if (!TFile::FileExists(pszFile))
	{
		TFile::CreateFileFromResource(IDR_FILE_EVENTS, pszFile);
	}

	if (F.Open(pszFile, "rt") == TRUE)
	{
		GCGlobal::customEventList.clear();

		nRet ++;
		while(F.ReadLine())
		{
			if (atoi(F.GetTag()) == 13)
			{
				nRet++;
				pce = GCGlobal::customEventList.add();
				if (pce)
				{
					pce->strText = F.GetField(0);
					pce->nMasa   = masa  = atoi(F.GetField(1));
					pce->nTithi  = tithi = atoi(F.GetField(2));
					pce->nClass  = atoi(F.GetField(3));
					pce->nVisible = atoi(F.GetField(4));
					pce->nFastType = atoi(F.GetField(5));
					pce->strFastSubject = F.GetField(6);
					pce->nUsed = atoi(F.GetField(7));
					pce->nSpec = atoi(F.GetField(8));
					const char * str10 = F.GetField(9);
					if (strlen(str10) == 0)
						pce->nStartYear = -10000;
					else
						pce->nStartYear = atoi(str10);

				}

			}
		}

		F.Close();
	}

	return nRet;
}


//==============================================================
//
//==============================================================

int CCustomEventList::SaveFile(const char * pszFile)
{
	if (GCGlobal::customEventListModified == 0)
		return 0;

	TFile csd;
	TString str;
	CCustomEvent * pce;
	int nRet = -1;

	if (csd.Open(pszFile, "w") == TRUE)
	{
		nRet ++;
		for(int i = 0; i < list.Count(); i++)
		{
			pce = list.ObjectAtIndex(i);
			if (pce->nDeleted==0)
			{
				nRet ++;
				// write to file
				str.Format("13 %s|%d|%d|%d|%d|%d|%s|%d|%d\n", pce->strText.c_str(), 
					pce->nMasa, pce->nTithi, pce->nClass, pce->nVisible,
					pce->nFastType, pce->strFastSubject.c_str(), pce->nUsed, pce->nSpec);
				csd.WriteString(str);
			}
		}
		// close file
		csd.Close();
	}

	return nRet;
}
//==============================================================
//
//==============================================================

void CCustomEventList::Export(const char * pszFile, int format) 
{
	TFile f;
	TString strc;

	if (f.Open(pszFile, "w"))
	{
		switch(format)
		{
		case 1:
			for(int nk = 0; nk < CCustomEventList::Count(); nk++)
			{
				CCustomEvent * pce = CCustomEventList::EventAtIndex(nk);
				strc.Format("%s\n\t%s Tithi,%s Paksa,%s Masa\n", pce->strText.c_str(),
					GCStrings::GetTithiName(pce->nTithi), GCStrings::GetPaksaName(pce->nTithi/15), GCStrings::GetMasaName(pce->nMasa));
				f.WriteString(strc);
				if (pce->nFastType)
				{
					strc.Format("\t%s\n", GCStrings::GetFastingName(0x200 + pce->nFastType));
					f.WriteString(strc);
				}
			}
			f.WriteString(GCStrings::getString(78));
			f.WriteString("\n\t\tMakara Sankranti\n");
			f.WriteString(GCStrings::getString(79));
			f.WriteString("\n\t\tMesha Sankranti\n");
			f.WriteString(GCStrings::getString(80));
			f.WriteString("\n\t\tA day before Vrsabha Sankranti\n");
			break;
		case 2:
			{
				f.WriteString("<xml>\n");
				for(int nk = 0; nk < CCustomEventList::Count(); nk++)
				{
					CCustomEvent * pce = CCustomEventList::EventAtIndex(nk);
					f.WriteString("\t<event>\n");
					strc.Format("\t\t<name>%s</name>\n", pce->strText.c_str());
					f.WriteString(strc);
					strc.Format("\t\t<tithi>%s</tithi>\n", GCStrings::GetTithiName(pce->nTithi));
					f.WriteString(strc);
					strc.Format("\t\t<paksa>%s</paksa>\n", GCStrings::GetPaksaName(pce->nTithi/15));
					f.WriteString(strc);
					strc.Format("\t\t<masa>%s</masa>\n", GCStrings::GetMasaName(pce->nMasa));
					f.WriteString(strc);
					f.WriteString("\t</event>\n");
				}
				f.WriteString("\t<event>\n\t\t<name>");
				f.WriteString(GCStrings::getString(78));
				f.WriteString("</name>\n\t\t<sankranti>Makara</sankranti>\n\t\t<rel>0</rel>\n\t</event>\n");
				f.WriteString("\t<event>\n\t\t<name>");
				f.WriteString(GCStrings::getString(79));
				f.WriteString("</name>\n\t\t<sankranti>Mesha</sankranti>\n\t\t<rel>0</rel>\n\t</event>\n");
				f.WriteString("\t<event>\n\t\t<name>");
				f.WriteString(GCStrings::getString(80));
				f.WriteString("</name>\n\t\t<sankranti>Vrsabha</sankranti>\n\t\t<rel>-1</rel>\n\t</event>\n");
				f.WriteString("</xml>\n");
			}
			break;
		default:
			break;
		}

		f.Close();
	}
}

//==============================================================
//
//==============================================================

//==============================================================
//
//==============================================================


void CCustomEventList::clear()
{
	list.RemoveAllObjects();
}

//==============================================================
//
//==============================================================

int CCustomEventList::SetOldStyleFasting(int bOldStyle)
{
	CCustomEvent * pce = NULL;

	const int locMatrix[][5] =
	{
		// visnu tattva and sakti tattva
		{/*tithi*/ 28, /*masa*/  0, /*fast*/ 4, /*new fast*/ 7, /*class*/ 0},
		{/*tithi*/ 29, /*masa*/  3, /*fast*/ 1, 7, 0},
		{/*tithi*/ 22, /*masa*/  4, /*fast*/ 1, 0, 0},
		{/*tithi*/ 26, /*masa*/  4, /*fast*/ 1, 7, 0},
		{/*tithi*/ 21, /*masa*/  9, /*fast*/ 1, 7, 0},
		{/*tithi*/ 26, /*masa*/  9, /*fast*/ 1, 7, 0},
		{/*tithi*/ 27, /*masa*/  9, /*fast*/ 1, 7, 0},
		{/*tithi*/  7, /*masa*/  4, /*fast*/ 5, 7, 0},
		{/*tithi*/ 29, /*masa*/ 10, /*fast*/ 3, 7, 0},
		{/*tithi*/ 23, /*masa*/ 11, /*fast*/ 2, 7, 0},
		// acaryas
		{/*tithi*/  8, /*masa*/  4, /*fast*/ 1, 0, 2},
		{/*tithi*/ 27, /*masa*/  4, /*fast*/ 1, 0, 2},
		{/*tithi*/ 14, /*masa*/  2, /*fast*/ 1, 0, 2},
		{/*tithi*/ 25, /*masa*/  6, /*fast*/ 1, 0, 2},
		{/*tithi*/ 18, /*masa*/  6, /*fast*/ 1, 0, 2},
		{/*tithi*/  3, /*masa*/  8, /*fast*/ 1, 0, 2},
		{/*tithi*/  4, /*masa*/ 10, /*fast*/ 1, 0, 2},
		{-1,-1,-1,-1, -1}
	};

	int i, idx;
	int ret = 0;
	if (bOldStyle) idx = 2;
	else idx = 3;

	for(i = 0; locMatrix[i][0] >= 0; i++)
	{
		for(int n = 0; n < list.Count(); n++)
		{
			pce = list.ObjectAtIndex(n);
			if (pce->nMasa==locMatrix[i][1] &&
				pce->nTithi==locMatrix[i][0] &&
				pce->nClass==locMatrix[i][4])
			{
				if (pce->nFastType != locMatrix[i][idx])
				{
					ret ++;
					pce->nFastType = locMatrix[i][idx];
				}
				break;
			}
		}
	}

	return ret;
}



int CCustomEventList::Count(void)
{
	return list.Count();
}


CCustomEvent * CCustomEventList::EventAtIndex(int index)
{
	return list.ObjectAtIndex(index);
}
