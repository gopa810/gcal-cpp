// DlgMasaListing.cpp : implementation file
//

#include "stdafx.h"
#include "VCal5beta.h"
#include "DlgMasaListing.h"

#include "TString.h"
#include "TResultMasaList.h"
#include "GCUserInterface.h"


/////////////////////////////////////////////////////////////////////////////
// DlgMasaListing dialog

int DlgMasaListing::s_init = 0;
int DlgMasaListing::s_year = 0;
int DlgMasaListing::s_count = 0;


DlgMasaListing::DlgMasaListing(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMasaListing::IDD, pParent)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (s_init == 0)
	{
		m_Year = st.wYear;
		m_Count = 1;
		s_init = 1;
	}
	else
	{
		m_Year = s_year;
		m_Count = s_count;
	}

	//{{AFX_DATA_INIT(DlgMasaListing)
	//}}AFX_DATA_INIT
	bFinal = FALSE;
}


void DlgMasaListing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMasaListing)
	DDX_Control(pDX, IDC_PROGRESS1, m_prog);
	DDX_Text(pDX, IDC_EDIT1, m_Year);
	DDV_MinMaxInt(pDX, m_Year, 1500, 3999);
	DDX_Text(pDX, IDC_EDIT2, m_Count);
	DDV_MinMaxUInt(pDX, m_Count, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMasaListing, CDialog)
	//{{AFX_MSG_MAP(DlgMasaListing)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMasaListing message handlers

void DlgMasaListing::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	s_year = m_Year;
	s_count = m_Count;


	if (m_Count > 20)
		return;

	if (m_Year + m_Count > 3999)
	{
		m_Count = 4000 - m_Year;
	}

	m_pMasaList->CalculateMasaList(m_loc, m_Year, m_Count);

	if (GCUserInterface::ShowMode == 0)
	{
		m_pMasaList->formatText(m_strResult);
	}
	else if (GCUserInterface::ShowMode == 1)
	{
		m_pMasaList->formatRtf(m_strResult);
	}

	m_nNextStep = 1;
	CDialog::OnOK();
}


BOOL DlgMasaListing::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (bFinal)
		SetDlgItemText(IDC_BUTTON_NEXT, "Calculate");
	
	m_nNextStep = 0;
	m_strResult.Empty();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgMasaListing::OnButtonBack() 
{
	// TODO: Add your control notification handler code here
	m_nNextStep = -1;
	CDialog::OnCancel();
}
