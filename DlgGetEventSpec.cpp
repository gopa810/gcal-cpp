// DlgGetEventSpec.cpp : implementation file
//

#include "stdafx.h"
#include "vcal5beta.h"
#include "DlgGetEventSpec.h"
#include "level_6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGetEventSpec dialog


DlgGetEventSpec::DlgGetEventSpec(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGetEventSpec::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGetEventSpec)
	m_sun = FALSE;
	m_tithi = FALSE;
	m_naks = FALSE;
	m_sank = FALSE;
	m_conj = FALSE;
	//}}AFX_DATA_INIT
}


void DlgGetEventSpec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGetEventSpec)
	DDX_Check(pDX, IDC_CHECK1, m_sun);
	DDX_Check(pDX, IDC_CHECK2, m_tithi);
	DDX_Check(pDX, IDC_CHECK3, m_naks);
	DDX_Check(pDX, IDC_CHECK4, m_sank);
	DDX_Check(pDX, IDC_CHECK5, m_conj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGetEventSpec, CDialog)
	//{{AFX_MSG_MAP(DlgGetEventSpec)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGetEventSpec message handlers

void DlgGetEventSpec::OnButtonBack() 
{
	m_nNextStep = -1;
	CDialog::OnOK();
}

void DlgGetEventSpec::OnCancel() 
{
	m_nNextStep = 0;
	CDialog::OnCancel();
}

void DlgGetEventSpec::OnOK() 
{
	UpdateData();
	m_fOptions = 0;
	if (m_sun) m_fOptions |= CCE_SUN;
	if (m_naks) m_fOptions |= CCE_NAK;
	if (m_sank) m_fOptions |= CCE_SNK;
	if (m_tithi) m_fOptions |= CCE_TIT;
	if (m_conj) m_fOptions |= CCE_CNJ;

	m_fOptions |= m_fSort;

	m_nNextStep = 1;
	CDialog::OnOK();
}

void DlgGetEventSpec::OnRadio1() 
{
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	m_fSort = CCE_SORT;
}

void DlgGetEventSpec::OnRadio2() 
{
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	m_fSort = 0;
}

BOOL DlgGetEventSpec::OnInitDialog() 
{
	m_conj = (m_fOptions & CCE_CNJ) ? TRUE : FALSE;
	m_naks = (m_fOptions & CCE_NAK) ? TRUE : FALSE;
	m_sank = (m_fOptions & CCE_SNK) ? TRUE : FALSE;
	m_sun  = (m_fOptions & CCE_SUN) ? TRUE : FALSE;
	m_tithi = (m_fOptions & CCE_TIT) ? TRUE : FALSE;

	CDialog::OnInitDialog();

	m_fSort = m_fOptions & CCE_SORT;

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, (m_fSort ? IDC_RADIO1 : IDC_RADIO2));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
