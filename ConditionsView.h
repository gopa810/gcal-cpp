#pragma once

#include "LocationRef.h"


/////////////////////////////////////////////////////////////////////////////
// CConditionsView window

class CConditionsView : public CWnd
{
// Construction
public:
	CConditionsView();

// Attributes
public:
	NONCLIENTMETRICS ncm;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionsView)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetCountYear();
	int GetStartYear();
	int m_dst;
	int lastYposition;
	CLocationRef m_earth;
	CFont m_infoFont;
	CEdit m_edits[4];
	CButton m_buttons[3];
	CStatic m_wnd_texts[5];

	CFont m_comboFont;
	int GetProperHeight();
	virtual ~CConditionsView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionsView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

