// EventsHtmlView.cpp: implementation of the CResultHtmlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vcal5beta.h"
#include "EventsHtmlView.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResultHtmlView::CResultHtmlView()
{

}

CResultHtmlView::~CResultHtmlView()
{

}

void CResultHtmlView::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags,
			LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, 
			LPCTSTR lpszHeaders, BOOL* pbCancel )
{
	int a;

	a = 0;

	if (strcmp(lpszURL,"action:list")==0)
	{
		*pbCancel = TRUE;
	}
}
