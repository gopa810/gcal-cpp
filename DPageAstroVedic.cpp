// DPageToday.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "DPageAstroVedic.h"
#include "showset.h"

extern CShowSetting gss[];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DPageToday property page

IMPLEMENT_DYNCREATE(DPageToday, CPropertyPage)

DPageToday::DPageToday() : CPropertyPage(DPageToday::IDD)
{
	//{{AFX_DATA_INIT(DPageToday)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPageToday::~DPageToday()
{
}

void DPageToday::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPageToday)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPageToday, CPropertyPage)
	//{{AFX_MSG_MAP(DPageToday)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPageToday message handlers

void DPageToday::OnOK() 
{
	CPropertyPage::OnOK();
}

BOOL DPageToday::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CheckDlgButton(IDC_CHECK1, gss[29].val);
	CheckDlgButton(IDC_CHECK2, gss[30].val);
	CheckDlgButton(IDC_CHECK3, gss[31].val);
	CheckDlgButton(IDC_CHECK4, gss[32].val);
	CheckDlgButton(IDC_CHECK5, gss[33].val);
	CheckDlgButton(IDC_CHECK6, gss[45].val);
	CheckDlgButton(IDC_CHECK7, gss[46].val);
	CheckDlgButton(IDC_CHECK8, gss[47].val);
	
	return CPropertyPage::OnSetActive();
}

BOOL DPageToday::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	gss[29].val = IsDlgButtonChecked(IDC_CHECK1);
	gss[30].val = IsDlgButtonChecked(IDC_CHECK2);
	gss[31].val = IsDlgButtonChecked(IDC_CHECK3);
	gss[32].val = IsDlgButtonChecked(IDC_CHECK4);
	gss[33].val = IsDlgButtonChecked(IDC_CHECK5);
	gss[45].val = IsDlgButtonChecked(IDC_CHECK6);
	gss[46].val = IsDlgButtonChecked(IDC_CHECK7);
	gss[47].val = IsDlgButtonChecked(IDC_CHECK8);
	
	return CPropertyPage::OnKillActive();
}
