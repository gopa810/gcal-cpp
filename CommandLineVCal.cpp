// CommandLineVCal.cpp: implementation of the CCommandLineVCal class.
// PORTABLE
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TString.h"
#include "CommandLineVCal.h"
#include "LocationRef.h"
#include "TResultApp.h"
#include "TResultCalendar.h"
#include "TResultEvents.h"
#include "TResultMasaList.h"
#include "TResultToday.h"
#include "GCCalendar.h"
#include "GCGlobal.h"
#include "GCTithi.h"
#include "GCSankranti.h"
#include "GCNaksatra.h"
#include "GCStrings.h"
#include "TTimeZone.h"
#include "GCUserInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandLineVCal::CCommandLineVCal()
{
	gnLastError = 0;
}

CCommandLineVCal::~CCommandLineVCal()
{

}

//////////////////////////////////////////////////////////////////////
//

int CCommandLineVCal::GetArgLastError()
{
	return gnLastError;
}

//////////////////////////////////////////////////////////////////////
//

int CCommandLineVCal::SetArgLastError(int i)
{
	return (gnLastError = i);
}

//////////////////////////////////////////////////////////////////////
//

int CCommandLineVCal::GetArg_Year(const char * str, int &nYear)
{
	int n = _ttoi(str);

	if (n < 1600)
	{
		return SetArgLastError(10);
	}

	if (n > 3999)
		return SetArgLastError(11);

	nYear = n;

	return NULL;

}

//////////////////////////////////////////////////////////////////////
//

int CCommandLineVCal::GetArg_Int(const char * str, int &nInteger)
{
	nInteger = _ttoi(str);

	return NULL;
}

//////////////////////////////////////////////////////////////////////
//

int CCommandLineVCal::GetArg_Masa(const char * str)
{
	int nMasa = 0;

	char * masaname[14] = 
	{
		"visnu",
		"madhusudana",
		"trivikrama",
		"vamana",
		"sridhara",
		"hrsikesa",
		"padmanadbha",
		"damodara",
		"kesava",
		"narayana",
		"madhava",
		"govinda",
		"purusottama"
	};


	for(int i = 0; i < 13; i++)
	{
		if (stricmp(masaname[i], str) == 0)
		{
			return MasaIndexToInternal(i);
		}
	}

	nMasa = atoi(str);

	if (nMasa != 0)
	{
		return MasaIndexToInternal(nMasa - 1);
	}

	return 11;
}

int CCommandLineVCal::MasaIndexToInternal(int nMasa)
{
	int nMasaIndex[] = { 11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12 };

	if (nMasa < 0)
		return 0;
	else if (nMasa > 12)
		return 0;
	else
		return nMasaIndex[ nMasa ];
}

int CCommandLineVCal::GetArg_EarthPos(const char * str, double &Latitude, double &Longitude)
{
	double l = 0.0;
	double sig = 1.0;
	double coma = 10.0;
	bool after_coma = false;
	bool is_deg = false;
	bool bNorth = false;
	bool bLat = false;
	bool bLon = false;

	const char * s = str;

	while(*s)
	{
		switch(*s)
		{
		case '0': case '1':
		case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9':
			if (after_coma)
			{
				if (is_deg)
				{
					l += (*s - '0') * 5.0 / (coma * 3.0);
				}
				else
				{
					l += (*s - '0') / coma;
				}
				coma *= 10.0;
			}
			else
			{
				l = l*10.0 + (*s - '0');
			}
			break;
		case 'n': case 'N':
			after_coma = true;
			is_deg = true;
			sig = 1.0;
			bNorth = true;
			break;
		case 's': case 'S':
			bNorth = true;
			after_coma = true;
			is_deg = true;
			sig = -1.0;
			break;
		case 'e': case 'E':
			bNorth = false;
			after_coma = true;
			is_deg = true;
			sig = 1.0;
			break;
		case 'w': case 'W':
			bNorth = false;
			after_coma = true;
			is_deg = true;
			sig = -1.0;
			break;
		case '.':
			after_coma = true;
			is_deg = false;
			break;
		case '-':
			sig = -1.0;
			break;
		case '+':
			sig = 1.0;
			break;
		case ';':
			if (bNorth == true)
			{
				bLat = true;
				Latitude = l * sig;
				bNorth = false;
			}
			else
			{
				bLon = true;
				Longitude = l * sig;
				bNorth = true;
			}
			l = 0.0;
			sig = 1.0;
			after_coma = false;
			is_deg = false;
			coma = 10.0;
			break;
		default:
			break;
		}
		s++;
	}

	if (bNorth == true)
	{
		bLat = true;
		Latitude = l * sig;
	}
	else
	{
		bLon = true;
		Longitude = l * sig;
	}

	return NULL;
}

int CCommandLineVCal::GetArg_TimeZone(const char * str, double &TimeZone)
{
	double l = 0.0;
	double sig = 1.0;
	double coma = 10.0;
	bool after_coma = false;
	bool is_deg = false;

	const char * s = str;

	while(*s)
	{
		switch(*s)
		{
		case '0': case '1':
		case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9':
			if (after_coma)
			{
				if (is_deg)
				{
					l += (*s - '0') * 5.0 / (coma * 3.0);
				}
				else
				{
					l += (*s - '0') / coma;
				}
				coma *= 10.0;
			}
			else
			{
				l = l*10.0 + (*s - '0');
			}
			break;
		case 'W':
			after_coma = true;
			sig = -1.0;
			is_deg = true;
			break;
		case 'E':
			after_coma = true;
			sig = 1.0;
			is_deg = true;
			break;
		case '-':
			//after_coma = true;
			sig = -1.0;
			break;
		case '+':
			//after_coma = true;
			sig = 1.0;
			break;
		case '.':
			after_coma = true;
			is_deg = false;
			break;
		case ':':
			after_coma = true;
			is_deg = true;
			break;
		default:
			return SetArgLastError(14);
		}
		s++;
	}

	TimeZone = l * sig;

	return NULL;
}

int CCommandLineVCal::GetArg_Tithi(const char * str)
{
	char * tithi[] = {
		"Pratipat",
		"Dvitiya",
		"Tritiya",
		"Caturthi",
		"Pancami",
		"Sasti",
		"Saptami",
		"Astami",
		"Navami",
		"Dasami",
		"Ekadasi",
		"Dvadasi",
		"Trayodasi",
		"Caturdasi",
		"Amavasya",
		"Pratipat",
		"Dvitiya",
		"Tritiya",
		"Caturthi",
		"Pancami",
		"Sasti",
		"Saptami",
		"Astami",
		"Navami",
		"Dasami",
		"Ekadasi",
		"Dvadasi",
		"Trayodasi",
		"Caturdasi",
		"Purnima"
	};

	int i;

	for(i = 0; i < 30; i++)
	{
		if (stricmp(tithi[i], str) == 0)
		{
			return i % 15;
		}
	}

	i = _ttoi(str);
	if (i > 0)
	{
		return i - 1;
	}

	return 0;
}

int CCommandLineVCal::GetArg_Paksa(const char * str)
{
	if ((str[0] == TCHAR('g')) || (str[0] == TCHAR('G')) || (str[0] == TCHAR('1')))
	{
		return 1;
	}

	return 0;
}

int CCommandLineVCal::GetArg_Date(const char * str, VCTIME &vc)
{
	char szt[32];
	TString s[4];
	int n, p, len;
	int c = 0;

	p = strlen(str);
	for(n = p - 1; n >= 0; n--)
	{
		if (str[n] == '-' || str[n]=='/' || n == 0)
		{
			if (n == 0)
				n--;
			len = p - n - 1;
			if (len > 30)
				len = 30;
			strncpy(szt, str + n + 1, len);
			szt[len] = 0;
			//TRACE2("###arg %d = %s\n", c, szt);
			s[c] = szt;
			c++;
			p = n;
		}
	}

	vc.day = atoi(s[2].c_str());
	if (vc.day == 0)
		vc.day = 1;
	vc.month = atoi(s[1].c_str());
	if (vc.month == 0)
		vc.month = 1;
	vc.year = atoi(s[0].c_str());

	return NULL;
}

int CCommandLineVCal::GetArg_VaisnDate(const char * str, VATIME &va)
{
	char szt[32];
	TString s[4];
	int n, p, len;
	int c = 0;

	p = strlen(str);
	for(n = p - 1; n >= 0; n--)
	{
		if (str[n] == '-' || str[n] == '/' || n == 0)
		{
			if (n == 0)
				n--;
			len = p - n - 1;
			if (len > 30)
				len = 30;
			strncpy(szt, str + n + 1, len);
			szt[len] = 0;
			//TRACE2("###arg %d = %s\n", c, szt);
			s[c] = szt;
			c++;
			p = n;
		}
	}

	va.tithi = GetArg_Tithi(s[3].c_str()) + GetArg_Paksa(s[2].c_str())*15;
	va.masa = GetArg_Masa(s[1].c_str());
	va.gyear = atoi(s[0].c_str());

	return NULL;
}


int CCommandLineVCal::GetArg_Interval(const char * pszText, int &A, int &B)
{
	int * p = &A;

	A = 0;
	B = 0;

	const TCHAR * t = pszText;

	while(t && *t)
	{
		if ((*t == TCHAR('-')) || (*t == TCHAR(':')))
		{
			p = &B;
		}
		else if (isdigit(TCHAR(*t)))
		{
			*p = (*p) * 10 + (*t - TCHAR('0'));
		}
		else
			return 1;
		t++;
	}

	if (A == 0)
		A = B;
	else if (B == 0)
		B = A;
	
	return 0;
}


int CCommandLineVCal::GetArg_Time(const char * str, VCTIME &vc)
{
	double l = 0.0;
	double sig = 1.0;
	double coma = 10.0;
	bool after_coma = false;
	bool is_deg = false;

	const char * s = str;

	while(*s)
	{
		switch(*s)
		{
		case '0': case '1':
		case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9':
			if (after_coma)
			{
				if (is_deg)
				{
					l += (*s - '0') * 5.0 / (coma * 3.0);
				}
				else
				{
					l += (*s - '0') / coma;
				}
				coma *= 10.0;
			}
			else
			{
				l = l*10.0 + (*s - '0');
			}
			break;
		case ':':
			after_coma = true;
			is_deg = true;
			break;
		default:
			return SetArgLastError(14);
		}
		s++;
	}

	vc.shour = l / 24.0;

	return NULL;
}

int CCommandLineVCal::ParseCommandArguments(const char * m_lpCmdLine)
{
	CCommandLineVCal * cmd = this;
	// if no arguments, then application should open window
	if (_tcslen(m_lpCmdLine) < 1)
		return FALSE;
	
	char * arg = new char[_tcslen(m_lpCmdLine) + 1];
	if (arg == NULL)
		return FALSE;

	const char * parse = m_lpCmdLine;
	int argp = 0;
	int argc = 0;
	char * args[64];
	char stopc;
	char * pw = arg;

	while(*parse)
	{
		while(*parse != 0 && *parse==' ')
			parse++;
		if (*parse)
		{
			args[argc] = pw;
			argc++;
		}
		stopc = (*parse == '\"') ? '\"' : ' ';
		if (stopc == '\"')
			parse++;
		while(*parse != 0 && *parse != stopc)
		{
			*pw = *parse;
			pw++;
			parse++;
		}
		if (*parse)
			parse++;
		*pw = 0;
		pw++;
	}

	CLocationRef loc;
	VCTIME vcStart, vcEnd;
	VATIME vaStart, vaEnd;
	int nCount;
	int nReq;
	TString str;
	TString strFileOut;
	FILE * fout = stdout;

	loc.m_fLatitude = 0.0;
	loc.m_fLongitude = 0.0;
	loc.m_fTimezone = 0.0;
	loc.m_nDST = 0;
	loc.m_strName = "";
	vcStart.day = 0;
	vcStart.month = 0;
	vcStart.year = 0;
	vcEnd = vcStart;
	vaStart.tithi = vaStart.masa = vaStart.gyear = 0;
	vaEnd = vaStart;
	nCount = -1;

	for(int i = 0; i < argc; i++)
	{
		//TRACE2("arg %d = %s\n", i, args[i]);
		if (strcmp(args[i],"-L") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_strLongitude = args[i+1];
				cmd->GetArg_EarthPos(args[i+1], loc.m_fLatitude, loc.m_fLongitude);
				//TRACE2("-L latitude=%f longitude=%f\n", loc.m_fLatitude, loc.m_fLongitude);
			}
			i++;
		}
		else if (strcmp(args[i],"-N") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_strName = args[i+1];
				//TRACE1("-N name=%s\n", loc.m_strName);
			}
			i++;
		}
		else if (strcmp(args[i],"-SV") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_VaisnDate(args[i+1], vaStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-SG") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Date(args[i+1], vcStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-ST") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Time(args[i+1], vcStart);
			}
			i++;
		}
		else if (strcmp(args[i],"-EG") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_Date(args[i+1], vcEnd);
				//AfxTrace("-EG day=%d month=%d year=%d\n", vcEnd.day, vcEnd.month, vcEnd.year);
			}
			i++;
		}
		else if (strcmp(args[i],"-EV") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_VaisnDate(args[i+1], vaEnd);
				//AfxTrace("-EV tithi=%d masa=%d gyear=%d\n", vaEnd.tithi, vaEnd.masa, vaEnd.gyear);
			}
			i++;
		}
		else if (strcmp(args[i],"-EC") == 0)
		{
			if (argc >= i+2)
			{
				nCount = atoi(args[i+1]);
				//AfxTrace("couint = %d\n", nCount);
			}
			i++;
		}
		else if (strcmp(args[i],"-TZ") == 0)
		{
			if (argc >= i+2)
			{
				cmd->GetArg_TimeZone(args[i+1], loc.m_fTimezone);
				//TRACE1("-TZ  timezone=%f\n", loc.m_fTimezone);
			}
			i++;
		}
		else if (strcmp(args[i],"-LNG") == 0)
		{
			if (argc >= i+2)
			{
				TString strLang;
				strLang = args[i+1];
				TString strFile;
				if (GCGlobal::GetLangFileForAcr(strLang, strFile) == TRUE)
				{
					GCStrings::InitLanguageOutputFromFile(strFile);
				}
			}
			i++;
		}
		else if (strcmp(args[i],"-DST") == 0)
		{
			if (argc >= i+2)
			{
				loc.m_nDST = TTimeZone::GetID(args[i+1]);
			}
			i++;
		}
		else if (strcmp(args[i],"-O") == 0)
		{
			if (argc >= i+2)
			{
				if (fout != stdout && fout != stderr)
					fclose(fout);
				fout = fopen(args[i+1], "wt");
				//strFileOut = args[i+1];
				if (fout == NULL)
					fout = stderr;
			}
			i++;
		}
		else if (stricmp(args[i],"-R") == 0)
		{
			if (argc >= i+2)
			{
				if (stricmp(args[i + 1], "calendar") == 0)
				{
					nReq = 10;
				} else if (stricmp(args[i + 1], "appday") == 0)
				{
					nReq = 11;
				} else if (stricmp(args[i + 1], "tithi") == 0)
				{
					nReq = 12;
				} else if (stricmp(args[i + 1], "sankranti") == 0)
				{
					nReq = 13;
				} else if (stricmp(args[i + 1], "naksatra") == 0)
				{
					nReq = 14;
				} else if (stricmp(args[i + 1], "firstday") == 0)
				{
					nReq = 15;
				} else if (stricmp(args[i + 1], "gcalendar") == 0)
				{
					nReq = 16;
				} else if (stricmp(args[i + 1], "gtithi") == 0)
				{
					nReq = 17;
				} else if (stricmp(args[i + 1], "next") == 0)
				{
					nReq = 18;
				} else if (stricmp(args[i + 1], "xlan") == 0)
				{
					nReq = 50;
				}
				else if (stricmp(args[i + 1], "help") == 0)
				{
					nReq = 60;
				}
				/*else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				} else if (stricmp(args[i + 1], "") == 0)
				{
				}*/
			}
			i++;
		}
	}

	loc.m_strFullName.Format("%s (%s %s, %s)", loc.m_strName.c_str(), loc.m_strLatitude.c_str(), 
		loc.m_strLongitude.c_str(), loc.m_strTimeZone.c_str());
	vcStart.tzone = loc.m_fTimezone;
	vcEnd.tzone = loc.m_fTimezone;
	//AfxTrace("vcStart = %d,%d,%d,...,%f\n", vcStart.day, vcStart.month, vcStart.year, vcStart.shour);

	switch(nReq)
	{
	case 10:
	case 13:
	case 14:
		if (vcStart.year == 0 && vaStart.gyear != 0)
			GCCalendar::VATIMEtoVCTIME(vaStart, vcStart, (EARTHDATA)loc);
		if (vcEnd.year == 0 && vaEnd.gyear != 0)
			GCCalendar::VATIMEtoVCTIME(vaEnd, vcEnd, (EARTHDATA)loc);		
		break;
	default:
		break;
	}

	if (vcStart.year != 0 && vcEnd.year != 0 && nCount < 0)
		nCount = int(vcEnd.GetJulian() - vcStart.GetJulian());

	if (nCount < 0)
		nCount = 30;

	TResultApp appday;
	TResultCalendar calendar;
	//AfxTrace("Count === %d\n", nCount);

	switch(nReq)
	{
	case 10:
		// -R -O -LAT -LON -SG -C [-DST -NAME]
		vcStart.NextDay();
		vcStart.PreviousDay();
		GCUserInterface::CalculateCalendar(calendar, loc, vcStart, nCount);
		calendar.writeXml(fout);
		break;
	case 11:
		// -R -O -LAT -LON -SG -ST [-NAME]
		appday.calculateAppDay(loc, vcStart);
		appday.formatXml(str);
		fputs(str, fout);
		break;
	case 12:
		GCTithi::writeXml( fout, loc, vcStart);
		break;
	case 13:
		if (vcEnd.year == 0)
		{
			vcEnd = vcStart;
			vcEnd += nCount;
		}
		GCSankranti::writeXml(fout, loc, vcStart, vcEnd);
		break;
	case 14:
		GCNaksatra::writeXml(fout, loc, vcStart, nCount);
		break;
	case 15:
		GCCalendar::writeFirstDayXml(fout, loc, vcStart);
		break;
	case 16:
		vcStart = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, vcStart.year);
		vcEnd = DAYDATA::GetFirstDayOfYear((EARTHDATA)loc, vcStart.year + 1);
		nCount = int(vcEnd.GetJulian() - vcStart.GetJulian());
		GCUserInterface::CalculateCalendar(calendar, loc, vcStart, nCount);
		calendar.writeXml(fout);
		break;
	case 17:
		GCTithi::writeGaurabdaTithiXml( fout, loc, vaStart, vaEnd);
		break;
	case 18:
		GCTithi::writeGaurabdaNextTithiXml( fout, loc, vcStart, vaStart);
		break;
	case 50:
		{
			int i = 0;
			TString str;
			fputs("10000\nEnglish\n10001\nen\n", fout);
			// export obsahu do subora
			for(i = 0; i < 900; i ++)
			{
				if (!GCStrings::getString(i).IsEmpty())
				{
					str.Format("%d\n%s\n", i, GCStrings::getString(i).c_str());
					fputs(str, fout);
				}
			}
		}
		break;
	}
	// application should be windowless
	// since some parameters are present
	delete [] arg;

	return TRUE;
}
