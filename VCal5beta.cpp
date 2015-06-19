// VCal5beta.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "platform.h"
#include "VCal5beta.h"
#include "CommandLineVCal.h"
#include "FrameMain.h"
#include "FrameServer.h"
#include "locationref.h"
#include "TFile.h"
#include "location.h"
#include "customevent.h"
#include "dlgasklimitation.h"
#include "TTimeZone.h"
#include "langfileinfo.h"
#include "TFileRichList.h"
#include "TCountry.h"
#include "GCStrings.h"
#include "GCCalendar.h"
#include "GCDisplaySettings.h"
#include "GCLayoutData.h"
#include "GCSankranti.h"
#include "GCNaksatra.h"
#include "GCTithi.h"
#include "GCUserInterface.h"
#include "GCGlobal.h"
#include "GCUserInterface.h"
#include "GCWelcomeTips.h"

/////////////////////////////////////////////////////////////////////////////
// GCalApp

BEGIN_MESSAGE_MAP(GCalApp, CWinApp)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)
	//{{AFX_MSG_MAP(GCalApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GCalApp construction

GCalApp::GCalApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bWindowless = FALSE;
	m_bHelpAvailable = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// GCalApp initialization
//double AvcGetTimeZone(int);
BOOL GCalApp::InitInstance()
{
	//AddDocTemplate(pDocTemplate);
	GCUserInterface::windowController.Initialize();

	//MessageBox(NULL, getenv("QUERY_STRING"), "a", MB_OK);
	//MessageBox(NULL, getenv("Path"), "a", MB_OK);
	m_bWindowless = TRUE;

	SetRegistryKey(_T("GCAL"));

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

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCommandLineVCal cmdInfo;
	//ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	/*if (!ProcessShellCommand(cmdInfo))
		return FALSE;*/

	m_bWindowless = ParseCommandArguments(&cmdInfo);

	//if (cmdInfo.GetCount())
	if (!m_bWindowless)
	{
		//m_bWindowless = FALSE;
		/*NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);

		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);*/

		InitInstanceData();

		CFrameMain * pMainFrame = GCUserInterface::windowController.CreateNewFrame();

		if (pMainFrame == NULL)
			return FALSE;

		m_pMainWnd = pMainFrame;

		ShowTipAtStartup();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	// CG: This line inserted by 'Tip of the Day' component.

	return TRUE;
}

void GCalApp::InitInstanceData()
{
	TString strFile;
	TFile f;
	TString str, strA, strB;
	TLangFileInfo * pl;


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
	if (TFile::FileExists(GCGlobal::getFileName(GSTR_TIPS_FILE)))
	{
		if (f.Open(GCGlobal::getFileName(GSTR_TIPS_FILE), "w")==TRUE)
		{
			GCWelcomeTips::initializeFile(f.m_fHandle);
			f.Close();
		}
	}

	if (TFile::FileExists(GCGlobal::getFileName(GSTR_HELP_FILE)))
	{
		m_bHelpAvailable = TRUE;
	}
}

DWORD GCalApp::GetDWordFromString(const char * psz)
{
	return (DWORD)atoi(psz);
}

int GCalApp::ExitInstance() 
{
	if (m_bWindowless == FALSE)
	{
		GCGlobal::SaveFile(GCGlobal::getFileName(GSTR_CONFX_FILE));
	}

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

	return CWinApp::ExitInstance();
}

BOOL GCalApp::InitLanguageOutputFromFile(const char * pszFile)
{
	TFile f;
	TString strA, strB;

	if (f.Open(pszFile, "r") == TRUE)
	{
		int n;

		while(f.ReadString2(strA, strB))
		{
			n = atoi(strA);
			if ((n >= 0) && (n < 800))
			{
				GCStrings::setString(n, strB);
			}
		}

		f.Close();

		return TRUE;
	}

	return FALSE;
}

BOOL GCalApp::GetLangFileForAcr(const char * pszAcr, TString &strFile)
{
	TLangFileInfo * p = GCGlobal::languagesList.list;

	while(p)
	{
		if (p->m_strAcr.CompareNoCase(pszAcr) == 0)
		{
			strFile = p->m_strFile;
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}

#define ISARGSEP(c) (((c) == 0) || ((c)==' '))


int GCalApp::ParseCommandArguments(CCommandLineVCal * cmd)
{
	// if no arguments, then application should open window
	if (_tcslen(m_lpCmdLine) < 1)
		return FALSE;
	
	char * arg = new char[_tcslen(m_lpCmdLine) + 1];
	if (arg == NULL)
		return FALSE;

	const char * parse = m_lpCmdLine;
	int argp = 0;
	int argc = 0;
	char * args[64];
	char stopc;
	char * pw = arg;

	while(*parse)
	{
		while(*parse != 0 && *parse==' ')
			parse++;
		if (*parse)
		{
			args[argc] = pw;
			argc++;
		}
		stopc = (*parse == '\"') ? '\"' : ' ';
		if (stopc == '\"')
			parse++;
		while(*parse != 0 && *parse != stopc)
		{
			*pw = *parse;
			pw++;
			parse++;
		}
		if (*parse)
			parse++;
		*pw = 0;
		pw++;
	}

	CLocationRef loc;
	VCTIME vcStart, vcEnd;
	VATIME vaStart, vaEnd;
	int nCount;
	int nReq;
	TString str;
	TString strFileOut;
	FILE * fout = stdout;

	loc.m_fLatitude = 0.0;
	loc.m_fLongitude = 0.0;
	loc.m_fTimezone = 0.0;
	loc.m_nDST = 0;
	loc.m_strName = "";
	vcStart.day = 0;
	vcStart.month = 0;
	vcStart.year = 0;
	vcEnd = vcStart;
	vaStart.tithi = vaStart.masa = vaStart.gyear = 0;
	vaEnd = vaStart;
	nCount = -1;

	for(int i = 0; i < argc; i++)
	{
		//TRACE2("arg %d = %s\n", i, args[i]);
		if (strcmp(args[i],"-L") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_strLongitude = args[i+1];
				cmd->GetArg_EarthPos(args[i+1], loc.m_fLatitude, loc.m_fLongitude);
				//TRACE2("-L latitude=%f longitude=%f\n", loc.m_fLatitude, loc.m_fLongitude);
			}
			i++;
		}
		else if (strcmp(args[i],"-N") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_strName = args[i+1];
				//TRACE1("-N name=%s\n", loc.m_strName);
			}
			i++;
		}
		else if (strcmp(args[i],"-SV") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_VaisnDate(args[i+1], vaStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-SG") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Date(args[i+1], vcStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-ST") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Time(args[i+1], vcStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-EG") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Date(args[i+1], vcEnd);
				//AfxTrace("-EG day=%d month=%d year=%d\n", vcEnd.day, vcEnd.month, vcEnd.year);
			}
			i++;
		}
		else if (strcmp(args[i],"-EV") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_VaisnDate(args[i+1], vaEnd);
				//AfxTrace("-EV tithi=%d masa=%d gyear=%d\n", vaEnd.tithi, vaEnd.masa, vaEnd.gyear);
			}
			i++;
		}
		else if (strcmp(args[i],"-EC") == 0)
		{
			if (argc >= i+2)
			{
				nCount = atoi(args[i+1]);
				//AfxTrace("couint = %d\n", nCount);
			}
			i++;
		}
		else if (strcmp(args[i],"-TZ") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_TimeZone(args[i+1], loc.m_fTimezone);
				//TRACE1("-TZ  timezone=%f\n", loc.m_fTimezone);
			}
			i++;
		}
		else if (strcmp(args[i],"-LNG") == 0)
		{
			if (argc >= i+2)
			{
				TString strLang;
				strLang = args[i+1];
				TString strFile;
				if (GetLangFileForAcr(strLang, strFile) == TRUE)
				{
					InitLanguageOutputFromFile(strFile);
				}
			}
			i++;
		}
		else if (strcmp(args[i],"-DST") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_nDST = TTimeZone::GetID(args[i+1]);
			}
			i++;
		}
		else if (strcmp(args[i],"-O") == 0)
		{
			if (argc >= i+2)
			{
				if (fout != stdout && fout != stderr)
					fclose(fout);
				fout = fopen(args[i+1], "wt");
				//strFileOut = args[i+1];
				if (fout == NULL)
					fout = stderr;
			}
			i++;
		}
		else if (stricmp(args[i],"-R") == 0)
		{
			if (argc >= i+2)
			{
				if (stricmp(args[i + 1], "calendar") == 0)
				{
					nReq = 10;
				} else if (stricmp(args[i + 1], "appday") == 0)
				{
					nReq = 11;
				} else if (stricmp(args[i + 1], "tithi") == 0)
				{
					nReq = 12;
				} else if (stricmp(args[i + 1], "sankranti") == 0)
				{
					nReq = 13;
				} else if (stricmp(args[i + 1], "naksatra") == 0)
				{
					nReq = 14;
				} else if (stricmp(args[i + 1], "firstday") == 0)
				{
					nReq = 15;
				} else if (stricmp(args[i + 1], "gcalendar") == 0)
				{
					nReq = 16;
				} else if (stricmp(args[i + 1], "gtithi") == 0)
				{
					nReq = 17;
				} else if (stricmp(args[i + 1], "next") == 0)
				{
					nReq = 18;
				} else if (stricmp(args[i + 1], "xlan") == 0)
				{
					nReq = 50;
				}
				else if (stricmp(args[i + 1], "help") == 0)
				{
					nReq = 60;
				}
				/*else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				}*/
			}
			i++;
		}
	}

	loc.m_strFullName.Format("%s (%s %s, %s)", loc.m_strName.c_str(), loc.m_strLatitude.c_str(), 
		loc.m_strLongitude.c_str(), loc.m_strTimeZone.c_str());
	vcStart.tzone = loc.m_fTimezone;
	vcEnd.tzone = loc.m_fTimezone;
	//AfxTrace("vcStart = %d,%d,%d,...,%f\n", vcStart.day, vcStart.month, vcStart.year, vcStart.shour);

	switch(nReq)
	{
	case 10:
	case 13:
	case 14:
		if (vcStart.year == 0 && vaStart.gyear != 0)
			GCCalendar::VATIMEtoVCTIME(vaStart, vcStart, (EARTHDATA)loc);
		if (vcEnd.year == 0 && vaEnd.gyear != 0)
			GCCalendar::VATIMEtoVCTIME(vaEnd, vcEnd, (EARTHDATA)loc);		
		break;
	default:
		break;
	}

	if (vcStart.year != 0 && vcEnd.year != 0 && nCount < 0)
		nCount = int(vcEnd.GetJulian() - vcStart.GetJulian());

	if (nCount < 0)
		nCount = 30;

	TResultApp appday;
	TResultCalendar calendar;
	//AfxTrace("Count === %d\n", nCount);

	switch(nReq)
	{
	case 10:
		// -R -O -LAT -LON -SG -C [-DST -NAME]
		vcStart.NextDay();
		vcStart.PreviousDay();
		GCUserInterface::CalculateCalendar(calendar, loc, vcStart, nCount);
		calendar.writeXml(fout);
		break;
	case 11:
		// -R -O -LAT -LON -SG -ST [-NAME]
		appday.calculateAppDay(loc, vcStart);
		appday.formatXml(str);
		fputs(str, fout);
		break;
	case 12:
		GCTithi::writeXml( fout, loc, vcStart);
		break;
	case 13:
		if (vcEnd.year == 0)
		{
			vcEnd = vcStart;
			vcEnd += nCount;
		}
		GCSankranti::writeXml(fout, loc, vcStart, vcEnd);
		break;
	case 14:
		GCNaksatra::writeXml(fout, loc, vcStart, nCount);
		break;
	case 15:
		GCCalendar::writeFirstDayXml(fout, loc, vcStart);
		break;
	case 16:
		vcStart = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, vcStart.year);
		vcEnd = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, vcStart.year + 1);
		nCount = int(vcEnd.GetJulian() - vcStart.GetJulian());
		GCUserInterface::CalculateCalendar(calendar, loc, vcStart, nCount);
		calendar.writeXml(fout);
		break;
	case 17:
		GCTithi::writeGaurabdaTithiXml( fout, loc, vaStart, vaEnd);
		break;
	case 18:
		GCTithi::writeGaurabdaNextTithiXml( fout, loc, vcStart, vaStart);
		break;
	case 50:
		{
			int i = 0;
			TString str;
			fputs("10000\nEnglish\n10001\nen\n", fout);
			// export obsahu do subora
			for(i = 0; i < 900; i ++)
			{
				if (!GCStrings::getString(i).IsEmpty())
				{
					str.Format("%d\n%s\n", i, GCStrings::getString(i).c_str());
					fputs(str, fout);
				}
			}
		}
		break;
	}
	// application should be windowless
	// since some parameters are present
	delete [] arg;

	return TRUE;
}

void GCalApp::ShowTipAtStartup(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}

}

void GCalApp::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CTipDlg dlg;
	dlg.DoModal();

}
