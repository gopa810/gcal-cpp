#pragma once

#include "TString.h"
#include "enums.h"
#include "NSMutableArray.h"

class GCStrings
{
public:
	GCStrings(void);
	~GCStrings(void);
	static int gstr_Modified;
	static NSMutableArray<TString> gstr;
	static const char * GetMonthAbreviation(int month);
	static const char * GetTithiName(int i);
	static const char * GetNaksatraName(int n);
	static const char * GetSankrantiName(int i);
	static const char * GetSankrantiNameEn(int i);
	static const char * GetYogaName(int n);
	static char GetPaksaChar(int i);
	static const char * GetMasaName(int i);
	static const char * GetMahadvadasiName(int i);
	static const char * GetPaksaName(int i);
	static const char * GetSpecFestivalName(int i);
	static const char * GetNaksatraChildSylable(int n, int pada);
	static const char * GetFastingName(int i);
	static const char * GetRasiChildSylable(int n);
	static const char * GetEkadasiName(int nMasa, int nPaksa);
	static const char * GetDSTSignature(int nDST);
	static const char * GetParanaReasonText(int eparana_type);
	static void SetSpecFestivalName(int i, const char * szName);
	static TString & getString(int i);
	static void setString(int i, const char * str);
	static const char * GetVersionText(void);
	static const char * GetEventClassText(int i);
	static const char * GetKalaName(KalaType i);
	static const char * GetDayOfWeek(int i);
	static int readFile(const char * pszFile);
	static int writeFile(const char * pszFile);
	static const char * getLongitudeDirectionName(double d);
	static const char * getLatitudeDirectionName(double d);
	static bool InitLanguageOutputFromFile(const char * pszFile);
	static int getCount(void);
};

