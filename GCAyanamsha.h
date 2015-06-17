#pragma once
class GCAyanamsha
{
public:
	static int ayamashaType;
	GCAyanamsha(void);
	~GCAyanamsha(void);
	static double GetAyanamsa(double);
	static int GetAyanamsaType();
	static int SetAyanamsaType(int i);
	static const char * GetAyanamsaName(int nType);
	static double GetLahiriAyanamsa(double d);
};

