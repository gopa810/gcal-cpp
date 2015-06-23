#pragma once

#include "LocationRef.h"
#include "TResultMasaList.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// DlgMasaListing dialog

class DlgMasaListing : public CDialog
{
// Construction
public:
	TResultMasaList * m_pMasaList;
	static int s_init, s_year, s_count;

	BOOL bFinal;
	int m_nNextStep;
	TString m_strResult;
	CLocationRef m_loc;
	DlgMasaListing(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgMasaListing)
	enum { IDD = IDD_MASALIST };
	CProgressCtrl	m_prog;
	int		m_Year;
	UINT	m_Count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgMasaListing)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgMasaListing)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

