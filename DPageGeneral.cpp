// DPageGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "DPageGeneral.h"
#include "showset.h"
#include "strings.h"
#include "GCStrings.h"

extern CShowSetting gss[];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DPageGeneral property page

IMPLEMENT_DYNCREATE(DPageGeneral, CPropertyPage)

DPageGeneral::DPageGeneral() : CPropertyPage(DPageGeneral::IDD)
{
	//{{AFX_DATA_INIT(DPageGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPageGeneral::~DPageGeneral()
{
}

void DPageGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPageGeneral)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPageGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(DPageGeneral)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPageGeneral message handlers

void DPageGeneral::OnOK() 
{
	CPropertyPage::OnOK();
}

BOOL DPageGeneral::OnSetActive() 
{
	int i;
	SendDlgItemMessage(IDC_COMBO1, CB_RESETCONTENT);
	SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)LPCTSTR("Before New Masa"));
	SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)LPCTSTR("Before New Month"));
	if (gss[18].val == 1)
		i = 0;
	else
		i = 1;
	SendDlgItemMessage(IDC_COMBO1, CB_SETCURSEL, i);

	SendDlgItemMessage(IDC_COMBO2, CB_RESETCONTENT);
	for(i=0; i<7; i++)
		SendDlgItemMessage(IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)LPCTSTR(GCStrings::getString(i).c_str()));
	SendDlgItemMessage(IDC_COMBO2, CB_SETCURSEL, gss[40].val);
	SendDlgItemMessage(IDC_COMBO3, CB_SETCURSEL, gss[49].val);

	CheckDlgButton(IDC_CHECK1, gss[20].val);
	CheckDlgButton(IDC_CHECK2, gss[21].val);

	CheckDlgButton(IDC_CHECK18, gss[11].val);
	CheckDlgButton(IDC_CHECK19, gss[0].val);
	CheckDlgButton(IDC_CHECK3, gss[1].val);
	CheckDlgButton(IDC_CHECK4, gss[7].val);
	CheckDlgButton(IDC_CHECK7, gss[12].val);
	CheckDlgButton(IDC_CHECK5, gss[8].val);
	CheckDlgButton(IDC_CHECK6, gss[35].val);
	CheckDlgButton(IDC_CHECK8, gss[50].val);
	SendDlgItemMessage(IDC_COMBO4, CB_SETCURSEL, gss[51].val);

	return CPropertyPage::OnSetActive();
}

BOOL DPageGeneral::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	LPNMHDR lp = (LPNMHDR)lParam;

	if (lp->code == PSN_HELP)
	{
		MessageBox("This page allows to change the mode of display of the general information in the calendar.", "General Settings of Display", MB_OK);
		return TRUE;
	}
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

BOOL DPageGeneral::OnKillActive() 
{
	int i = 0;
	i = SendDlgItemMessage(IDC_COMBO1, CB_GETCURSEL);
	gss[20].val = IsDlgButtonChecked(IDC_CHECK1);
	gss[21].val = IsDlgButtonChecked(IDC_CHECK2);
	if (i >= 0 && i < 2)
	{
		gss[18 + i].val = 1;
		gss[19 - i].val = 0;
	}
	gss[11].val = IsDlgButtonChecked(IDC_CHECK18);
	gss[0].val = IsDlgButtonChecked(IDC_CHECK19);
	gss[1].val = IsDlgButtonChecked(IDC_CHECK3);
	gss[7].val = IsDlgButtonChecked(IDC_CHECK4);
	gss[8].val = IsDlgButtonChecked(IDC_CHECK5);
	gss[12].val = IsDlgButtonChecked(IDC_CHECK7);
	gss[35].val = IsDlgButtonChecked(IDC_CHECK6);
	gss[40].val = SendDlgItemMessage(IDC_COMBO2, CB_GETCURSEL);
	gss[49].val = SendDlgItemMessage(IDC_COMBO3, CB_GETCURSEL);
	gss[50].val = IsDlgButtonChecked(IDC_CHECK8);
	gss[51].val = SendDlgItemMessage(IDC_COMBO4, CB_GETCURSEL);
	return CPropertyPage::OnKillActive();
}
