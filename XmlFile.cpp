// XmlFile.cpp: implementation of the TFileXml class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlFile.h"
// PORTABLE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TFileXml::TFileXml()
{

}

TFileXml::~TFileXml()
{

}

TFileXml & TFileXml::operator<<(TCHAR *p)
{
	fputs(p, fout);
	return * this;
}

TFileXml & TFileXml::operator<<(double d)
{
	TString s;
	s.Format("%+.5f", d);
	fputs(s, fout);
	return * this;
}

TFileXml & TFileXml::operator<<(int n)
{
	TString s;
	s.Format("%d", n);
	fputs(s, fout);
	return * this;
}

TFileXml & TFileXml::operator<<(DAYTIME dt)
{
	TString s;
	if (dt.hour >= 0)
		s.Format("%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
	else
		s = "N/A";
	fputs(s, fout);
	return * this;	
}

TFileXml & TFileXml::operator<<(TString &s)
{
	fputs(s, fout);
	return * this;
}

TFileXml & TFileXml::operator<<(const char * s)
{
	fputs(s, fout);
	return * this;
}

TFileXml & TFileXml::operator<<(VCTIME vc)
{
	TString s;
	s.Format("%d %s %d", vc.day, AvcGetMonthAbr(vc.month), vc.year);
	fputs(s, fout);
	return * this;
}
