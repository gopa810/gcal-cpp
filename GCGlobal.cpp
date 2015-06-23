#include "stdafx.h"
#include "GCGlobal.h"
#include "enums.h"
#include "VCal5beta.h"
#include "TFileRichList.h"
#include "GCUserInterface.h"
#include "GCDisplaySettings.h"
#include "TTimeZone.h"
#include "FrameServer.h"
#include "GCLayoutData.h"
#include "TCountry.h"
#include "GCStrings.h"

GCGlobal::GCGlobal(void)
{
}

GCGlobal::~GCGlobal(void)
{
}

CLocationRef GCGlobal::myLocation;

CLocationRef GCGlobal::lastLocation;

VCTIME GCGlobal::dateTimeShown;

VCTIME GCGlobal::dateTimeToday;

VCTIME GCGlobal::dateTimeTomorrow;

VCTIME GCGlobal::dateTimeYesterday;

TLangFileList GCGlobal::languagesList;

TString GCGlobal::applicationStrings[32];

CCustomEventList GCGlobal::customEventList;

GCalApp GCGlobal::application;

int GCGlobal::customEventListModified;

int GCGlobal::initFolders()
{
	char pszBuffer[1024];
	int len, i;
	len = GetModuleFileName(AfxGetInstanceHandle(), pszBuffer, 1020);
	for(i = len-1; i > 0; i--)
	{
		if (pszBuffer[i] == '\\')
		{
			pszBuffer[i+1] = '\0';
			len = i+1;
			break;
		}
	}

	GCGlobal::applicationStrings[GSTR_APPFOLDER] = pszBuffer;

	GCGlobal::applicationStrings[GSTR_CONFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_CONFOLDER] += "config\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_CONFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_LANFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_LANFOLDER] += "lang\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_LANFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_TEMFOLDER] = GCGlobal::applicationStrings[GSTR_APPFOLDER];
	GCGlobal::applicationStrings[GSTR_TEMFOLDER] += "temp\\";
	CreateDirectory(GCGlobal::applicationStrings[GSTR_TEMFOLDER], NULL);

	GCGlobal::applicationStrings[GSTR_CE_FILE].Format("%scevents.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_CONF_FILE].Format("%scurrent.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_LOC_FILE].Format("%slocations.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_SSET_FILE].Format("%sshowset.cfg", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_LOCX_FILE].Format("%sloc.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());//GCAL 3.0
	GCGlobal::applicationStrings[GSTR_CEX_FILE].Format("%scev3.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());//GCAL 3.0
	GCGlobal::applicationStrings[GSTR_CONFX_FILE].Format("%sset.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TZ_FILE].Format("%stz.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_COUNTRY_FILE].Format("%sctrs.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TEXT_FILE].Format("%sstrings.rl", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_TIPS_FILE].Format("%stips.txt", GCGlobal::applicationStrings[GSTR_CONFOLDER].c_str());
	GCGlobal::applicationStrings[GSTR_HELP_FILE].Format("%sgcal.chm", GCGlobal::applicationStrings[GSTR_APPFOLDER].c_str());
	return 1;
}

const char * GCGlobal::getFileName(int n)
{
	return GCGlobal::applicationStrings[n].c_str();
}


void GCGlobal::OpenFile(const char * fileName)
{
	int tagn;
	TFileRichList rf;
	if ( rf.Open(fileName, "r") != 0)
	{
		// nacitava
		while(rf.ReadLine())
		{
			tagn = atoi(rf.GetTag());
			switch(tagn)
			{
			case 12701:
				GCGlobal::myLocation.m_strName     = rf.GetField(0);
				GCGlobal::myLocation.m_fLongitude  = atof(rf.GetField(1));
				GCGlobal::myLocation.m_fLatitude   = atof(rf.GetField(2));
				GCGlobal::myLocation.m_fTimezone   = atof(rf.GetField(3));
				GCGlobal::myLocation.m_strLatitude = rf.GetField(2);
				GCGlobal::myLocation.m_strLongitude = rf.GetField(1);
				GCGlobal::myLocation.m_strTimeZone = rf.GetField(4);
				GCGlobal::myLocation.m_nDST = TTimeZone::GetID(rf.GetField(4));
				break;
			case 12702:
				GCGlobal::lastLocation.m_strName     = rf.GetField(0);
				GCGlobal::lastLocation.m_fLongitude  = atof(rf.GetField(1));
				GCGlobal::lastLocation.m_fLatitude   = atof(rf.GetField(2));
				GCGlobal::lastLocation.m_fTimezone   = atof(rf.GetField(3));
				GCGlobal::lastLocation.m_strLatitude = rf.GetField(2);
				GCGlobal::lastLocation.m_strLongitude = rf.GetField(1);
				GCGlobal::lastLocation.m_strTimeZone = rf.GetField(4);
				GCGlobal::lastLocation.m_nDST = TTimeZone::GetID(rf.GetField(4));
				break;
			case 12710:
				GCUserInterface::windowController.m_rectMain.left = atoi(rf.GetField(0));
				GCUserInterface::windowController.m_rectMain.top = atoi(rf.GetField(1));
				GCUserInterface::windowController.m_rectMain.right = atoi(rf.GetField(2));
				GCUserInterface::windowController.m_rectMain.bottom = atoi(rf.GetField(3));
				break;
			case 12711:
				GCDisplaySettings::setValue(atoi(rf.GetField(0)), atoi(rf.GetField(1)));
				break;
			case 12800:
				GCUserInterface::ShowMode = atoi(rf.GetField(0));
				break;
			case 12801:
				GCLayoutData::textSizeNote = atoi(rf.GetField(0));
				GCLayoutData::textSizeText = atoi(rf.GetField(1));
				GCLayoutData::textSizeH1 = atoi(rf.GetField(2));
				GCLayoutData::textSizeH2 = atoi(rf.GetField(3));
				break;
			default:
				break;
			}
		}
		rf.Close();
	}

}


void GCGlobal::SaveFile(const char * fileName)
{
	TString str;
	TString strFile;
	TFile f;
		
	if (f.Open(fileName,"w")==TRUE)
	{
		str.Format("12701 %s|%f|%f|%f|%s\n", GCGlobal::myLocation.m_strName.c_str(), GCGlobal::myLocation.m_fLongitude, GCGlobal::myLocation.m_fLatitude,
			GCGlobal::myLocation.m_fTimezone, TTimeZone::GetTimeZoneName(GCGlobal::myLocation.m_nDST));
		f.WriteString(str.c_str());
		str.Format("12702 %s|%f|%f|%f|%s\n", GCGlobal::lastLocation.m_strName.c_str(), GCGlobal::lastLocation.m_fLongitude, GCGlobal::lastLocation.m_fLatitude,
			GCGlobal::lastLocation.m_fTimezone, TTimeZone::GetTimeZoneName(GCGlobal::lastLocation.m_nDST));
		f.WriteString(str.c_str());
		str.Format("12710 %d|%d|%d|%d\n", GCUserInterface::windowController.m_rectMain.left, GCUserInterface::windowController.m_rectMain.top,
			GCUserInterface::windowController.m_rectMain.right, GCUserInterface::windowController.m_rectMain.bottom );
		f.WriteString(str.c_str());
		for(int y=0; y < GCDisplaySettings::getCount(); y++)
		{
			str.Format("12711 %d|%d\n", y, GCDisplaySettings::getValue(y));
			f.WriteString(str.c_str());
		}
		str.Format("12800 %d\n", GCUserInterface::ShowMode);
		f.WriteString(str.c_str());
		str.Format("12801 %d|%d|%d|%d\n", GCLayoutData::textSizeNote, GCLayoutData::textSizeText,
			GCLayoutData::textSizeH1, GCLayoutData::textSizeH2);
		f.WriteString(str.c_str());

		f.Close();
	}
}


bool GCGlobal::GetLangFileForAcr(const char * pszAcr, TString &strFile)
{
	TLangFileInfo * p = GCGlobal::languagesList.list;

	while(p)
	{
		if (p->m_strAcr.CompareNoCase(pszAcr) == 0)
		{
			strFile = p->m_strFile;
			return true;
		}
		p = p->next;
	}
	return false;
}


void GCGlobal::LoadInstanceData(void)
{
	TString strFile;
	TFile f;
	TString str, strA, strB;

	// initialization for AppDir
	GCGlobal::initFolders();

	// initialization of global strings
	GCStrings::readFile(GCGlobal::getFileName(GSTR_TEXT_FILE));

	// inicializacia countries
	TCountry::InitWithFile(GCGlobal::getFileName(GSTR_COUNTRY_FILE));

	// inicializacia miest a kontinentov
	CLocationList::OpenFile(GCGlobal::getFileName(GSTR_LOCX_FILE));

	// inicializacia zobrazovanych nastaveni
	GCDisplaySettings::readFile(GCGlobal::getFileName(GSTR_SSET_FILE));

	// inicializacia custom events
	CCustomEventList::OpenFile(GCGlobal::getFileName(GSTR_CEX_FILE));

	// initialization of global variables
	GCGlobal::myLocation.m_fLongitude = 77.73;
	GCGlobal::myLocation.m_fLatitude = 27.583;
	GCGlobal::myLocation.m_fTimezone = 5.5;
	GCGlobal::myLocation.m_strLatitude = "27N35";
	GCGlobal::myLocation.m_strLongitude = "77E42";
	GCGlobal::myLocation.m_strName = "Vrindavan, India";
	GCGlobal::myLocation.m_strTimeZone = "+5:30";
	GCGlobal::myLocation.m_nDST = 188;
	GCGlobal::lastLocation.m_fLongitude = 77.73;
	GCGlobal::lastLocation.m_fLatitude = 27.583;
	GCGlobal::lastLocation.m_fTimezone = 5.5;
	GCGlobal::lastLocation.m_strLatitude = "27N35";
	GCGlobal::lastLocation.m_strLongitude = "77E42";
	GCGlobal::lastLocation.m_strName = "Vrindavan, India";
	GCGlobal::lastLocation.m_strTimeZone = "+5:30";
	GCGlobal::lastLocation.m_nDST = 188;

	GCGlobal::OpenFile(GCGlobal::getFileName(GSTR_CONFX_FILE));
	// refresh fasting style after loading user settings
	CCustomEventList::SetOldStyleFasting(GCDisplaySettings::getValue(42));

	// inicializacia tipov dna
	if (!TFile::FileExists(GCGlobal::getFileName(GSTR_TIPS_FILE)))
	{
		TFile::CreateFileFromResource(IDR_FILE_TIPS, GCGlobal::getFileName(GSTR_TIPS_FILE));
	}
}


void GCGlobal::SaveInstanceData(void)
{
	GCGlobal::SaveFile(GCGlobal::getFileName(GSTR_CONFX_FILE));

	if (CLocationList::IsModified())
	{
		CLocationList::SaveAs(GCGlobal::getFileName(GSTR_LOCX_FILE),4);//GCAL 3.0
	}

	if (TCountry::IsModified())
	{
		TCountry::SaveToFile(GCGlobal::getFileName(GSTR_COUNTRY_FILE));
	}

	if (GCStrings::gstr_Modified)
	{
		GCStrings::writeFile(GCGlobal::getFileName(GSTR_TEXT_FILE));
	}

	CCustomEventList::SaveFile(GCGlobal::getFileName(GSTR_CEX_FILE));

}
