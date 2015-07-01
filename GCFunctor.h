#pragma once


class GCFunctor
{
public:
	GCFunctor(void);
	~GCFunctor(void);

	virtual double getDoubleValue(double jdate) = 0;
};

