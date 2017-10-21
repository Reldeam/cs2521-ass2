/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#include <stdlib.h>

#include "Number.h"

struct Integer {
    int value;
};

struct Double {
    double value;
};

Integer newInteger(int value)
{
    Integer i = malloc(sizeof(struct Integer));
    i->value = value;
    return i;
}

int getInteger(Integer i)
{
	if(!i) return 0;
	return i->value;
}

void setInteger(Integer i, int value)
{
	i->value = value;
}

void addInteger(Integer i, int amount)
{
	i->value += amount;
}

Double newDouble(double value)
{
    Double d = malloc(sizeof(struct Double));
    d->value = value;
    return d;
}

double getDouble(Double d)
{
	if(!d) return 0;
	return d->value;
}

void setDouble(Double d, double value)
{
	d->value = value;
}

void addDouble(Double d, double amount)
{
	d->value += amount;
}