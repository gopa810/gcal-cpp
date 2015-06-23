// CustomEvent.h: interface for the CCustomEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "TString.h"
#include "NSMutableArray.h"

//==============================================================
//
//==============================================================

class CCustomEvent  
{
public:
	CCustomEvent();
	virtual ~CCustomEvent();

	int nTithi;
	int nMasa;
	int nClass;
	int nFastType;
	int nVisible;
	int nStartYear;
	short int nUsed;
	short int nDeleted;
	short int nSpec;
	TString strFastSubject;
	TString strText;
};

class CCustomEventList
{
public:
	static void Export(const char * pszFile, int format);
	static int OpenFile(const char * pszFile);
	static int SaveFile(const char * pszFile);
	static int Count(void);
	static CCustomEvent * EventAtIndex(int index);
	static int SetOldStyleFasting(int i);

	static NSMutableArray<CCustomEvent> list;

public:
	CCustomEventList() { }
	~CCustomEventList() { clear(); };

	CCustomEvent * add(void);
	void clear();
};


