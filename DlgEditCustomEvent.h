#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgEditCustomEvent dialog

class DlgEditCustomEvent : public CDialog
{
// Construction
public:
	DlgEditCustomEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgEditCustomEvent)
	enum { IDD = IDD_EDIT_CUSTOM_EVENT };
	int		m_nTithi;
	int		m_nMasa;
	CString	m_strText;
	int		m_nClass;
	int		m_nFastType;
	CString	m_strFastSubject;
	CString m_strStartYear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEditCustomEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgEditCustomEvent)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

