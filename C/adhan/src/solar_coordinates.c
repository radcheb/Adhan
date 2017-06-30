/*
 * Created by stormcaster on 22/06/17.
 */


#include <stdlib.h>
#include "include/solar_coordinates.h"
#include "include/calendrical_helper.h"
#include "include/astronomical.h"
#include "math.h"
#include "include/double_utils.h"

inline solar_coordinates_t *new_solar_coordinates(double julianDay){

    double T = julianCentury(julianDay);
    double L0 = meanSolarLongitude(/* julianCentury */ T);
    double Lp = meanLunarLongitude(/* julianCentury */ T);
    double omega = ascendingLunarNodeLongitude(/* julianCentury */ T);
    double lambda = to_radius(
            apparentSolarLongitude(/* julianCentury*/ T, /* meanLongitude */ L0));

    double theta0 = meanSiderealTime(/* julianCentury */ T);
    double delta_psi = nutationInLongitude(/* julianCentury */ T, /* solarLongitude */ L0,
            /* lunarLongitude */ Lp, /* ascendingNode */ omega);
    double delta_epsilon = nutationInObliquity(/* julianCentury */ T, /* solarLongitude */ L0,
            /* lunarLongitude */ Lp, /* ascendingNode */ omega);

    double epsilon0 = meanObliquityOfTheEcliptic(/* julianCentury */ T);
    double epsilonapp = to_radius(apparentObliquityOfTheEcliptic(
            /* julianCentury */ T, /* meanObliquityOfTheEcliptic */ epsilon0));

    /* Equation from Astronomical Algorithms page 165 */
    double declination = to_degrees(asin(sin(epsilonapp) * sin(lambda)));

    /* Equation from Astronomical Algorithms page 165 */
    double rightAscension = unwind_angle(
            to_degrees(atan2(cos(epsilonapp) * sin(lambda), cos(lambda))));

    /* Equation from Astronomical Algorithms page 88 */
    double apparentSiderealTime = theta0 + (((delta_psi * 3600) * cos(to_radius(epsilon0 + delta_epsilon))) / 3600);


    solar_coordinates_t* solar_coordinates = malloc(sizeof(solar_coordinates_t));
    *solar_coordinates = (solar_coordinates_t){declination, apparentSiderealTime, rightAscension};

    return solar_coordinates;
}
