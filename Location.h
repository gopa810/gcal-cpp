// PORTABLE
// Location.h: interface for the CLocation class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


class CLocation  
{
public:
	void SetTextB(TString &str);
	void SetTextA(TString &str);
	void Empty();
	TString m_strCountry;
	TString m_strCity;
	double m_fLongitude;
	double m_fLatitude;
	double m_fTimezone;
	int m_nDST;
	CLocation * next;
	CLocation * prev;

	CLocation();
	virtual ~CLocation();

};

class CLocationList
{
	CLocation * list;

public:
	int RenameCountry(const char * pszOld, const char * pszNew);
	BOOL InitList(const char * pszList);
	BOOL InitListX(const char * pszList);
	BOOL m_bModified;
	void AddTail(CLocation * lc);
	void RemoveAll();
	void RemoveAt(CLocation * rem);
	BOOL SaveAs(const char * lpszFileName, int nType);
	CLocation * GetHeadPosition();
	CMapStringToString m_mapCC;
	BOOL Analyze(const char * psz, CLocation &lc);
	BOOL ImportFile(const char * pszFile, BOOL bDeleteCurrent);
	void InitInternal();
	CLocationList();
	virtual ~CLocationList();
};


