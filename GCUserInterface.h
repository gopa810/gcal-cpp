#pragma once

#include "GCGregorianTime.h"
#include "LocationRef.h"
#include "TResultCalendar.h"

class DlgCalcProgress;
class CFrameServer;

class GCUserInterface
{
public:
	GCUserInterface(void);
	~GCUserInterface(void);

	static int CalculateCalendar(TResultCalendar & daybuff, CLocationRef & loc, VCTIME date, int nDaysCount); 
	static int CreateProgressWindow();
	static int SetProgressWindowRange(int nMin, int nMax);
	static int SetProgressWindowPos(double nPos);
	static int CloseProgressWindow();
	static void ShowHelp(LPCTSTR pszFile);
	static CFrameServer windowController;
	static DlgCalcProgress * dcp;

	static int dstSelectionMethod;
	static int ShowMode;
};

