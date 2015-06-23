#pragma once

#include "staticheader3.h"

/////////////////////////////////////////////////////////////////////////////
// DlgEventsGaubdTM dialog

class DlgEventsGaubdTM : public CDialog
{
// Construction
public:
	int m_nRetCancel;
	int m_nMasa;
	int m_nTithi;
	DlgEventsGaubdTM(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgEventsGaubdTM)
	enum { IDD = IDD_GET_GAUBD_TM };
	CStaticHeader3	m_s2;
	CStaticHeader3	m_s1;
	CListBox	m_list2;
	CListBox	m_list1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEventsGaubdTM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgEventsGaubdTM)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

