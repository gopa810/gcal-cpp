#include "GCEarthData.h"

#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgGetDate dialog

class DlgGetDate : public CDialog
{
// Construction
public:
	static VCTIME s_event;
	static int s_init;
	EARTHDATA m_earth;
	int m_nNextStep;
	DlgGetDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGetDate)
	enum { IDD = IDD_GET_DATE };
	UINT	m_nDay;
	UINT	m_nMonth;
	UINT	m_nYear;
	UINT	m_nHour;
	UINT	m_nMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGetDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGetDate)
	afx_msg void OnButtonNext();
	afx_msg void OnButtonBack();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

