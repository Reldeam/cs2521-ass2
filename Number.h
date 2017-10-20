/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#ifndef NUMBER_H
#define NUMBER_H

typedef struct Integer * Integer;
typedef struct Double * Double;

Integer newInteger(int value);
Double newDouble(double value);

int getInteger(Integer i);
double getDouble(Double d);

void setInteger(Integer i, int value);
void setDouble(Double d, double value);

void addInteger(Integer i, int amount);
void addDouble(Double d, double amount);

#endif