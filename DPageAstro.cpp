// DPageAstro.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "DPageAstro.h"
#include "showset.h"

extern CShowSetting gss[];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DPageAstro property page

IMPLEMENT_DYNCREATE(DPageAstro, CPropertyPage)

DPageAstro::DPageAstro() : CPropertyPage(DPageAstro::IDD)
{
	//{{AFX_DATA_INIT(DPageAstro)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPageAstro::~DPageAstro()
{
}

void DPageAstro::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPageAstro)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPageAstro, CPropertyPage)
	//{{AFX_MSG_MAP(DPageAstro)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPageAstro message handlers

void DPageAstro::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage::OnOK();
}

BOOL DPageAstro::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CheckDlgButton(IDC_CHECK1, gss[2].val);
	CheckDlgButton(IDC_CHECK2, gss[3].val);
	CheckDlgButton(IDC_CHECK3, gss[9].val);
	CheckDlgButton(IDC_CHECK4, gss[4].val);
	CheckDlgButton(IDC_CHECK5, gss[5].val);
	CheckDlgButton(IDC_CHECK6, gss[10].val);
	CheckDlgButton(IDC_CHECK7, gss[34].val);
	CheckDlgButton(IDC_CHECK8, gss[48].val);
	
	return CPropertyPage::OnSetActive();
}

BOOL DPageAstro::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	LPNMHDR lp = (LPNMHDR)lParam;

	if (lp->code == PSN_HELP)
	{
		MessageBox("This page allows to change the mode of display of the astronomical events in the calendar.", "Settings for Astronomy Display", MB_OK);
		return TRUE;
	}
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

BOOL DPageAstro::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	gss[2].val = IsDlgButtonChecked(IDC_CHECK1);
	gss[3].val = IsDlgButtonChecked(IDC_CHECK2);
	gss[9].val = IsDlgButtonChecked(IDC_CHECK3);
	gss[4].val = IsDlgButtonChecked(IDC_CHECK4);
	gss[5].val = IsDlgButtonChecked(IDC_CHECK5);
	gss[10].val = IsDlgButtonChecked(IDC_CHECK6);
	gss[34].val = IsDlgButtonChecked(IDC_CHECK7);
	gss[48].val = IsDlgButtonChecked(IDC_CHECK8);
	
	return CPropertyPage::OnKillActive();
}
