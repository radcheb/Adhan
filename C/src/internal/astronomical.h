//
// Created by stormcaster on 22/06/17.
//

#ifndef C_ASTRONOMICAL_H
#define C_ASTRONOMICAL_H

#include <stdbool.h>
#include "coordinates.h"

double to_radius(double deg);

double to_degrees(double radians);

double meanSolarLongitude(double T);

double meanLunarLongitude(double T);

double apparentSolarLongitude(double T, double L0);

double ascendingLunarNodeLongitude(double T);

double meanSolarAnomaly(double T);

double solarEquationOfTheCenter(double T, double M);

double meanObliquityOfTheEcliptic(double T);

double apparentObliquityOfTheEcliptic(double T, double epsilon0);

double meanSiderealTime(double T);

double nutationInLongitude(double T, double L0, double Lp, double omega);

double nutationInObliquity(double T, double L0, double Lp, double omega);

double altitudeOfCelestialBody(double phi, double delta, double H);

double getApproximateTransit(double L, double theta0, double alpha2);

double correctedTransit(double m0, double L, double theta0, double alpha2, double alpha1, double alpha3);

double correctedHourAngle(double m0, double h0, const coordinates_t* coordinates, bool afterTransit,
                          double theta0, double alpha2, double alpha1, double alpha3, double delta2, double delta1, double delta3);
double interpolate(double y2, double y1, double y3, double n);

double interpolateAngles(double y2, double y1, double y3, double n);

#endif //C_ASTRONOMICAL_H
