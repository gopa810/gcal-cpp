#include "stdafx.h"
#include "GCSearch.h"
#include "GCMath.h"

GCSearch::GCSearch(GCFunctor * fnc)
{
	F = fnc;
	precisionSec = 1;
	maxIters = 50;
    errorValue = 0;
    useErrorValue = false;
    was_error = false;
}


GCSearch::~GCSearch(void)
{
}

void GCSearch::setSilentErrorValue(bool se, double val)
{
    useErrorValue = se;
    errorValue = val;
}

bool GCSearch::wasError()
{
	return was_error;
}

double GCSearch::findMaximum(double from, double to)
{
    was_error = false;
    double a = from;
    double b = to;
    int iterc = 0;
    int iterd = 0;
    double c, d;

    double vala = F->getDoubleValue(a);
    double valb = F->getDoubleValue(b);
    double valc, vald;

    while (GCMath::Abs(a - b) * 86400 > precisionSec && (iterc + iterd) < maxIters)
    {
        c = (2*a + b) / 3;
        valc = F->getDoubleValue(c);

        d = (a + 2 * b) / 3;
        vald = F->getDoubleValue(d);

        if (valc < vald)
        {
            a = c;
            vala = valc;
            iterc++;
        }
        else if (valc > vald)
        {
            b = d;
            valb = vald;
            iterd++;
        }
    }

    lastC = (a + b) / 2;
    lastValueC = GCMath::Max(vala, valb);
    if (iterd == 0 || iterc == 0)
    {
        if (useErrorValue)
        {
            was_error = true;
	        return errorValue;
        }
        throw "Function has no maximum peak in given interval";
    }
    return lastC;
}

double GCSearch::findMinimum(double from, double to)
{
    was_error = false;
    double a = from;
    double b = to;
    int iterc = 0;
    int iterd = 0;
	int itere = 0;
    double c, d;

    double vala = F->getDoubleValue(a);
    double valb = F->getDoubleValue(b);
    double valc, vald;

    while (GCMath::Abs(a - b) * 86400 > precisionSec && (iterc + iterd) < maxIters)
    {
        c = (2 * a + b) / 3;
        valc = F->getDoubleValue(c);

        d = (a + 2 * b) / 3;
        vald = F->getDoubleValue(d);

        if (valc > vald)
        {
            a = c;
            vala = valc;
            iterc++;
        }
        else if (valc < vald)
        {
            b = d;
            valb = vald;
            iterd++;
        }
		else
		{
			break;
		}
    }
    lastC = (a + b) / 2;
    lastValueC = GCMath::Min(vala, valb);
    if (iterd == 0 || iterc == 0)
    {
        if (useErrorValue)
        {
            was_error = true;
            return errorValue;
        }
        throw "Function has no minumum peak in given interval";
    }
    return lastC;
}

/// <summary>
/// There is assumption, that there is only 1 rise in given x interval
/// It is not defined, which rise will be resurned, if there are more than 1 rises, 
/// </summary>
/// <param name="from">start of time interval</param>
/// <param name="to">end of time interval</param>
/// <param name="limit">value for testing</param>
/// <returns>returns time of next rise or -1 for not found</returns>
double GCSearch::findRise(double from, double to, double limit)
{
    was_error = false;
    double a = findBellow(from, to, limit);
    if (was_error)
        return errorValue;
    double b = findAbove(a, to, limit);
    int iters = 0;
    if (was_error)
        return errorValue;

    double vala = F->getDoubleValue(a);
    double valb = F->getDoubleValue(b);

    while (GCMath::Abs(a - b)*86400 > precisionSec && iters < maxIters)
    {
        lastC = ( a + b ) / 2;
        lastValueC = F->getDoubleValue(lastC);

        if (lastValueC < limit)
        {
            a = lastC;
            vala = lastValueC;
        }
        else if (lastValueC > limit)
        {
            b = lastC;
            valb = lastValueC;
        }
        iters++;
    }

    return lastC;
}

double GCSearch::findSet(double from, double to, double limit)
{
    was_error = false;
    double a = findAbove(from, to, limit);
    if (was_error)
        return errorValue;
    double b = findBellow(a, to, limit);
    int iters = 0;
    if (was_error)
        return errorValue;

    double vala = F->getDoubleValue(a);
    double valb = F->getDoubleValue(b);

    while (GCMath::Abs(a - b) * 86400 > precisionSec && iters < maxIters)
    {
        lastC = (a + b) / 2;
        lastValueC = F->getDoubleValue(lastC);

        if (lastValueC > limit)
        {
            a = lastC;
            vala = lastValueC;
        }
        else if (lastValueC < limit)
        {
            b = lastC;
            valb = lastValueC;
        }
        iters++;
    }

    return lastC;
}

/// <summary>
/// This functions finds first X where value(x) is abouve given limit.
/// Difference with rise is, that this function gives first value found
/// above that limit, so it is not precise moment, when func(x)
/// has rose above given limit.
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="limit"></param>
/// <returns></returns>
double GCSearch::findAbove(double from, double to, double limit)
{
    if (F->getDoubleValue(from) > limit)
        return from;
    if (F->getDoubleValue(to) > limit)
        return to;

    double start = from;
    double step = (to - from) / 2;
    int steps = 1;
    double x;

    for (int i = 0; i < 10; i++)
    {
        x = start;
        for(int a = 0; a < steps; a++)
        {
            x += step;
            if (F->getDoubleValue(x) > limit)
                return x;
        }

        start = (start + from) / 2;
        step /= 2;
        steps *= 2;
    }

    throw "next value above limit not found";
}

/// <summary>
/// This functions finds first X where value(x) is bellow given limit.
/// Difference with findNextSet is, that this function gives first value found
/// bellow that limit, so it is not precise moment, when func(x)
/// has set bellow given limit.
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="limit"></param>
/// <returns></returns>
double GCSearch::findBellow(double from, double to, double limit)
{
    if (F->getDoubleValue(from) < limit)
        return from;
    if (F->getDoubleValue(to) > limit)
        return to;

    double start = (from + to) / 2;
    double step = (to - from) / 2;
    int steps = 1;
    double x;

    for (int i = 0; i < 10; i++)
    {
        x = start;
        for (int a = 0; a < steps; a++)
        {
            x += step;
            if (F->getDoubleValue(x) < limit)
                return x;
        }

        start = (start + from) / 2;
        step /= 2;
        steps *= 2;
    }

    throw "next value bellow limit not found";
}
