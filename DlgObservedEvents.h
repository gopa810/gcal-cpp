#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgObservedEvents dialog

class DlgObservedEvents : public CDialog
{
// Construction
public:
	DlgObservedEvents(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgObservedEvents)
	enum { IDD = IDD_OBSERVED_EVENT };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgObservedEvents)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgObservedEvents)
	afx_msg void OnExport();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

