#pragma once
class GCMeeusEarth
{
public:
	GCMeeusEarth(void);
	~GCMeeusEarth(void);

	static double vsop87_ear_r0[][3];
	static double vsop87_ear_r1[][3];
	static double vsop87_ear_r2[][3];
	static double vsop87_ear_r3[][3];
	static double vsop87_ear_r4[][3];
	static double vsop87_ear_r5[][3];

	static double vsop87_ear_b0[][3];
	static double vsop87_ear_b1[][3];
	static double vsop87_ear_b2[][3];
	static double vsop87_ear_b3[][3];
	static double vsop87_ear_b4[][3];
	static double vsop87_ear_b5[][3];

	static double vsop87_ear_l0[][3];
	static double vsop87_ear_l1[][3];
	static double vsop87_ear_l2[][3];
	static double vsop87_ear_l3[][3];
	static double vsop87_ear_l4[][3];
	static double vsop87_ear_l5[][3];

	static double latitudeFactorCount(int a);
	static double * latitudeFactor(int a, int b);
	static double longitudeFactorCount(int a);
	static double * longitudeFactor(int a, int b);
	static double radiusFactorCount(int a);
	static double * radiusFactor(int a, int b);

};

