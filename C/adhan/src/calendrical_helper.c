/*
 * Created by stormcaster on 22/06/17.
 */


#include "include/calendrical_helper.h"

double _julianDay(int year, int month, int day, double hours) {
    /* Equation from Astronomical Algorithms page 60 */

    // NOTE: Integer conversion is done intentionally for the purpose of decimal truncation

    int Y = month > 2 ? year : year - 1;
    int M = month > 2 ? month : month + 12;
    double D = day + (hours / 24);

    int A = Y/100;
    int B = 2 - A + (A/4);

    int i0 = (int) (365.25 * (Y + 4716));
    int i1 = (int) (30.6001 * (M + 1));
    return i0 + i1 + D + B - 1524.5;
}

double julianDay(int year, int month, int day) {
    return _julianDay(year, month, day, 0.0);
}

double julianDay2(const struct tm* date) {
    return _julianDay(date->tm_year + 1900,
                     date->tm_mon + 1, date->tm_mday,
                     date->tm_hour + date->tm_min / 60.0);
}

double julianCentury(double JD) {
    /* Equation from Astronomical Algorithms page 163 */
    return (JD - 2451545.0) / 36525;
}