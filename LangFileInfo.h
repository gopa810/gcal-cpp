#pragma once

#include "TString.h"

class TLangFileInfo  
{
public:
	TString m_strAcr;
	TString m_strLang;
	TString m_strFile;
	TLangFileInfo * next;
	TLangFileInfo(){ next = NULL; };
	virtual ~TLangFileInfo(){};
	void decode(const char * p)
	{
		m_strAcr.Empty();
		m_strLang.Empty();
		m_strFile.Empty();
		int ndest = 0;
		if (p == NULL) return;
		while(*p)
		{
			if (*p == '#')
				ndest++;
			else
			{
				switch(ndest)
				{
				case 0:
					m_strAcr += *p;
					break;
				case 1:
					m_strLang += *p;
					break;
				default:
					m_strFile += *p;
					break;
				}
			}
		}
	}
	void encode(TString &str)
	{
		str = m_strAcr;
		str += "#";
		str += m_strLang;
		str += "#";
		str += m_strFile;
	}

};

class TLangFileList
{
public:
	TLangFileInfo * list;
	TLangFileInfo * add()
	{
		if (list == NULL)
		{
			list = new TLangFileInfo;
			return list;
		}
		TLangFileInfo * p = list;
		while(p->next)
			p = p->next;
		p->next = new TLangFileInfo;
		return p->next;
	}
	void clear()
	{
		TLangFileInfo * p, * r;
		p = list;
		while(p)
		{
			r = p->next;
			delete p;
			p = r;
		}
		list = NULL;
	}
	TLangFileList() { list = NULL; }
	~TLangFileList() { clear(); }
};

