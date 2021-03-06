#pragma once

#include "TString.h"
#include "locationref.h"
#include "StaticHeader1.h"

/////////////////////////////////////////////////////////////////////////////
// DlgGetLocation dialog

class DlgGetLocation : public CDialog
{
// Construction
public:
	int m_nLastDst;
	int SetCurSelDST(int nID);
	bool b_upd;
	double m_longSign;
	double m_latiSign;
	CLocationRef m_location;

	int UpdateDstByTimezone(double tzone);
	TString m_strTitle;
	BOOL m_bFinal;
	void UpdateDSTInfo();
	void GetEarthData(EARTHDATA &earth);
	void LocationToDialog();
	BOOL m_bMyLocation;
	int m_nNextStep;
	BOOL wrongCoordinates;
	const char * errorText;
	DlgGetLocation(LPCTSTR szTitle, CWnd* pParent = NULL);   // standard constructor
	int LocGetDeg(double d);
	int LocGetArc(double d);

// Dialog Data
	//{{AFX_DATA(DlgGetLocation)
	enum { IDD = IDD_GET_LOCATION };
	CButton	cLongSw;
	CButton	cLatiSw;
	CEdit	cLongDeg;
	CEdit	cLongArc;
	CEdit	cLatiDeg;
	CEdit	cLatiArc;
	CComboBox	cDst;
	CEdit	cLocName;
	CEdit	cDstInfo;
	CStaticHeader1	m_hdr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGetLocation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGetLocation)
	afx_msg void OnSelectLocation();
	afx_msg void OnButtonBack();
	afx_msg void OnButtonNext();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboDst();
	afx_msg void OnDropdownComboDst();
	afx_msg void OnBtnLong();
	afx_msg void OnBtnLati();
	afx_msg void OnChangeLatiArc();
	afx_msg void OnChangeLatiDeg();
	afx_msg void OnChangeLongArc();
	afx_msg void OnChangeLongDeg();
	afx_msg void OnChangeLocation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

