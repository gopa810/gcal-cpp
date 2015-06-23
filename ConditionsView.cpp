// ConditionsView.cpp : implementation file
//

#include "stdafx.h"
#include "TString.h"
#include "vcal5beta.h"
#include "ConditionsView.h"
#include "locationref.h"
#include "dlggetlocation.h"
#include "CustomEvent.h"
#include "GCStrings.h"
#include "GCGlobal.h"

/////////////////////////////////////////////////////////////////////////////
// CConditionsView

CConditionsView::CConditionsView()
{
	memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
}

CConditionsView::~CConditionsView()
{
}


BEGIN_MESSAGE_MAP(CConditionsView, CWnd)
	//{{AFX_MSG_MAP(CConditionsView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionsView message handlers

int CConditionsView::GetProperHeight()
{
	return lastYposition;
}

int CConditionsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//int i, j, n;
	CRect rcClient;
	TString str;
	int ylev = 2;
	int basicHeight = ncm.iCaptionHeight;
	GetClientRect(&rcClient);

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_comboFont.CreateFontIndirect(&(ncm.lfMenuFont));
	m_infoFont.CreateFontIndirect(&(ncm.lfCaptionFont));
	
	m_wnd_texts[3].Create("Text to find:", WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(4, ylev, 300, ylev + basicHeight), this);
	m_wnd_texts[3].SetFont(&m_infoFont);
	ylev += basicHeight + 8;

	m_edits[3].Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 
		CRect(4, ylev, 300, ylev + basicHeight), this, IDC_EDIT4);
	m_edits[3].SetFont(&m_comboFont);

	// button FIND
	m_buttons[1].Create("Find", WS_VISIBLE | WS_CHILD | BS_FLAT, 
		CRect(312, ylev, 376, ylev + basicHeight), this, 131);
	m_buttons[1].SetFont(&m_comboFont);



	ylev += basicHeight + 20;


	CRect cr;
	SetFont(&m_comboFont);
	const char * pszTitle;
	CSize cs;
	CDC * pdc = GetDC();

	// row 1
	// button for setting location
	cr.SetRect(4, ylev, 100, ylev + basicHeight);
	m_buttons[0].Create("Set Location", WS_VISIBLE | WS_CHILD | BS_FLAT, cr, this, 130);
	m_buttons[0].SetFont(&m_comboFont);

	// location
	cr.left = 120;
	cr.right = 800;
	m_edits[0].Create(WS_VISIBLE | WS_CHILD | ES_READONLY, cr, this, 132);
	m_edits[0].SetFont(&m_infoFont);
	m_earth = (EARTHDATA)GCGlobal::lastLocation;
	m_dst = GCGlobal::lastLocation.m_nDST;

	GCGlobal::lastLocation.GetFullName(&str);
	m_edits[0].SetWindowText(str);
	ylev += basicHeight + 8;

	// row 2
	// start year
	cr.SetRect(4, ylev, 4, ylev + basicHeight);
	pszTitle = "Start Year:";
	cs = pdc->GetTextExtent(pszTitle, _tcslen(pszTitle));
	cr.right += cs.cx + 4;
	m_wnd_texts[0].Create(pszTitle, WS_VISIBLE | WS_CHILD | SS_CENTER, cr, this);
	m_wnd_texts[0].SetFont(&m_comboFont);
	cr.left = cr.right;

	// EDIT START YEAR
	SYSTEMTIME st;
	GetLocalTime(&st);
	str.Format("%d", st.wYear);
	cr.right = cr.left + 58;
	m_edits[1].Create(WS_BORDER | WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_RIGHT, cr, this, 133);
	m_edits[1].SetFont(&m_comboFont);
	m_edits[1].SetWindowText(str);
	cr.left = cr.right + 4;

	// title count....
	pszTitle = "Count of Years:";
	cs = pdc->GetTextExtent(pszTitle, _tcslen(pszTitle));
	cr.right += cs.cx + 4;
	m_wnd_texts[1].Create(pszTitle, WS_VISIBLE | WS_CHILD | SS_CENTER, cr, this);
	m_wnd_texts[1].SetFont(&m_comboFont);
	cr.left = cr.right;

	// EDIT COUNT YEARS
	cr.right = cr.left + 58;
	m_edits[2].Create(WS_BORDER | WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_RIGHT, cr, this, 134);
	m_edits[2].SetFont(&m_comboFont);
	m_edits[2].SetWindowText("2");
	cr.left = cr.right + 4;

	lastYposition = ylev + basicHeight + 8;
	return 0;
}

BOOL CConditionsView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//UINT nRow, nCol, n;
	CString str;

	if (HIWORD(wParam) == BN_CLICKED)
	{
		// stlacil tlacidlo
		if (LOWORD(wParam) == 130)
		{
			// set location
			DlgGetLocation dlg(GCStrings::getString(110));
			dlg.m_bFinal = TRUE;
			TString str;

			if (dlg.DoModal() == IDOK)
			{
				m_earth = (EARTHDATA)GCGlobal::lastLocation;
				m_dst = GCGlobal::lastLocation.m_nDST;
				GCGlobal::lastLocation.GetFullName(&str);
				m_edits[0].SetWindowText(str);
			}
		}
		else if (LOWORD(wParam) == 131)
		{
			// find
			GetParent()->PostMessage(WM_COMMAND, ID_EVENT_FIND);
		}
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

int CConditionsView::GetStartYear()
{
	CString str;

	m_edits[1].GetWindowText(str);
	return _ttoi(str);
}

int CConditionsView::GetCountYear()
{
	CString str;

	m_edits[2].GetWindowText(str);
	return _ttoi(str);
}

void CConditionsView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}
