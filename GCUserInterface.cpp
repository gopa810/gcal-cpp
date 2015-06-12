#include "stdafx.h"
#include "GCUserInterface.h"
#include "GCDisplaySettings.h"
#include "resource.h"
#include "DlgCalcProgress.h"

GCUserInterface::GCUserInterface(void)
{
}


GCUserInterface::~GCUserInterface(void)
{
}

int GCUserInterface::ShowMode = 1; // 0=text,1=rtf

int GCUserInterface::CalculateCalendar(TResultCalendar &daybuff, CLocationRef & loc, VCTIME date, int nDaysCount) 
{
	TString str, str2, str3, dayText;

	char * spaces = "                                                                                ";
	int lastmasa = -1;
	int lastmonth = -1;
	bool bCalcMoon = (GCDisplaySettings::getValue(4) > 0 || GCDisplaySettings::getValue(5) > 0);

	DlgCalcProgress dcp;

	dcp.Create(IDD_CALC_PROGRESS, AfxGetMainWnd());
	dcp.CenterWindow();
	dcp.ShowWindow(SW_SHOW);
	dcp.UpdateWindow();

	daybuff.m_pProgress = &(dcp.m_p1);

	if (daybuff.CalculateCalendar(loc, date, nDaysCount) == 0)
		return 0;

	daybuff.m_pProgress = NULL;

	dcp.DestroyWindow();

	return 1;
}

DlgCalcProgress dcp;

int GCUserInterface::CreateProgressWindow()
{
	dcp.Create(IDD_CALC_PROGRESS, AfxGetMainWnd());
	dcp.CenterWindow();
	dcp.ShowWindow(SW_SHOW);
	dcp.UpdateWindow();

	return 0;
}

int GCUserInterface::SetProgressWindowRange(int nMin, int nMax)
{
	dcp.m_p1.SetRange32(nMin, nMax);

	return 0;
}

int GCUserInterface::SetProgressWindowPos(int nPos)
{
	dcp.m_p1.SetPos(nPos);

	return 0;
}

int GCUserInterface::CloseProgressWindow()
{
	dcp.DestroyWindow();

	return 0;
}