// VCal5beta.h : main header file for the VCAL5BETA application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "afxtempl.h"
#include "resource.h"		// main symbols
#include "TString.h"

/////////////////////////////////////////////////////////////////////////////
// GCalApp:
// See VCal5beta.cpp for the implementation of this class
//
class CCommandLineVCal;

class GCalApp : public CWinApp
{
public:
	BOOL m_bHelpAvailable;
	BOOL m_bWindowless;
	GCalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GCalApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(GCalApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ShowTipAtStartup(void);
private:
	void ShowTipOfTheDay(void);
};

