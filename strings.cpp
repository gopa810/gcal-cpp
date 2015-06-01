#include "stdafx.h"
#include "platform.h"


// PORTABLE

TString gstr[950];
int gstr_Modified = 0;


/*
*  nRange: 0 - vsetky stringy
*          1 - vsetko okrem events (200-560)
*/
void InitGlobalStrings(int nRange)
{
	// days of week
	gstr[0] = "Sunday";
	gstr[1] = "Monday";
	gstr[2] = "Tuesday";
	gstr[3] = "Wednesday";
	gstr[4] = "Thursday";
	gstr[5] = "Friday";
	gstr[6] = "Saturday";
	
	gstr[7] = "Date";
	gstr[8] = "Time";
	gstr[9] = "Location";
	gstr[10]= "Latitude";
	gstr[11]= "Longitude";
	gstr[12] = "Timezone";
	gstr[13] = "Tithi";
	gstr[14] = "Tithi Elaps.";
	gstr[15] = "Naksatra";
	gstr[16] = "Naksatra Elaps.";
	gstr[17] = "Child Name";
	gstr[18] = "(according naksatra)";
	gstr[19] = "(according rasi)";
	gstr[20] = "Paksa";
	gstr[21] = "(during Purusottama Adhika Masa)";
	gstr[22] = "Masa";
	gstr[23] = "Gaurabda Year";
	gstr[24] = "Celebrations";
	gstr[25] = "Appearance Day Calculation";
	gstr[26] = "Day can be only from 1 to 31";
	gstr[27] = "Month can be only from 1 to 12";
	gstr[28] = "Year can be only from 1500 to 3999";
	gstr[29] = "Given date was corrected, since date given by you was nonexisting.";
	gstr[30] = "days";
	gstr[31] = "months";
	gstr[32] = "years";
	gstr[33] = "exactly given date";
	gstr[34] = "from begining of the month";
	gstr[35] = "align to the start of the Masa";
	gstr[36] = "from begining of the year";
	gstr[37] = "start from first day of the Gaurabda Year";
	gstr[38] = "<all mahadvadasis>";
	gstr[39] = "Masa Listing";
	gstr[40] = "Location";
	gstr[41] = "From";
	gstr[42] = "To";
	gstr[43] = "Today";
	gstr[44] = " Calendar";
	gstr[45] = " Appearance Day";
	gstr[46] = " Core Events";
	gstr[47] = "";
	gstr[48] = " Masa List";
	gstr[49] = "";
	gstr[50] = "Could not create new window.";
	gstr[51] = "Sunrise";
	gstr[52] = "Sunset";
	gstr[53] = "Moonrise";
	gstr[54] = "Moonset";
	gstr[55] = "Year";
	gstr[56] = "Sankranti";
	gstr[57] = "Please update GCAL to the latest version from http://www.krishnadays.com";
	gstr[58] = "(not suitable for fasting)";
	gstr[59] = "(suitable for fasting)";
	gstr[60] = "Break fast";
	gstr[61] = "Break fast after";
	gstr[62] = "Break fast not calculated";
	gstr[63] = "(DST not considered)";


	gstr[64] = "";
	gstr[65] = "Jan";
	gstr[66] = "Feb";
	gstr[67] = "Mar";
	gstr[68] = "Apr";
	gstr[69] = "May";
	gstr[70] = "Jun";
	gstr[71] = "Jul";
	gstr[72] = "Aug";
	gstr[73] = "Sep";
	gstr[74] = "Oct";
	gstr[75] = "Nov";
	gstr[76] = "Dec";


	gstr[78] = "Ganga Sagara Mela";
	gstr[79] = "Tulasi Jala Dan begins.";
	gstr[80] = "Tulasi Jala Dan ends.";
	gstr[81] = "First day of Bhisma Pancaka";
	gstr[82] = "Last day of Bhisma Pancaka";
	gstr[83] = "Looking for";
	gstr[84] = "Start";
	gstr[85] = "End";
	gstr[86] = "Not found in this year.";
	gstr[87] = "Fasting for";
	gstr[88] = "(Fasting is done yesterday, today is feast)";
	gstr[89] = "Ksaya tithi";
	gstr[90] = "[Second day of Tithi]";

	gstr[91] = "First day";
	gstr[92] = "Last day";
	gstr[93] = "of the first month";
	gstr[94] = "of the second month";
	gstr[95] = "of the third month";
	gstr[96] = "of the fourth month";
	gstr[97] = "of Caturmasya-vrata";

	gstr[98] = "Arunodaya Tithi";
	gstr[99] = "Arunodaya at";
	gstr[100] = "Sun Longitude";
	gstr[101] = "Moon Longitude";
	gstr[102] = "Ayanamsa";
	gstr[103] = "Julian day";
	gstr[104] = "Yoga";
	gstr[105] = "Rasi";

	gstr[106] = "(Daylight Saving Time not considered)";
	gstr[107] = "(Winter Time)";
	gstr[108] = "(Summer Time)";
	gstr[109] = "(Second half)";

	gstr[110] = "Event Finder";
	gstr[111] = "Sun enters";

	gstr[112] = "First month of Caturmasya begins";
	gstr[113] = "Last day of the first Caturmasya month";
	gstr[114] = "(green leafy vegetable fast for one month)";
	gstr[115] = "First month of Caturmasya continues";

	gstr[116] = "Second month of Caturmasya begins";
	gstr[117] = "Last day of the second Caturmasya month";
	gstr[118] = "(yogurt fast for one month)";
	gstr[119] = "Second month of Caturmasya continues";

	gstr[120] = "Third month of Caturmasya begins";
	gstr[121] = "Last day of the third Caturmasya month";
	gstr[122] = "(milk fast for one month)";
	gstr[123] = "Third month of Caturmasya continues";

	gstr[124] = "Fourth month of Caturmasya begins";
	gstr[125] = "Last day of the fourth Caturmasya month";
	gstr[126] = "(urad dal fast for one month)";
	gstr[127] = "Fourth month of Caturmasya continues";

	gstr[128] = "(Caturmasya is not observed during Purusottama Adhika Masa.)";
	gstr[129] = "11, Build 3";
	gstr[130] = "GCal ";
	gstr[130] += gstr[129];
	gstr[131] = "Gaurabda Calendar ";
	gstr[131] += gstr[129];

	gstr[132] = "<all sankrantis>";
	gstr[133] = "<all tithis>";
	gstr[134] = "<all fasting days>";
	gstr[135] = "(Fasting for Ekadasi)";

	gstr[136] = "No Moonrise";
	gstr[137] = "No Moonset";
	gstr[138] = "No Sunrise";
	gstr[139] = "No Sunset";

	gstr[140] = "SUN - MOON CONJUNCTIONS";

	gstr[150] = "Su";
	gstr[151] = "Mo";
	gstr[152] = "Tu";
	gstr[153] = "We";
	gstr[154] = "Th";
	gstr[155] = "Fr";
	gstr[156] = "Sa";

	gstr[157] = "DATE";
	gstr[158] = "SUNRISE";
	gstr[159] = "MASA";
	gstr[160] = "TITHI START";
	gstr[161] = "NAKSATRA START";

	gstr[162] = "Page";

	gstr[165] = "1/3 of daylight";
	gstr[166] = "1/4 of tithi";
	gstr[167] = "end of tithi";
	gstr[168] = "end of naksatra";
	gstr[169] = "sunrise";

	if (nRange != 1)
	{
		// strings for festivals
		gstr[200] = "";
		gstr[201] = "";
		gstr[202] = "";
		gstr[203] = "";
		gstr[204] = "";
		gstr[205] = "";
		gstr[206] = "[c3]Sri Abhirama Thakura -- Disappearance";
		gstr[207] = "";
		gstr[208] = "";
		gstr[209] = "[c3]Srila Vrndavana Dasa Thakura -- Disappearance";
		gstr[210] = "";
		gstr[211] = "";
		gstr[212] = "";
		gstr[213] = "";
		gstr[214] = "[c3]Sri Gadadhara Pandita -- Appearance";
		gstr[215] = "";
		gstr[216] = "";
		gstr[217] = "[c1]Aksaya Trtiya. Candana Yatra starts. (Continues for 21 days)";
		gstr[218] = "";
		gstr[219] = "";
		gstr[220] = "";
		gstr[221] = "[c5]Jahnu Saptami";
		gstr[222] = "";
		gstr[223] = "[c1]Srimati Sita Devi (consort of Lord Sri Rama) -- Appearance#[c3]Sri Madhu Pandita -- Disappearance#[c3]Srimati Jahnava Devi -- Appearance";
		gstr[224] = "";
		gstr[225] = "";
		gstr[226] = "[c1]Rukmini Dvadasi";
		gstr[227] = "[c4]Sri Jayananda Prabhu -- Disappearance";
		gstr[228] = "[c0]Nrsimha Caturdasi: Appearance of Lord Nrsimhadeva[f:4:Lord Nrsimhadeva]";
		gstr[229] = "[c1]Krsna Phula Dola, Salila Vihara#[c3]Sri Paramesvari Dasa Thakura -- Disappearance#[c1]Sri Sri Radha-Ramana Devaji -- Appearance#[c3]Sri Madhavendra Puri -- Appearance#[c3]Sri Srinivasa Acarya -- Appearance";
		gstr[230] = "";
		gstr[231] = "";
		gstr[232] = "";
		gstr[233] = "";
		gstr[234] = "[c3]Sri Ramananda Raya -- Disappearance";
		gstr[235] = "";
		gstr[236] = "";
		gstr[237] = "";
		gstr[238] = "";
		gstr[239] = "";
		gstr[240] = "";
		gstr[241] = "[c3]Srila Vrndavana Dasa Thakura -- Appearance";
		gstr[242] = "";
		gstr[243] = "";
		gstr[244] = "";
		gstr[245] = "";
		gstr[246] = "";
		gstr[247] = "";
		gstr[248] = "";
		gstr[249] = "";
		gstr[250] = "";
		gstr[251] = "";
		gstr[252] = "";
		gstr[253] = "";
		gstr[254] = "[c3]Sri Baladeva Vidyabhusana -- Disappearance#[c1]Ganga Puja#[c3]Srimati Gangamata Gosvamini -- Appearance";
		gstr[255] = "";
		gstr[256] = "";
		gstr[257] = "[c1]Panihati Cida Dahi Utsava";
		gstr[258] = "";
		gstr[259] = "[c1]Snana Yatra#[c3]Sri Mukunda Datta -- Disappearance#[c3]Sri Sridhara Pandita -- Disappearance";
		gstr[260] = "[c3]Sri Syamananda Prabhu -- Disappearance";
		gstr[261] = "";
		gstr[262] = "";
		gstr[263] = "";
		gstr[264] = "[c3]Sri Vakresvara Pandita -- Appearance";
		gstr[265] = "";
		gstr[266] = "";
		gstr[267] = "";
		gstr[268] = "";
		gstr[269] = "[c3]Sri Srivasa Pandita -- Disappearance";
		gstr[270] = "";
		gstr[271] = "";
		gstr[272] = "";
		gstr[273] = "";
		gstr[274] = "[c3]Sri Gadadhara Pandita -- Disappearance#[c2]Srila Bhaktivinoda Thakura -- Disappearance[f:1:Bhaktivinoda Thakura]";
		gstr[275] = "";
		gstr[276] = "[c3]Sri Svarupa Damodara Gosvami -- Disappearance#[c3]Sri Sivananda Sena -- Disappearance";
		gstr[277] = "";
		gstr[278] = "";
		gstr[279] = "";
		gstr[280] = "[c3]Sri Vakresvara Pandita -- Disappearance";
		gstr[281] = "";
		gstr[282] = "";
		gstr[283] = "";
		gstr[284] = "";
		gstr[285] = "";
		gstr[286] = "";
		gstr[287] = "";
		gstr[288] = "";
		gstr[289] = "[c1]Guru (Vyasa) Purnima#[c3]Srila Sanatana Gosvami -- Disappearance";
		gstr[290] = "";
		gstr[291] = "";
		gstr[292] = "";
		gstr[293] = "";
		gstr[294] = "[c3]Srila Gopala Bhatta Gosvami -- Disappearance";
		gstr[295] = "";
		gstr[296] = "";
		gstr[297] = "[c3]Srila Lokanatha Gosvami -- Disappearance";
		gstr[298] = "[c4]The incorporation of ISKCON in New York";
		gstr[299] = "";
		gstr[300] = "";
		gstr[301] = "";
		gstr[302] = "";
		gstr[303] = "";
		gstr[304] = "";
		gstr[305] = "";
		gstr[306] = "";
		gstr[307] = "";
		gstr[308] = "[c3]Sri Raghunandana Thakura -- Disappearance#[c2]Sri Vamsidasa Babaji -- Disappearance";
		gstr[309] = "";
		gstr[310] = "";
		gstr[311] = "";
		gstr[312] = "";
		gstr[313] = "";
		gstr[314] = "";
		gstr[315] = "[c1]Radha Govinda Jhulana Yatra begins";
		gstr[316] = "[c3]Srila Rupa Gosvami -- Disappearance#[c3]Sri Gauridasa Pandita -- Disappearance";
		gstr[317] = "";
		gstr[318] = "";
		gstr[319] = "[c1]Jhulana Yatra ends#[c0]Lord Balarama -- Appearance[f:1:Lord Balarama]";
		gstr[320] = "[c4]Srila Prabhupada's departure for the USA";
		gstr[321] = "";
		gstr[322] = "";
		gstr[323] = "";
		gstr[324] = "";
		gstr[325] = "";
		gstr[326] = "";
		gstr[327] = "";
		gstr[328] = "";
		gstr[329] = "";
		gstr[330] = "";
		gstr[331] = "";
		gstr[332] = "";
		gstr[333] = "";
		gstr[334] = "";
		gstr[335] = "";
		gstr[336] = "";
		gstr[337] = "";
		gstr[338] = "";
		gstr[339] = "[c3]Srimati Sita Thakurani (Sri Advaita's consort) -- Appearance";
		gstr[340] = "[c1]Lalita sasti";
		gstr[341] = "";
		gstr[342] = "[c0]Radhastami: Appearance of Srimati Radharani[f:1:Srimati Radharani]";
		gstr[343] = "";
		gstr[344] = "";
		gstr[345] = "";
		gstr[346] = "[c0]Sri Vamana Dvadasi: Appearance of Lord Vamanadeva[f:1:Vamanadeva]#[c3]Srila Jiva Gosvami -- Appearance";
		gstr[347] = "[c2]Srila Bhaktivinoda Thakura -- Appearance[f:1:Bhaktivinoda Thakura]";
		gstr[348] = "[c1]Ananta Caturdasi Vrata#[c3]Srila Haridasa Thakura -- Disappearance";
		gstr[349] = "[c1]Sri Visvarupa Mahotsava#[c4]Acceptance of sannyasa by Srila Prabhupada";
		gstr[350] = "";
		gstr[351] = "";
		gstr[352] = "";
		gstr[353] = "";
		gstr[354] = "";
		gstr[355] = "";
		gstr[356] = "[c4]Srila Prabhupada's arrival in the USA";
		gstr[357] = "";
		gstr[358] = "";
		gstr[359] = "";
		gstr[360] = "";
		gstr[361] = "";
		gstr[362] = "";
		gstr[363] = "";
		gstr[364] = "";
		gstr[365] = "";
		gstr[366] = "";
		gstr[367] = "";
		gstr[368] = "";
		gstr[369] = "";
		gstr[370] = "";
		gstr[371] = "[c5]Durga Puja";
		gstr[372] = "";
		gstr[373] = "";
		gstr[374] = "[c1]Ramacandra Vijayotsava#[c2]Sri Madhvacarya -- Appearance";
		gstr[375] = "";
		gstr[376] = "[c3]Srila Raghunatha Dasa Gosvami -- Disappearance#[c3]Srila Raghunatha Bhatta Gosvami -- Disappearance#[c3]Srila Krsnadasa Kaviraja Gosvami -- Disappearance";
		gstr[377] = "";
		gstr[378] = "";
		gstr[379] = "[c1]Sri Krsna Saradiya Rasayatra#[c3]Sri Murari Gupta -- Disappearance#[c5]Laksmi Puja";
		gstr[380] = "";
		gstr[381] = "";
		gstr[382] = "";
		gstr[383] = "";
		gstr[384] = "[c3]Srila Narottama Dasa Thakura -- Disappearance";
		gstr[385] = "";
		gstr[386] = "";
		gstr[387] = "[c1]Appearance of Radha Kunda, snana dana#[c1]Bahulastami";
		gstr[388] = "[c3]Sri Virabhadra -- Appearance";
		gstr[389] = "";
		gstr[390] = "";
		gstr[391] = "";
		gstr[392] = "";
		gstr[393] = "";
		gstr[394] = "[c1]Dipa dana, Dipavali, (Kali Puja)";
		gstr[395] = "[c1]Bali Daityaraja Puja#[c3]Sri Rasikananda -- Appearance";
		gstr[396] = "[c3]Sri Vasudeva Ghosh -- Disappearance";
		gstr[397] = "";
		gstr[398] = "[c2]Srila Prabhupada -- Disappearance[f:1:Srila Prabhupada]";
		gstr[399] = "";
		gstr[400] = "";
		gstr[401] = "";
		gstr[402] = "[c1]Gopastami, Gosthastami#[c3]Sri Gadadhara Dasa Gosvami -- Disappearance#[c3]Sri Dhananjaya Pandita -- Disappearance#[c3]Sri Srinivasa Acarya -- Disappearance";
		gstr[403] = "[c5]Jagaddhatri Puja";
		gstr[404] = "";
		gstr[405] = "[c2]Srila Gaura Kisora Dasa Babaji -- Disappearance[f:1:Gaura Kisora Dasa Babaji]";
		gstr[406] = "";
		gstr[407] = "";
		gstr[408] = "[c3]Sri Bhugarbha Gosvami -- Disappearance#[c3]Sri Kasisvara Pandita -- Disappearance";
		gstr[409] = "[c1]Sri Krsna Rasayatra#[c1]Tulasi-Saligrama Vivaha (marriage)#[c3]Sri Nimbarkacarya -- Appearance";
		gstr[410] = "[c1]Katyayani vrata begins";
		gstr[411] = "";
		gstr[412] = "";
		gstr[413] = "";
		gstr[414] = "";
		gstr[415] = "";
		gstr[416] = "";
		gstr[417] = "";
		gstr[418] = "";
		gstr[419] = "";
		gstr[420] = "[c3]Sri Narahari Sarakara Thakura -- Disappearance";
		gstr[421] = "[c3]Sri Kaliya Krsnadasa -- Disappearance";
		gstr[422] = "[c3]Sri Saranga Thakura -- Disappearance";
		gstr[423] = "";
		gstr[424] = "";
		gstr[425] = "";
		gstr[426] = "";
		gstr[427] = "";
		gstr[428] = "";
		gstr[429] = "";
		gstr[430] = "[c1]Odana sasthi";
		gstr[431] = "";
		gstr[432] = "";
		gstr[433] = "";
		gstr[434] = "";
		gstr[435] = "[c1]Advent of Srimad Bhagavad-gita";
		gstr[436] = "";
		gstr[437] = "";
		gstr[438] = "";
		gstr[439] = "[c1]Katyayani vrata ends";
		gstr[440] = "";
		gstr[441] = "";
		gstr[442] = "";
		gstr[443] = "[c2]Srila Bhaktisiddhanta Sarasvati Thakura -- Disappearance[f:1:Bhaktisiddhanta Sarasvati]";
		gstr[444] = "";
		gstr[445] = "";
		gstr[446] = "";
		gstr[447] = "";
		gstr[448] = "";
		gstr[449] = "";
		gstr[450] = "[c3]Sri Devananda Pandita -- Disappearance";
		gstr[451] = "";
		gstr[452] = "[c3]Sri Mahesa Pandita -- Disappearance#[c3]Sri Uddharana Datta Thakura -- Disappearance";
		gstr[453] = "";
		gstr[454] = "";
		gstr[455] = "[c3]Sri Locana Dasa Thakura -- Appearance";
		gstr[456] = "";
		gstr[457] = "[c3]Srila Jiva Gosvami -- Disappearance#[c3]Sri Jagadisa Pandita -- Disappearance";
		gstr[458] = "";
		gstr[459] = "";
		gstr[460] = "";
		gstr[461] = "";
		gstr[462] = "";
		gstr[463] = "";
		gstr[464] = "";
		gstr[465] = "";
		gstr[466] = "[c3]Sri Jagadisa Pandita -- Appearance";
		gstr[467] = "";
		gstr[468] = "";
		gstr[469] = "[c1]Sri Krsna Pusya abhiseka";
		gstr[470] = "";
		gstr[471] = "";
		gstr[472] = "";
		gstr[473] = "";
		gstr[474] = "[c3]Sri Ramacandra Kaviraja -- Disappearance#[c3]Srila Gopala Bhatta Gosvami -- Appearance";
		gstr[475] = "[c3]Sri Jayadeva Gosvami -- Disappearance";
		gstr[476] = "[c3]Sri Locana Dasa Thakura -- Disappearance";
		gstr[477] = "";
		gstr[478] = "";
		gstr[479] = "";
		gstr[480] = "";
		gstr[481] = "";
		gstr[482] = "";
		gstr[483] = "";
		gstr[484] = "";
		gstr[485] = "";
		gstr[486] = "";
		gstr[487] = "";
		gstr[488] = "";
		gstr[489] = "[c1]Vasanta Pancami#[c3]Srimati Visnupriya Devi -- Appearance#[c5]Sarasvati Puja#[c3]Srila Visvanatha Cakravarti Thakura -- Disappearance#[c3]Sri Pundarika Vidyanidhi -- Appearance#[c3]Sri Raghunandana Thakura -- Appearance#[c3]Srila Raghunatha Dasa Gosvami -- Appearance";
		gstr[490] = "";
		gstr[491] = "[c0]Sri Advaita Acarya -- Appearance[f:1:Advaita Acarya]";
		gstr[492] = "[c1]Bhismastami";
		gstr[493] = "[c3]Sri Madhvacarya -- Disappearance";
		gstr[494] = "[c3]Sri Ramanujacarya -- Disappearance";
		gstr[495] = "";
		gstr[496] = "[c0]Varaha Dvadasi: Appearance of Lord Varahadeva[f:1:Varahadeva]";
		gstr[497] = "[c0]Nityananda Trayodasi: Appearance of Sri Nityananda Prabhu[f:1:Sri Nityananda]";
		gstr[498] = "";
		gstr[499] = "[c1]Sri Krsna Madhura Utsava#[c3]Srila Narottama Dasa Thakura -- Appearance";
		gstr[500] = "";
		gstr[501] = "";
		gstr[502] = "";
		gstr[503] = "";
		gstr[504] = "[c3]Sri Purusottama Das Thakura -- Disappearance#[c2]Srila Bhaktisiddhanta Sarasvati Thakura -- Appearance[f:1:Bhaktisiddhanta Sarasvati]";//#Srila Gour Govinda Swami -- Disappearance";
		gstr[505] = "";
		gstr[506] = "";
		gstr[507] = "";
		gstr[508] = "";
		gstr[509] = "";
		gstr[510] = "";
		gstr[511] = "[c3]Sri Isvara Puri -- Disappearance";
		gstr[512] = "";
		gstr[513] = "[c1]Siva Ratri";
		gstr[514] = "";
		gstr[515] = "[c2]Srila Jagannatha Dasa Babaji -- Disappearance#[c3]Sri Rasikananda -- Disappearance";
		gstr[516] = "";
		gstr[517] = "";
		gstr[518] = "[c3]Sri Purusottama Dasa Thakura -- Appearance";
		gstr[519] = "";
		gstr[520] = "";
		gstr[521] = "";
		gstr[522] = "";
		gstr[523] = "";
		gstr[524] = "";
		gstr[525] = "";
		gstr[526] = "[c3]Sri Madhavendra Puri -- Disappearance";
		gstr[527] = "";
		gstr[528] = "";
		gstr[529] = "";
		gstr[530] = "";
		gstr[531] = "";
		gstr[532] = "";
		gstr[533] = "";
		gstr[534] = "";
		gstr[535] = "";
		gstr[536] = "";
		gstr[537] = "[c3]Sri Srivasa Pandita -- Appearance";
		gstr[538] = "";
		gstr[539] = "";
		gstr[540] = "";
		gstr[541] = "[c3]Sri Govinda Ghosh -- Disappearance";
		gstr[542] = "";
		gstr[543] = "";
		gstr[544] = "";
		gstr[545] = "";
		gstr[546] = "";
		gstr[547] = "";
		gstr[548] = "";
		gstr[549] = "[c3]Sri Ramanujacarya -- Appearance";
		gstr[550] = "";
		gstr[551] = "";
		gstr[552] = "";
		gstr[553] = "";
		gstr[554] = "";
		gstr[555] = "";
		gstr[556] = "[c1]Damanakaropana Dvadasi";
		gstr[557] = "";
		gstr[558] = "";
		gstr[559] = "[c1]Sri Balarama Rasayatra#[c1]Sri Krsna Vasanta Rasa#[c3]Sri Vamsivadana Thakura -- Appearance#[c3]Sri Syamananda Prabhu -- Appearance";
	}

	// mena ekadasi
	gstr[560] = "Varuthini Ekadasi";
	gstr[561] = "Mohini Ekadasi";
	gstr[562] = "Apara Ekadasi";
	gstr[563] = "Pandava Nirjala Ekadasi#(Total fast, even from water, if you have broken Ekadasi)";
	gstr[564] = "Yogini Ekadasi";
	gstr[565] = "Sayana Ekadasi";
	gstr[566] = "Kamika Ekadasi";
	gstr[567] = "Pavitraropana Ekadasi";
	gstr[568] = "Annada Ekadasi";
	gstr[569] = "Parsva Ekadasi";
	gstr[570] = "Indira Ekadasi";
	gstr[571] = "Pasankusa Ekadasi";
	gstr[572] = "Rama Ekadasi";
	gstr[573] = "Utthana Ekadasi";
	gstr[574] = "Utpanna Ekadasi";
	gstr[575] = "Moksada Ekadasi";
	gstr[576] = "Saphala Ekadasi";
	gstr[577] = "Putrada Ekadasi";
	gstr[578] = "Sat-tila Ekadasi";
	gstr[579] = "Bhaimi Ekadasi";
	gstr[580] = "Vijaya Ekadasi";
	gstr[581] = "Amalaki vrata Ekadasi";
	gstr[582] = "Papamocani Ekadasi";
	gstr[583] = "Kamada Ekadasi";
	gstr[584] = "Parama Ekadasi";
	gstr[585] = "Padmini Ekadasi";

	gstr[600] = "Pratipat";
	gstr[601] = "Dvitiya";
	gstr[602] = "Tritiya";
	gstr[603] = "Caturthi";
	gstr[604] = "Pancami";
	gstr[605] = "Sasti";
	gstr[606] = "Saptami";
	gstr[607] = "Astami";
	gstr[608] = "Navami";
	gstr[609] = "Dasami";
	gstr[610] = "Ekadasi";
	gstr[611] = "Dvadasi";
	gstr[612] = "Trayodasi";
	gstr[613] = "Caturdasi";
	gstr[614] = "Amavasya";
	gstr[615] = "Pratipat";
	gstr[616] = "Dvitiya";
	gstr[617] = "Tritiya";
	gstr[618] = "Caturthi";
	gstr[619] = "Pancami";
	gstr[620] = "Sasti";
	gstr[621] = "Saptami";
	gstr[622] = "Astami";
	gstr[623] = "Navami";
	gstr[624] = "Dasami";
	gstr[625] = "Ekadasi";
	gstr[626] = "Dvadasi";
	gstr[627] = "Trayodasi";
	gstr[628] = "Caturdasi";
	gstr[629] = "Purnima";

	// naksatras
	gstr[630] = "Asvini";
	gstr[631] = "Bharani";
	gstr[632] = "Krittika";
	gstr[633] = "Rohini";
	gstr[634] = "Mrigasira";
	gstr[635] = "Ardra";
	gstr[636] = "Punarvasu";
	gstr[637] = "Pusyami";
	gstr[638] = "Aslesa";
	gstr[639] = "Magha";
	gstr[640] = "Purva-phalguni";
	gstr[641] = "Uttara-phalguni";
	gstr[642] = "Hasta";
	gstr[643] = "Citra";
	gstr[644] = "Swati";
	gstr[645] = "Visakha";
	gstr[646] = "Anuradha";
	gstr[647] = "Jyestha";
	gstr[648] = "Mula";
	gstr[649] = "Purva-asadha";
	gstr[650] = "Uttara-asadha";  
	gstr[651] = "Sravana";
	gstr[652] = "Dhanista";
	gstr[653] = "Satabhisa";
	gstr[654] = "Purva-bhadra";
	gstr[655] = "Uttara-bhadra";
	gstr[656] = "Revati";

	//yoga
	gstr[660] = "Viskumba";
	gstr[661] = "Priti"; 
	gstr[662] = "Ayusmana";
	gstr[663] = "Saubhagya";
	gstr[664] = "Sobana";
	gstr[665] = "Atiganda";
	gstr[666] = "Sukarma";
	gstr[667] = "Dhriti";
	gstr[668] = "Sula";
	gstr[669] = "Ganda";
	gstr[670] = "Vriddhi";
	gstr[671] = "Dhruva";
	gstr[672] = "Vyagata"; 
	gstr[673] = "Harsana"; 
	gstr[674] = "Vajra"; 
	gstr[675] = "Siddhi"; 
	gstr[676] = "Vyatipata"; 
	gstr[677] = "Variyana"; 
	gstr[678] = "Parigha"; 
	gstr[679] = "Siva"; 
	gstr[680] = "Siddha"; 
	gstr[681] = "Sadhya"; 
	gstr[682] = "Subha";
	gstr[683] = "Sukla"; 
	gstr[684] = "Brahma";
	gstr[685] = "Indra"; 
	gstr[686] = "Vaidhriti";

	// rasi
	gstr[688] = "Mesa";
	gstr[689] = "Vrsabha";
	gstr[690] = "Mithuna";
	gstr[691] = "Karka";
	gstr[692] = "Simha";
	gstr[693] = "Kanya";
	gstr[694] = "Tula";
	gstr[695] = "Vrscika";
	gstr[696] = "Dhanus";
	gstr[697] = "Makara";
	gstr[698] = "Kumbha";
	gstr[699] = "Mina";

	// rasi eng
	gstr[700] = "Aries",
	gstr[701] = "Taurus",
	gstr[702] = "Gemini",
	gstr[703] = "Cancer",
	gstr[704] = "Leo",
	gstr[705] = "Virgo",
	gstr[706] = "Libra",
	gstr[707] = "Scorpio",
	gstr[708] = "Sagittarius",
	gstr[709] = "Capricorn",
	gstr[710] = "Aquarius",
	gstr[711] = "Pisces";

	// paksa
	gstr[712] = "Gaura";
	gstr[713] = "Krsna";

	//masa
	gstr[720] = "Madhusudana";
	gstr[721] = "Trivikrama";
	gstr[722] = "Vamana";
	gstr[723] = "Sridhara";
	gstr[724] = "Hrsikesa";
	gstr[725] = "Padmanabha";
	gstr[726] = "Damodara";
	gstr[727] = "Kesava";
	gstr[728] = "Narayana";
	gstr[729] = "Madhava";
	gstr[730] = "Govinda";
	gstr[731] = "Visnu";
	gstr[732] = "Purusottama-adhika";

	// mahadvadasi
	gstr[733] = "Unmilani Mahadvadasi";
	gstr[734] = "Trisprsa Mahadvadasi";
	gstr[735] = "Paksa vardhini Mahadvadasi";
	gstr[736] = "Jaya Mahadvadasi";
	gstr[737] = "Vijaya Mahadvadasi";
	gstr[738] = "Papa Nasini Mahadvadasi";
	gstr[739] = "Jayanti Mahadvadasi";
	gstr[740] = "Vyanjuli Mahadvadasi";

	gstr[741] = "Sri Krsna Janmastami: Appearance of Lord Sri Krsna";
	gstr[742] = "Gaura Purnima: Appearance of Sri Caitanya Mahaprabhu";
	gstr[743] = "Return Ratha (8 days after Ratha Yatra)";
	gstr[744] = "Hera Pancami (4 days after Ratha Yatra)";
	gstr[745] = "Gundica Marjana";
	gstr[746] = "Go Puja. Go Krda. Govardhana Puja.";
	gstr[747] = "Rama Navami: Appearance of Lord Sri Ramacandra";
	gstr[748] = "Ratha Yatra";
	gstr[749] = "Nandotsava";
	gstr[750] = "Festival of Jagannatha Misra";

	gstr[751] = "(Fast till noon)";
	gstr[752] = "(Fast till sunset)";
	gstr[753] = "(Fast till moonrise)";
	gstr[754] = "(Fast till dusk)";
	gstr[755] = "(Fast till midnight)";
	gstr[756] = "(Fast today)";

	gstr[759] = "Srila Prabhupada -- Appearance";

	gstr[760] = "January";
	gstr[761] = "February";
	gstr[762] = "March";
	gstr[763] = "April";
	gstr[764] = "May";
	gstr[765] = "June";
	gstr[766] = "July";
	gstr[767] = "August";
	gstr[768] = "September";
	gstr[769] = "October";
	gstr[770] = "November";
	gstr[771] = "December";

	gstr[780] = "First day of";
	gstr[781] = "Last day of";

	gstr[782] = "first"; // 2. pad
	gstr[783] = "second";
	gstr[784] = "third";
	gstr[785] = "fourth";
	gstr[786] = "last";

	gstr[787] = "Sunday"; // 2.pad
	gstr[788] = "Monday";
	gstr[789] = "Tuesday";
	gstr[790] = "Wednesday";
	gstr[791] = "Thursday";
	gstr[792] = "Friday";
	gstr[793] = "Saturday";

	gstr[795] = "of January";
	gstr[796] = "of February";
	gstr[797] = "of March";
	gstr[798] = "of April";
	gstr[799] = "of May";
	gstr[800] = "of June";
	gstr[801] = "of July";
	gstr[802] = "of August";
	gstr[803] = "of September";
	gstr[804] = "of October";
	gstr[805] = "of November";
	gstr[806] = "of December";

	gstr[807] = "For this location is Daylight Saving Time not observed.";
	gstr[808] = "Daylight saving time is observed ";

	gstr[810] = "0th";
	gstr[811] = "1st";
	gstr[812] = "2nd";
	gstr[813] = "3rd";
	gstr[814] = "4th";
	gstr[815] = "5th";
	gstr[816] = "6th";
	gstr[817] = "7th";
	gstr[818] = "8th";
	gstr[819] = "9th";
	gstr[820] = "10th";
	gstr[821] = "11th";
	gstr[822] = "12th";
	gstr[823] = "13th";
	gstr[824] = "14th";
	gstr[825] = "15th";
	gstr[826] = "16th";
	gstr[827] = "17th";
	gstr[828] = "18th";
	gstr[829] = "19th";
	gstr[830] = "20th";
	gstr[831] = "21st";
	gstr[832] = "22nd";
	gstr[833] = "23rd";
	gstr[834] = "24th";
	gstr[835] = "25th";
	gstr[836] = "26th";
	gstr[837] = "27th";
	gstr[838] = "28th";
	gstr[839] = "29th";
	gstr[840] = "30th";
	gstr[841] = "31st";

	gstr[850] = "since";
	gstr[851] = "to";
	gstr[852] = "on";

	gstr[853] = "Yesterday";
	gstr[854] = "Tomorrow";
	gstr[855] = "First day of Daylight Saving Time";
	gstr[856] = "Last day of Daylight Saving Time";
	gstr[857] = "Noon";

	gstr[860] = "(Fasting is done yesterday)";
	gstr[861] = "(Fasting is done yesterday, today is feast)";
	gstr[862] = "(Fasting till noon, with feast tomorrow)";

	gstr[871] = "Vaisakha";
	gstr[872] = "Jyestha";
	gstr[873] = "Asadha";
	gstr[874] = "Sravana";
	gstr[875] = "Bhadra";
	gstr[876] = "Asvina";
	gstr[877] = "Kartika";
	gstr[878] = "Margasirsa";
	gstr[879] = "Pausa";
	gstr[880] = "Magha";
	gstr[881] = "Phalguna";
	gstr[882] = "Caitra";
	gstr[883] = "Purusottama-adhika";

	gstr[884] = "Vaisakha (Madhusudana)";
	gstr[885] = "Jyestha (Trivikrama)";
	gstr[886] = "Asadha (Vamana)";
	gstr[887] = "Sravana (Sridhara)";
	gstr[888] = "Bhadra (Hrsikesa)";
	gstr[889] = "Asvina (Padmanabha)";
	gstr[890] = "Kartika (Damodara)";
	gstr[891] = "Margasirsa (Kesava)";
	gstr[892] = "Pausa (Narayana)";
	gstr[893] = "Magha (Madhava)";
	gstr[894] = "Phalguna (Govinda)";
	gstr[895] = "Caitra (Visnu)";
	gstr[896] = "Purusottama-adhika";

	gstr[897] = "Madhusudana (Vaisakha)";
	gstr[898] = "Trivikrama (Jyestha)";
	gstr[899] = "Vamana (Asadha)";
	gstr[900] = "Sridhara (Sravana)";
	gstr[901] = "Hrsikesa (Bhadra)";
	gstr[902] = "Padmanabha (Asvina)";
	gstr[903] = "Damodara (Kartika)";
	gstr[904] = "Kesava (Margasirsa)";
	gstr[905] = "Narayana (Pausa)";
	gstr[906] = "Madhava (Magha)";
	gstr[907] = "Govinda (Phalguna)";
	gstr[908] = "Visnu (Caitra)";
	gstr[909] = "Purusottama-adhika";

}

