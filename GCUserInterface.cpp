#include "stdafx.h"
#include "GCUserInterface.h"
#include "GCDisplaySettings.h"
#include "resource.h"
#include "DlgCalcProgress.h"
#include "FrameServer.h"

GCUserInterface::GCUserInterface(void)
{
}


GCUserInterface::~GCUserInterface(void)
{
}

int GCUserInterface::dstSelectionMethod = 2;

int GCUserInterface::ShowMode = 1; // 0=text,1=rtf

CFrameServer GCUserInterface::windowController;

////////////////////////////////////////////////////////

void GCUserInterface::ShowHelp(LPCTSTR pszFile)
{
	CString str;

	str.Format("gcal.chm::/%s", pszFile);

	HtmlHelp(NULL, str, HH_DISPLAY_TOPIC, 0);
}

int GCUserInterface::CalculateCalendar(TResultCalendar &daybuff, CLocationRef & loc, VCTIME date, int nDaysCount) 
{
	TString str, str2, str3, dayText;

	char * spaces = "                                                                                ";
	int lastmasa = -1;
	int lastmonth = -1;
	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);

	GCUserInterface::CreateProgressWindow();

	if (daybuff.CalculateCalendar(loc, date, nDaysCount) == 0)
		return 0;

	GCUserInterface::CloseProgressWindow();

	return 1;
}

DlgCalcProgress * GCUserInterface::dcp = NULL;

int GCUserInterface::CreateProgressWindow()
{
	if (dcp == NULL)
	{
		GCUserInterface::dcp = new DlgCalcProgress();
		GCUserInterface::dcp->Create(IDD_CALC_PROGRESS, AfxGetMainWnd());
	}

	GCUserInterface::dcp->CenterWindow();
	GCUserInterface::dcp->ShowWindow(SW_SHOW);
	GCUserInterface::dcp->UpdateWindow();

	return 0;
}

int GCUserInterface::SetProgressWindowRange(int nMin, int nMax)
{
	if (GCUserInterface::dcp != NULL)
	{
		GCUserInterface::dcp->m_p1.SetRange32(nMin, nMax);
	}

	return 0;
}

int GCUserInterface::SetProgressWindowPos(double nPos)
{
	if (GCUserInterface::dcp != NULL)
	{
		GCUserInterface::dcp->m_p1.SetPos((int)nPos);
	}

	return 0;
}

int GCUserInterface::CloseProgressWindow()
{
	if (dcp != NULL)
	{
		GCUserInterface::dcp->DestroyWindow();
		delete GCUserInterface::dcp;
		GCUserInterface::dcp = NULL;
	}

	return 0;
}