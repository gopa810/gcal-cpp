#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgRenameCountry dialog

class DlgRenameCountry : public CDialog
{
// Construction
public:
	int nSelected;
	DlgRenameCountry(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgRenameCountry)
	enum { IDD = IDD_RENAME_COUNTRY };
	CEdit	m_Edit;
	CListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgRenameCountry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgRenameCountry)
	afx_msg void OnSaveEdit();
	afx_msg void OnSelChange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

