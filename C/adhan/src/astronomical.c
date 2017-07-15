/*
 * Created by stormcaster on 22/06/17.
 */


#include "include/double_utils.h"
#include "include/astronomical.h"

double to_radius(double deg) {
    return deg * ( M_PI/ 180.0 );
}

double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

double meanSolarLongitude(double T) {
    /* Equation from Astronomical Algorithms page 163 */
    const double term1 = 280.4664567;
    const double term2 = 36000.76983 * T;
    const double term3 = 0.0003032 * pow(T, 2);
    const double L0 = term1 + term2 + term3;
    return unwind_angle(L0);
}

double meanLunarLongitude(double T) {
    /* Equation from Astronomical Algorithms page 144 */
    const double term1 = 218.3165;
    const double term2 = 481267.8813 * T;
    const double Lp = term1 + term2;
    return unwind_angle(Lp);
}

double apparentSolarLongitude(double T, double L0) {
    /* Equation from Astronomical Algorithms page 164 */
    const double longitude = L0 + solarEquationOfTheCenter(T, meanSolarAnomaly(T));
    const double omega = 125.04 - (1934.136 * T);
    const double lambda = longitude - 0.00569 - (0.00478 * sin(to_radius(omega)));
    return unwind_angle(lambda);
}

double ascendingLunarNodeLongitude(double T) {
    /* Equation from Astronomical Algorithms page 144 */
    const double term1 = 125.04452;
    const double term2 = 1934.136261 * T;
    const double term3 = 0.0020708 * pow(T, 2);
    const double term4 = pow(T, 3) / 450000;
    const double omega = term1 - term2 + term3 + term4;
    return unwind_angle(omega);
}

double meanSolarAnomaly(double T) {
    /* Equation from Astronomical Algorithms page 163 */
    const double term1 = 357.52911;
    const double term2 = 35999.05029 * T;
    const double term3 = 0.0001537 * pow(T, 2);
    const double M = term1 + term2 - term3;
    return unwind_angle(M);
}

double solarEquationOfTheCenter(double T, double M) {
    /* Equation from Astronomical Algorithms page 164 */
    const double Mrad = to_radius(M);
    const double term1 = (1.914602 - (0.004817 * T) - (0.000014 * pow(T, 2))) * sin(Mrad);
    const double term2 = (0.019993 - (0.000101 * T)) * sin(2 * Mrad);
    const double term3 = 0.000289 * sin(3 * Mrad);
    return term1 + term2 + term3;
}

double meanObliquityOfTheEcliptic(double T) {
    /* Equation from Astronomical Algorithms page 147 */
    const double term1 = 23.439291;
    const double term2 = 0.013004167 * T;
    const double term3 = 0.0000001639 * pow(T, 2);
    const double term4 = 0.0000005036 * pow(T, 3);
    return term1 - term2 - term3 + term4;
}

double apparentObliquityOfTheEcliptic(double T, double epsilon0) {
    /* Equation from Astronomical Algorithms page 165 */
    const double O = 125.04 - (1934.136 * T);
    return epsilon0 + (0.00256 * cos(to_radius(O)));
}

double meanSiderealTime(double T) {
    /* Equation from Astronomical Algorithms page 165 */
    const double JD = (T * 36525) + 2451545.0;
    const double term1 = 280.46061837;
    const double term2 = 360.98564736629 * (JD - 2451545);
    const double term3 = 0.000387933 * pow(T, 2);
    const double term4 = pow(T, 3) / 38710000;
    const double theta = term1 + term2 + term3 - term4;
    return unwind_angle(theta);
}

double nutationInLongitude(double T, double L0, double Lp, double omega) {
    /* Equation from Astronomical Algorithms page 144 */
    const double term1 = (-17.2/3600) * sin(to_radius(omega));
    const double term2 =  (1.32/3600) * sin(2 * to_radius(L0));
    const double term3 =  (0.23/3600) * sin(2 * to_radius(Lp));
    const double term4 =  (0.21/3600) * sin(2 * to_radius(omega));
    return term1 - term2 - term3 + term4;
}

double nutationInObliquity(double T, double L0, double Lp, double omega) {
    /* Equation from Astronomical Algorithms page 144 */
    const double term1 =  (9.2/3600) * cos(to_radius(omega));
    const double term2 = (0.57/3600) * cos(2 * to_radius(L0));
    const double term3 = (0.10/3600) * cos(2 * to_radius(Lp));
    const double term4 = (0.09/3600) * cos(2 * to_radius(omega));
    return term1 + term2 + term3 - term4;
}

double altitudeOfCelestialBody(double phi, double delta, double H) {
    /* Equation from Astronomical Algorithms page 93 */
    const double term1 = sin(to_radius(phi)) * sin(to_radius(delta));
    const double term2 = cos(to_radius(phi)) *
                         cos(to_radius(delta)) * cos(to_radius(H));
    return to_degrees(asin(term1 + term2));
}

double getApproximateTransit(double L, double theta0, double alpha2) {
    /* Equation from page Astronomical Algorithms 102 */
    const double Lw = L * -1;
    return normalize_with_bound((alpha2 + Lw - theta0) / 360, 1);
}

double correctedTransit(double m0, double L, double theta0, double alpha2, double alpha1, double alpha3) {
    /* Equation from page Astronomical Algorithms 102 */
    const double Lw = L * -1;
    const double theta = unwind_angle(theta0 + (360.985647 * m0));
    const double alpha = unwind_angle(interpolateAngles(
            /* value */ alpha2, /* previousValue */ alpha1, /* nextValue */ alpha3, /* factor */ m0));
    const double H = closest_angle(theta - Lw - alpha);
    const double deltam = H / -360;
    return (m0 + deltam) * 24;
}

double correctedHourAngle(double m0, double h0, const coordinates_t* coordinates, bool afterTransit,
                          double theta0, double alpha2, double alpha1, double alpha3, double delta2, double delta1, double delta3) {
    /* Equation from page Astronomical Algorithms 102 */
    const double Lw = coordinates->longitude * -1;
    const double term1 = sin(to_radius(h0)) -
                         (sin(to_radius(coordinates->latitude)) * sin(to_radius(delta2)));
    const double term2 = cos(to_radius(coordinates->latitude)) * cos(to_radius(delta2));
    const double H0 = to_degrees(acos(term1 / term2));
    const double m = afterTransit ? m0 + (H0 / 360) : m0 - (H0 / 360);
    const double theta = unwind_angle(theta0 + (360.985647 * m));
    const double alpha = unwind_angle(interpolateAngles(
            /* value */ alpha2, /* previousValue */ alpha1, /* nextValue */ alpha3, /* factor */ m));
    const double delta = interpolate(/* value */ delta2, /* previousValue */ delta1,
            /* nextValue */ delta3, /* factor */ m);
    const double H = (theta - Lw - alpha);
    const double h = altitudeOfCelestialBody(/* observerLatitude */ coordinates->latitude,
            /* declination */ delta, /* localHourAngle */ H);
    const double term3 = h - h0;
    const double term4 = 360 * cos(to_radius(delta)) *
                         cos(to_radius(coordinates->latitude)) * sin(to_radius(H));
    const double deltam = term3 / term4;
    return (m + deltam) * 24;
}

double interpolate(double y2, double y1, double y3, double n) {
    /* Equation from Astronomical Algorithms page 24 */
    const double a = y2 - y1;
    const double b = y3 - y2;
    const double c = b - a;
    return y2 + ((n/2) * (a + b + (n * c)));
}

double interpolateAngles(double y2, double y1, double y3, double n) {
    /* Equation from Astronomical Algorithms page 24 */
    const double a = unwind_angle(y2 - y1);
    const double b = unwind_angle(y3 - y2);
    const double c = b - a;
    return y2 + ((n/2) * (a + b + (n * c)));
}