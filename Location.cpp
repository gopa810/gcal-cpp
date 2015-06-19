// Location.cpp: implementation of the CLocation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCal5beta.h"
#include "Location.h"
#include "level_0.h"
#include "TFile.h"
#include "TTimeZone.h"
#include "TCountry.h"
#include "TLocation.h"
#include "TFileRichList.h"
#include "GCGlobal.h"

// PORTABLE
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocation::CLocation()
{
	m_fLatitude = 0.0;
	m_fLongitude = 0.0;
	m_fTimezone = 0.0;
	m_nTimezoneId = 0;
}

CLocation::~CLocation()
{

}


void CLocation::Empty()
{
	m_strCountry.Empty();
	m_strCity.Empty();
	m_fLatitude = 0.0;
	m_fLongitude = 0.0;
	m_fTimezone = 0.0;
	m_nTimezoneId = 0;
}


