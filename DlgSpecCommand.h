#pragma once
//

/////////////////////////////////////////////////////////////////////////////
// DlgSpecCommand dialog

class DlgSpecCommand : public CDialog
{
// Construction
public:
	void ExportStrings();
	void ExportEventsCocoa();
	void ExportTzCocoa();
	void ExportLocsCocoa();
	DlgSpecCommand(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgSpecCommand)
	enum { IDD = IDD_SPEC_COMMAND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSpecCommand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgSpecCommand)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

