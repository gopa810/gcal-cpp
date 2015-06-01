// PORTABLE
#ifndef _VCAL_SE_LEVEL_3
#define _VCAL_SE_LEVEL_3

#include "level_2.h"


int DayCalc(VCTIME date, EARTHDATA earth, DAYDATA &day);
int MasaCalc(VCTIME date, DAYDATA day, EARTHDATA earth,int&);
VCTIME GetFirstDayOfYear(EARTHDATA,int);
VCTIME GetFirstDayOfMasa(EARTHDATA earth, int GYear, int nMasa);
int GetNextNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
int GetPrevNaksatra(EARTHDATA ed, VCTIME startDate, VCTIME &prevDate);
int GetPrevTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
int GetNextTithiStart(EARTHDATA ed, VCTIME startDate, VCTIME &nextDate);
double DayCalcEx(VCTIME date, EARTHDATA earth, int nType);

#define MADHUSUDANA_MASA 0
#define TRIVIKRAMA_MASA  1
#define VAMANA_MASA      2
#define SRIDHARA_MASA    3
#define HRSIKESA_MASA    4
#define PADMANABHA_MASA  5
#define DAMODARA_MASA    6
#define KESAVA_MASA      7
#define NARAYANA_MASA    8
#define MADHAVA_MASA     9
#define GOVINDA_MASA     10
#define VISNU_MASA       11
#define ADHIKA_MASA      12

#define MESHA_SANKRANTI   0
#define VRSABHA_SANKRANTI 1
#define MITHUNA_SANKRANTI 2
#define KATAKA_SANKRANTI  3
#define SIMHA_SANKRANTI   4
#define KANYA_SANKRANTI   5
#define TULA_SANKRANTI    6
#define VRSCIKA_SANKRANTI 7
#define DHANUS_SANKRANTI  8
#define MAKARA_SANKRANTI  9
#define KUMBHA_SANKRANTI  10
#define MINA_SANKRANTI    11


#define KRSNA_PAKSA            0
#define GAURA_PAKSA            1

#define TITHI_KRSNA_PRATIPAT   0
#define TITHI_KRSNA_DVITIYA    1
#define TITHI_KRSNA_TRITIYA    2
#define TITHI_KRSNA_CATURTI    3
#define TITHI_KRSNA_PANCAMI    4
#define TITHI_KRSNA_SASTI      5
#define TITHI_KRSNA_SAPTAMI    6
#define TITHI_KRSNA_ASTAMI     7
#define TITHI_KRSNA_NAVAMI     8
#define TITHI_KRSNA_DASAMI     9
#define TITHI_KRSNA_EKADASI   10
#define TITHI_KRSNA_DVADASI   11
#define TITHI_KRSNA_TRAYODASI 12
#define TITHI_KRSNA_CATURDASI 13
#define TITHI_AMAVASYA        14
#define TITHI_GAURA_PRATIPAT  15
#define TITHI_GAURA_DVITIYA   16
#define TITHI_GAURA_TRITIYA   17
#define TITHI_GAURA_CATURTI   18
#define TITHI_GAURA_PANCAMI   19
#define TITHI_GAURA_SASTI     20
#define TITHI_GAURA_SAPTAMI   21
#define TITHI_GAURA_ASTAMI    22
#define TITHI_GAURA_NAVAMI    23
#define TITHI_GAURA_DASAMI    24
#define TITHI_GAURA_EKADASI   25
#define TITHI_GAURA_DVADASI   26
#define TITHI_GAURA_TRAYODASI 27
#define TITHI_GAURA_CATURDASI 28
#define TITHI_PURNIMA         29

#define ROHINI_NAKSATRA       3

#define DW_SUNDAY    0
#define DW_MONDAY    1
#define DW_TUESDAY   2
#define DW_WEDNESDAY 3
#define DW_THURSDAY  4
#define DW_FRIDAY    5
#define DW_SATURDAY  6

#define TITHI_EKADASI(a) (((a) == 10) || ((a) == 25))
#define TITHI_DVADASI(a) (((a) == 11) || ((a) == 26))
#define TITHI_TRAYODASI(a) (((a) == 12) || ((a) == 27))
#define TITHI_CATURDASI(a) (((a) == 13) || ((a) == 28))

#define TITHI_LESS_EKADASI(a) (((a) == 9) || ((a) == 24) || ((a) == 8) || ((a) == 23))
#define TITHI_LESS_DVADASI(a) (((a) == 10) || ((a) == 25) || ((a) == 9) || ((a) == 24))
#define TITHI_LESS_TRAYODASI(a) (((a) == 11) || ((a) == 26) || ((a) == 10) || ((a) == 25))
#define TITHI_FULLNEW_MOON(a) (((a) == 14) || ((a) == 29))

#define PREV_PREV_TITHI(a) (((a) + 28) % 30)
#define PREV_TITHI(a) (((a) + 29) % 30)
#define NEXT_TITHI(a) (((a) + 1) % 30)
#define NEXT_NEXT_TITHI(a) (((a) + 1) % 30)

#define TITHI_LESS_THAN(a,b) (((a) == PREV_TITHI(b)) || ((a) == PREV_PREV_TITHI(b)))
#define TITHI_GREAT_THAN(a,b) (((a) == NEXT_TITHI(b)) || ((a) == NEXT_NEXT_TITHI(b)))

// TRUE when transit from A to B is between T and U
#define TITHI_TRANSIT(t,u,a,b) (((t)==(a)) && ((u)==(b))) || (((t)==(a)) && ((u)==NEXT_TITHI(b))) || (((t)==PREV_TITHI(a)) && ((u) == (b)))

#endif