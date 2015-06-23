#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgCustomEventDialog dialog

class DlgCustomEventDialog : public CDialog
{
// Construction
public:
	DlgCustomEventDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCustomEventDialog)
	enum { IDD = IDD_CUSTOM_EVENTS };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCustomEventDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCustomEventDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddEvent();
	afx_msg void OnRemoveEvent();
	virtual void OnOK();
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

