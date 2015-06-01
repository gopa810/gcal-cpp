// XmlFile.h: interface for the TFileXml class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLFILE_H__58C81085_95B4_4F0A_9E95_6F2AFD75ADA0__INCLUDED_)
#define AFX_XMLFILE_H__58C81085_95B4_4F0A_9E95_6F2AFD75ADA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PORTABLE

#include "level_0.h"
;
class TFileXml  
{
	FILE * fout;
public:
	void operator=(FILE * file) { fout = file; }
	TFileXml & operator<<(VCTIME vc);
	TFileXml & operator<<(TString &s);
	TFileXml & operator<<(const char * s);
	TFileXml & operator<<(DAYTIME dt);
	TFileXml & operator<<(int n);
	TFileXml & operator<<(double d);
	TFileXml & operator<<(TCHAR * p);
	TFileXml();
	virtual ~TFileXml();

};

#endif // !defined(AFX_XMLFILE_H__58C81085_95B4_4F0A_9E95_6F2AFD75ADA0__INCLUDED_)
