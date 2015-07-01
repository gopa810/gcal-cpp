#pragma once
class GCDynamicTime
{
public:
	GCDynamicTime(void);
	~GCDynamicTime(void);
	static double epoch1[];
	static double epoch2[];
	static double epoch3[];

	static double getUniversalTimeFromDynamicTime(double jd);
	static double getDynamicTimeFromUniversalTime(double j);
	static double GetDeltaT(double julian);
};

