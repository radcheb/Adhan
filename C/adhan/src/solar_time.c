/*
 * Created by stormcaster on 22/06/17.
 */

#include <time.h>
#include <solar_time.h>
#include "math.h"

solar_time_t new_solar_time(const time_t today_time, coordinates_t *coordinates) {


    const time_t tomorrow_time = add_yday(today_time, 1);
    const time_t yesterday_time = add_yday(today_time, -1);

    solar_coordinates_t solar = new_solar_coordinates(julianDay2(localtime(&today_time)));
    solar_coordinates_t prevSolar = new_solar_coordinates(julianDay2(localtime(&yesterday_time)));
    solar_coordinates_t nextSolar = new_solar_coordinates(julianDay2(localtime(&tomorrow_time)));

    double approximateTransit = getApproximateTransit(coordinates->longitude, solar.apparentSiderealTime,
                                                      solar.rightAscension);
    double solarAltitude = -50.0 / 60.0;

    coordinates_t *observer = coordinates;
    double transit = correctedTransit(approximateTransit, coordinates->longitude,
                                      solar.apparentSiderealTime, solar.rightAscension, prevSolar.rightAscension,
                                      nextSolar.rightAscension);
    double sunrise = correctedHourAngle(approximateTransit, solarAltitude,
                                        coordinates, false, solar.apparentSiderealTime, solar.rightAscension,
                                        prevSolar.rightAscension, nextSolar.rightAscension, solar.declination,
                                        prevSolar.declination, nextSolar.declination);
    double sunset = correctedHourAngle(approximateTransit, solarAltitude,
                                       coordinates, true, solar.apparentSiderealTime, solar.rightAscension,
                                       prevSolar.rightAscension, nextSolar.rightAscension, solar.declination,
                                       prevSolar.declination, nextSolar.declination);

    solar_time_t result = (solar_time_t) {transit, sunrise, sunset, observer, solar, prevSolar, nextSolar,
                                          approximateTransit};

    return result;
}

double hourAngle(solar_time_t *this_solar_time, double angle, bool afterTransit) {
    return correctedHourAngle(this_solar_time->approximateTransit, angle, this_solar_time->observer,
                              afterTransit, this_solar_time->solar.apparentSiderealTime,
                              this_solar_time->solar.rightAscension,
                              this_solar_time->prevSolar.rightAscension, this_solar_time->nextSolar.rightAscension,
                              this_solar_time->solar.declination,
                              this_solar_time->prevSolar.declination, this_solar_time->nextSolar.declination);

}

double afternoon(solar_time_t *this_solar_time, shadow_length shadowLength) {
    /* TODO (from Swift version) source shadow angle calculation */
    const double tangent = fabs(this_solar_time->observer->latitude - this_solar_time->solar.declination);
    const double inverse = shadowLength + tan(to_radius(tangent));
    const double angle = to_degrees(atan(1.0 / inverse));

    return hourAngle(this_solar_time, angle, true);
}
