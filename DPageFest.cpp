// DPageFest.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "DPageFest.h"
#include "showset.h"
#include "CustomEvent.h"
extern CShowSetting gss[];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_set_oldstylefast = 0;

extern int gCustomEventList_Modified;

/////////////////////////////////////////////////////////////////////////////
// DPageFest property page

IMPLEMENT_DYNCREATE(DPageFest, CPropertyPage)

DPageFest::DPageFest() : CPropertyPage(DPageFest::IDD)
{
	//{{AFX_DATA_INIT(DPageFest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPageFest::~DPageFest()
{
}

void DPageFest::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPageFest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPageFest, CPropertyPage)
	//{{AFX_MSG_MAP(DPageFest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPageFest message handlers

void DPageFest::OnOK() 
{
	CPropertyPage::OnOK();
}

BOOL DPageFest::OnSetActive() 
{
	int i;
	SendDlgItemMessage(IDC_COMBO1, CB_RESETCONTENT);
	SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(const char *)("Purnima System"));
	SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(const char *)("Pratipat System"));
	SendDlgItemMessage(IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(const char *)("Ekadasi System"));
	if (gss[13].val == 1)
		i = 0;
	else if (gss[14].val == 1)
		i = 1;
	else
		i = 2;
	SendDlgItemMessage(IDC_COMBO1, CB_SETCURSEL, i);

	CheckDlgButton(IDC_CHECK2, gss[16].val);
	CheckDlgButton(IDC_CHECK3, gss[17].val);

	int chks[10][2];
	for(i=0;i<10;i++)
		chks[i][0] = chks[i][1] = 0;
	CCustomEvent * p = gCustomEventList.list;
	while(p)
	{
		chks[p->nClass][p->nVisible]++;
		p = p->next;
	}
	for(i=0;i<6;i++)
	{
		if (chks[i][1]>0 && chks[i][0]==0)
		{
			gss[22+i].val=1;
		}
		else if (chks[i][1]==0 && chks[i][0]>0)
		{
			gss[22+i].val=0;
		}
		else
		{
			gss[22+i].val=2;
		}
	}
	CheckDlgButton(IDC_CHECK4, gss[22].val);
	CheckDlgButton(IDC_CHECK5, gss[23].val);
	CheckDlgButton(IDC_CHECK6, gss[24].val);
	CheckDlgButton(IDC_CHECK7, gss[25].val);
	CheckDlgButton(IDC_CHECK8, gss[26].val);
	CheckDlgButton(IDC_CHECK9, gss[27].val);
	CheckDlgButton(IDC_CHECK10, gss[28].val);
	CheckDlgButton(IDC_CHECK1,  gss[36].val);
	CheckDlgButton(IDC_CHECK11, gss[37].val);
	CheckDlgButton(IDC_CHECK12, gss[38].val);
	CheckDlgButton(IDC_CHECK13, gss[39].val);
	CheckDlgButton(IDC_CHECK14, gss[42].val);
	CheckDlgButton(IDC_CHECK15, gss[41].val);

	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

BOOL DPageFest::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	LPNMHDR lp = (LPNMHDR)lParam;

	if (lp->code == PSN_HELP)
	{
		MessageBox("This page allows to change the mode of display of the most common events in the calendar.", "Settings for Events", MB_OK);
		return TRUE;
	}
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

BOOL DPageFest::OnKillActive() 
{
	int i = 0;
	// TODO: Add your specialized code here and/or call the base class
	i = SendDlgItemMessage(IDC_COMBO1, CB_GETCURSEL);
	gss[16].val = IsDlgButtonChecked(IDC_CHECK2);
	gss[17].val = IsDlgButtonChecked(IDC_CHECK3);
	
	gss[22].val = IsDlgButtonChecked(IDC_CHECK4);
	gss[23].val = IsDlgButtonChecked(IDC_CHECK5);
	gss[24].val = IsDlgButtonChecked(IDC_CHECK6);
	gss[25].val = IsDlgButtonChecked(IDC_CHECK7);
	gss[26].val = IsDlgButtonChecked(IDC_CHECK8);
	gss[27].val = IsDlgButtonChecked(IDC_CHECK9);
	gss[28].val = IsDlgButtonChecked(IDC_CHECK10);
	gss[36].val = IsDlgButtonChecked(IDC_CHECK1);
	gss[37].val = IsDlgButtonChecked(IDC_CHECK11);
	gss[38].val = IsDlgButtonChecked(IDC_CHECK12);
	gss[39].val = IsDlgButtonChecked(IDC_CHECK13);
	gss[41].val = IsDlgButtonChecked(IDC_CHECK15);
	gss[42].val = IsDlgButtonChecked(IDC_CHECK14);

	if (i >= 0 && i < 3)
	{
		gss[13 + i].val = 1;
		gss[13 + (i + 1)%3].val = 0;
		gss[13 + (i + 2)%3].val = 0;
	}
	
	CCustomEvent * p = gCustomEventList.list;
	while(p)
	{
		i = p->nClass;
		if (gss[22+i].val == 0)
		{
			p->nVisible = 0;
			gCustomEventList_Modified = 1;
		}
		else if (gss[22+i].val == 1)
		{
			p->nVisible = 1;
			gCustomEventList_Modified = 1;
		}
		p = p->next;
	}

	gCustomEventList_Modified = (gCustomEventList.setOldStyleFast(gss[42].val) ? 1 : gCustomEventList_Modified);

	return CPropertyPage::OnKillActive();
}

BOOL DPageFest::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CPropertyPage::OnCommand(wParam, lParam);
}
