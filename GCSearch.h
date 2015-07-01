#pragma once
#include "GCFunctor.h"

class GCSearch
{
	bool was_error;
	GCFunctor * F;
	double precisionSec;
	int maxIters;
    double lastC;
    double lastValueC;
    double errorValue;
    bool useErrorValue;

public:
	GCSearch(GCFunctor * fnc);
	~GCSearch(void);

	double findMaximum(double from, double to);
	double findMinimum(double from, double to);
	double findRise(double from, double to, double limit);
	double findSet(double from, double to, double limit);
	double findAbove(double from, double to, double limit);
	double findBellow(double from, double to, double limit);

	void setSilentErrorValue(bool se, double val);
	bool wasError();

};

