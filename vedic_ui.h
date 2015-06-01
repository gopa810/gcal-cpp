// PORTABLE
#ifndef _vedic_ui_2006_gpsl_
#define _vedic_ui_2006_gpsl_

const char * GetTithiName(int i);
const char * GetSankrantiName(int i);
const char * GetSankrantiNameEn(int i);
const char * GetNaksatraName(int n);
const char * GetYogaName(int n);
      char   GetPaksaChar(int i);
const char * GetMasaName(int i);
const char * GetMahadvadasiName(int i);
const char * GetPaksaName(int i);
const char * GetSpecFestivalName(int i);
const char * GetNaksatraChildSylable(int n, int pada);
const char * GetFastingName(int i);
const char * GetRasiChildSylable(int n);
const char * GetEkadasiName(int nMasa, int nPaksa);
   Boolean   GetSpecFestivalNameEx(TString &str, int i);
    double   AvcGetTimeZone(int nIndex);
       int   AvcGetNearestTimeZone(double d);
      char * AvcGetTimeZoneName(int n);
const char * AvcGetDSTSignature(int nDST);
const char * AvcGetParanaReasonText(int eparana_type);

#endif