/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_CALENDRICAL_HELPER_H
#define C_CALENDRICAL_HELPER_H

#include <time.h>

// TODO to implement
double _julianDay(int year, int month, int day, double hours);

double julianDay(int year, int month, int day);

double julianDay2(struct tm* date);

double julianCentury(double JD);
#endif //C_CALENDRICAL_HELPER_H
