// VCal5beta.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TString.h"
#include "VCal5beta.h"
#include "CommandLineVCal.h"
#include "FrameMain.h"
#include "FrameServer.h"
#include "locationref.h"
#include "TFile.h"
#include "location.h"
#include "customevent.h"
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
	GCUserInterface::windowController.Initialize();

	m_bWindowless = TRUE;

	SetRegistryKey(_T("GCAL"));


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCommandLineVCal cmdInfo;

	GCGlobal::LoadInstanceData();

	m_bWindowless = cmdInfo.ParseCommandArguments(m_lpCmdLine);

	//if (cmdInfo.GetCount())
	if (!m_bWindowless)
	{

		if (TFile::FileExists(GCGlobal::getFileName(GSTR_HELP_FILE)))
		{
			m_bHelpAvailable = TRUE;
		}

		CFrameMain * pMainFrame = GCUserInterface::windowController.CreateNewFrame();

		if (pMainFrame == NULL)
			return FALSE;

		m_pMainWnd = pMainFrame;

		ShowTipAtStartup();
	}



	return TRUE;
}

int GCalApp::ExitInstance() 
{
	GCGlobal::SaveInstanceData();

	return CWinApp::ExitInstance();
}


#define ISARGSEP(c) (((c) == 0) || ((c)==' '))



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
