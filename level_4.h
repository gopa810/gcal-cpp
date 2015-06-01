// PORTABLE
#ifndef _VCAL_SE_LEVEL_4
#define _VCAL_SE_LEVEL_4

#include "level_3.h"


VCTIME CalcTithiEnd(int nGYear, int nMasa, int nPaksa, int nTithi, EARTHDATA, VCTIME &);
VCTIME CalcTithiEndEx(VCTIME vcStart, int GYear, int nMasa, int nPaksa, int nTithi, EARTHDATA earth, VCTIME & endTithi);
VCTIME CalcTithiDate(int nGYear, int nMasa, int nPaksa, int nTithi, EARTHDATA earth);
int	GetGaurabdaYear(VCTIME vc, EARTHDATA earth);
void VATIMEtoVCTIME(VATIME va, VCTIME &vc, EARTHDATA earth);
void VCTIMEtoVATIME(VCTIME vc, VATIME &va, EARTHDATA earth);

#endif