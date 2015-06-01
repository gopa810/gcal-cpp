#include "stdafx.h"
#include "level_0.h"
#include "strings.h"


int GetShowSetVal(int i);

// PORTABLE

const char * GetTithiName(int i)
{
	return gstr[600 + i % 30];
}

const char * GetNaksatraName(int n)
{
	return gstr[630 + n % 27];
}

const char * GetNaksatraChildSylable(int n, int pada)
{
	int i = (n * 4 + pada) % 108;

	static char * childsylable[108] = {
		"chu", "che", "cho", "la", //asvini
		"li", "lu", "le", "lo", // bharani
		"a", "i", "u", "e", //krtika
		"o", "va", "vi", "vo", // rohini
		"ve", "vo", "ka", "ke", // mrgasira
		"ku","gha","ng","chha", // ardra
		"ke","ko","ha","hi", // punarvasu
		"hu","he","ho","da", // pushya
		"di","du","de","do", //aslesa
		"ma","mi","mu","me", //magha
		"mo","ta","ti","tu", //purvaphalguni
		"te","to","pa","pi", //uttaraphalguni
		"pu","sha","na","tha",//hasta
		"pe","po","ra","ri",//chitra
		"ru","re","ra","ta",//svati
		"ti","tu","te","to",//visakha
		"na","ni","nu","ne",// anuradha
		"no","ya","yi","yu",//jyestha
		"ye","yo","ba","bi",// mula
		"bu","dha","bha","dha",//purvasada
		"be","bo","ja","ji",// uttarasada
		"ju","je","jo","gha",//sravana
		"ga","gi","gu","ge",// dhanistha
		"go","sa","si","su",//satabhisda
		"se","so","da","di",//purvabhadra
		"du","tha","jna","da",//uttarabhadra
		"de","do","cha","chi"// revati

	};

	return childsylable[i];
}

const char * GetRasiChildSylable(int n)
{
	static char * childsylable[12] = {
		"a.. e.. la..",
		"u.. ba.. va..",
		"ka.. gha..",
		"da.. ha..",
		"ma..",
		"pa..",
		"ra.. ta..",
		"na.. ya..",
		"dha.. bha... pha..",
		"kha.. ja..",
		"ga.. sa.. sha..",
		"da.. ca.. tha.. jha.."
	};
	/*Mesa :	 	 a   la
Vrsabha: 	   u   va,   i, e, o
Mithuna:  	 ka,  cha, gha
Kataka:	   ha
Simha:		  ma
Kanya:		 pa
Tula:		 ra , ta
Vrschika:	  na ,  ba

Dhanus:	   dha , bha,  pha
Makra:  	kha, ja
Kumbha:	   ga,, sa
Mina:		  da, ca , jha
*/		

	return childsylable[n % 12];
}

const char * GetYogaName(int n)
{
	return gstr[660 + n % 27];
}

const char * GetSankrantiName(int i)
{
	return gstr[688 + i % 12];
}

const char * GetSankrantiNameEn(int i)
{

	return gstr[700 + i % 12];
}

char GetPaksaChar(int i)
{
	return (i ? 'G' : 'K');
}

const char * GetPaksaName(int i)
{
	return (i ? gstr[712] : gstr[713]);
}

const char * GetMasaName(int i)
{
	switch(GetShowSetVal(49))
	{
	case 0: // VAISNAVA
		return gstr[720 + i % 13];
	case 1: // VAISNAVA (COMMON)
		return gstr[897 + i % 13];
	case 2: // COMMON
		return gstr[871 + i % 13];
	case 3: // COMMON (VAISNAVA)
		return gstr[884 + i % 13];
	default:
		return gstr[720 + i % 13];
	}
}

const char * GetMahadvadasiName(int i)
{
	switch(i)
	{
	case EV_NULL:
	case EV_SUDDHA:
		return NULL;
	case EV_UNMILANI:
		return gstr[733];
	case EV_TRISPRSA:
	case EV_UNMILANI_TRISPRSA:
		return gstr[734];
	case EV_PAKSAVARDHINI:
		return gstr[735];
	case EV_JAYA:
		return gstr[736];
	case EV_VIJAYA:
		return gstr[737];
	case EV_PAPA_NASINI:
		return gstr[738];
	case EV_JAYANTI:
		return gstr[739];
	case EV_VYANJULI:
		return gstr[740];
	default:
		return NULL;
	}
}

const char * GetSpecFestivalName(int i)
{
	switch(i)
	{
	case SPEC_JANMASTAMI:
		return gstr[741];
	case SPEC_GAURAPURNIMA:
		return gstr[742];
	case SPEC_RETURNRATHA:
		return gstr[743];
	case SPEC_HERAPANCAMI:
		return gstr[744];
	case SPEC_GUNDICAMARJANA:
		return gstr[745];
	case SPEC_GOVARDHANPUJA:
		return gstr[746];
	case SPEC_RAMANAVAMI:
		return gstr[747];
	case SPEC_RATHAYATRA:
		return gstr[748];
	case SPEC_NANDAUTSAVA:
		return gstr[749];
	case SPEC_PRABHAPP:
		return gstr[759];
	case SPEC_MISRAFESTIVAL:
		return gstr[750];
	default:
		return gstr[64];
	}
}

void SetSpecFestivalName(int i, const char * szName)
{
	switch(i)
	{
	case SPEC_JANMASTAMI:
		gstr[741] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_GAURAPURNIMA:
		gstr[742] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_RETURNRATHA:
		gstr[743] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_HERAPANCAMI:
		gstr[744] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_GUNDICAMARJANA:
		gstr[745] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_GOVARDHANPUJA:
		gstr[746] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_RAMANAVAMI:
		gstr[747] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_RATHAYATRA:
		gstr[748] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_NANDAUTSAVA:
		gstr[749] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_PRABHAPP:
		gstr[759] = szName;
		gstr_Modified = 1;
		break;
	case SPEC_MISRAFESTIVAL:
		gstr[750] = szName;
		gstr_Modified = 1;
		break;
	default:
		break;
	}

	return;
}


Boolean GetSpecFestivalNameEx(TString &str, int i)
{
	switch(i)
	{
	case SPEC_JANMASTAMI:
		str = gstr[741];
		str += "[f:5:Sri Krsna]";
		break;
	case SPEC_GAURAPURNIMA:
		str = gstr[742];
		str += "[f:3:Sri Caitanya Mahaprabhu]";
		break;
	case SPEC_RETURNRATHA:
		str = gstr[743];
		break;
	case SPEC_HERAPANCAMI:
		str = gstr[744];
		break;
	case SPEC_GUNDICAMARJANA:
		str = gstr[745];
		break;
	case SPEC_GOVARDHANPUJA:
		str = gstr[746];
		break;
	case SPEC_RAMANAVAMI:
		str = gstr[747];
		str += "[f:2:Sri Ramacandra]";
		break;
	case SPEC_RATHAYATRA:
		str = gstr[748];
		break;
	case SPEC_NANDAUTSAVA:
		str = gstr[749];
		break;
	case SPEC_PRABHAPP:
		str = gstr[759];
		str += "[f:1:Srila Prabhupada]";
		break;
	case SPEC_MISRAFESTIVAL:
		str = gstr[750];
		break;
	default:
		str = gstr[64];
		return FALSE;
	}

	return TRUE;
}


const char * GetFastingName(int i)
{
	switch (i)
	{
	case FAST_NOON:
		return( gstr[751]);
		break;
	case FAST_SUNSET:
		return( gstr[752]);
		break;
	case FAST_MOONRISE:
		return( gstr[753]);
		break;
	case FAST_DUSK:
		return( gstr[754]);
		break;
	case FAST_MIDNIGHT:
		return( gstr[755]);
		break;
	case FAST_DAY:
		return( gstr[756]);
	default:
		return NULL;
	}
}

const char * GetEkadasiName(int nMasa, int nPaksa)
{
	return gstr[560 + nMasa*2 + nPaksa];
}

char * AvcGetTimeZoneName(int n)
{
	char * tzn[] = {
    "UTC-12",
    "UTC-11",
    "UTC-10",
    "UTC-9:30",
    "UTC-9",
    "UTC-8",
    "UTC-7",
    "UTC-6",
    "UTC-5",
    "UTC-4:30",
    "UTC-4",
    "UTC-3:30",
    "UTC-3",
    "UTC-2",
    "UTC-1",
    "UTC",
    "UTC+1",
    "UTC+2",
    "UTC+3",
    "UTC+3:30",
    "UTC+4",
    "UTC+4:30",
    "UTC+5",
    "UTC+5:30",
    "UTC+5:45",
    "UTC+6",
    "UTC+6:30",
    "UTC+7",
    "UTC+8",
    "UTC+8:45",
    "UTC+9",
    "UTC+9:30",
	"UTC+9:45",
    "UTC+10",
    "UTC+10:30",
    "UTC+11",
    "UTC+11:30",
    "UTC+12",
    "UTC+12:45",
    "UTC+13",
    "UTC+14"
	};

	if (n < 0)
		n = 0;
	if (n > 40)
		n = 40;
	return tzn[n];
}

double AvcGetTimeZone(int nIndex)
{
	double tzval[] = {
    -12.0,
    -11.0,
    -10.0,
    -9.5,
    -9.0,
    -8.0,
    -7.0,
    -6.0,
    -5.0,
    -4.5,
    -4.0,
    -3.5,
    -3.0,
    -2.0,
    -1.0,
    +0.0,
    +1.0,
    +2.0,
    +3.0,
    +3.5,
    +4.0,
    +4.5,
    +5.0,
    +5.5,
    +5.75,
    +6.0,
    +6.5,
    +7.0,
    +8.0,
    +8.75,
    +9.0,
    +9.5,
	+9.75,
    +10.0,
    +10.5,
    +11.0,
    +11.5,
    +12.0,
    +12.75,
    +13.0,
    +14.0
	};

	if (nIndex < 0)
		nIndex = 0;
	if (nIndex > 40)
		nIndex = 40;
	return tzval[nIndex];
}

int AvcGetNearestTimeZone(double d)
{

	if (d <= AvcGetTimeZone(0))
		return 0;
	if (d >= AvcGetTimeZone(40))
		return 40;
	int n;
	for(n = 0; n < 41; n++)
	{
		if (fabs(AvcGetTimeZone(n) - d) < 0.01)
			return n;
		if ((AvcGetTimeZone(n) <= d) && (AvcGetTimeZone(n+1) >= d))
		{
			if ((d - AvcGetTimeZone(n)) < (AvcGetTimeZone(n+1) - d))
				return n;
			else
				return n+1;
		}
	}

	return 39;
}

const char * AvcGetDSTSignature(int nDST)
{
	return nDST ? "DST" : "LT";
}

const char * AvcGetParanaReasonText(int eparana_type)
{
	switch(eparana_type)
	{
	case EP_TYPE_3DAY:
		return gstr[165].c_str();
	case EP_TYPE_4TITHI:
		return gstr[166].c_str();
	case EP_TYPE_SUNRISE:
		return gstr[169].c_str();
	case EP_TYPE_TEND:
		return gstr[167].c_str();
	case EP_TYPE_NAKEND:
		return gstr[168].c_str();
	default:
		break;
	}

	return "";
}

