#include "TString.h"	// Added by ClassView

#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgGetEventSpec dialog

class DlgGetEventSpec : public CDialog
{
// Construction
public:
	unsigned int m_fSort;
	unsigned int m_fOptions;
	int m_nNextStep;
	DlgGetEventSpec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGetEventSpec)
	enum { IDD = IDD_CALCEVENTS_TYPES };
	BOOL	m_sun;
	BOOL	m_tithi;
	BOOL	m_naks;
	BOOL	m_sank;
	BOOL	m_conj;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGetEventSpec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGetEventSpec)
	afx_msg void OnButtonBack();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck6();
	BOOL m_rahu_kalam;
	BOOL m_yama_ghanti;
	BOOL m_guli_kalam;
	BOOL m_moon_rasi;
	BOOL m_moon_times;
	BOOL m_ascendent;
//	CString m_off;
//	double m_off;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

