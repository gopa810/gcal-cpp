
#pragma once


// PORTABLE
#include "GCEarthData.h"
#include "GCStrings.h"

class CLocationRef  
{
public:
	TString m_strTimeZone;
	TString m_strLatitude;
	TString m_strLongitude;
	TString m_strName;
	TString m_strFullName;
	double  m_fLongitude;
	double  m_fLatitude;
	double  m_fTimezone;
	int     m_nDST;
	
	CLocationRef() {};
	virtual ~CLocationRef() {};
	operator EARTHDATA() {
		EARTHDATA e; 
		e.dst = m_nDST; 
		e.latitude_deg = m_fLatitude; 
		e.longitude_deg = m_fLongitude;
		e.tzone = m_fTimezone; return e;
	};
	EARTHDATA operator=(EARTHDATA e) { 
		m_fLongitude = e.longitude_deg; 
		m_fLatitude = e.latitude_deg; 
		m_fTimezone = e.tzone; 
		m_nDST = e.dst; return e;
	};
	
	void Set(CLocationRef & L)
	{
		m_strTimeZone = L.m_strTimeZone;
		m_strLatitude = L.m_strLatitude;
		m_strLongitude = L.m_strLongitude;
		m_strName = L.m_strName;
		m_strFullName = L.m_strFullName;
		m_fLongitude = L.m_fLongitude;
		m_fLatitude = L.m_fLatitude;
		m_fTimezone = L.m_fTimezone;
		m_nDST = L.m_nDST;
	}

	void GetFullName(TString * target)
	{
		if (target != NULL)
		{
			target->Format("%s (%s, %s, %s: %s)"
				, m_strName.c_str()
				, m_strLatitude.c_str()
				, m_strLongitude.c_str()
				, GCStrings::getString(12).c_str()
				, m_strTimeZone.c_str());
		}
	}
		private:
	void operator=(CLocationRef&);
};

