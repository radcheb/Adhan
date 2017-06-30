/*
 * Created by stormcaster on 22/06/17.
 */

#include <time.h>
#include <stdlib.h>
#include "math.h"
#include "include/solar_time.h"
#include "include/data_components.h"
#include "include/calendrical_helper.h"
#include "include/astronomical.h"

inline solar_time_t *new_solar_time(struct tm *today, coordinates_t *coordinates){

    struct tm *tomorrow = add_yday(today, 1);

    struct tm *yesterday = add_yday(today, -2);

    solar_coordinates_t *prevSolar = new_solar_coordinates(julianDay2(yesterday));
    solar_coordinates_t *solar = new_solar_coordinates(julianDay2(today));
    solar_coordinates_t *nextSolar = new_solar_coordinates(julianDay2(tomorrow));

    double approximateTransit = getApproximateTransit(coordinates->longitude, solar->rightAscension,
                                                      solar->rightAscension);
    double solarAltitude = -50.0 / 60.0;

    coordinates_t *observer = coordinates;
    double transit = correctedTransit(approximateTransit, coordinates->longitude,
                                      solar->apparentSiderealTime, solar->rightAscension, prevSolar->rightAscension,
                                      nextSolar->rightAscension);
    double sunrise = correctedHourAngle(approximateTransit, solarAltitude,
                                              coordinates, false, solar->apparentSiderealTime, solar->rightAscension,
                                              prevSolar->rightAscension, nextSolar->rightAscension, solar->declination,
                                              prevSolar->declination, nextSolar->declination);
    double sunset = correctedHourAngle(approximateTransit, solarAltitude,
                                             coordinates, true, solar->apparentSiderealTime, solar->rightAscension,
                                             prevSolar->rightAscension, nextSolar->rightAscension, solar->declination,
                                             prevSolar->declination, nextSolar->declination);

    solar_time_t *result = malloc(sizeof(solar_time_t));
    *(double *)&result->transit = transit;
    *(double *)&result->sunrise = sunrise;
    *(double *)&result->sunset = sunset;
    *(coordinates_t **)&result->observer = observer;
    *(solar_coordinates_t **)&result->solar = solar;
    *(solar_coordinates_t **)&result->prevSolar = prevSolar;
    *(solar_coordinates_t **)&result->nextSolar = nextSolar;
    *(double *)&result->approximateTransit = approximateTransit;

    return result;
}

double hourAngle(solar_time_t *this_solar_time, double angle, bool afterTransit) {
    return correctedHourAngle(this_solar_time->approximateTransit, angle, this_solar_time->observer,
                              afterTransit, this_solar_time->solar->apparentSiderealTime,
                              this_solar_time->solar->rightAscension,
                              this_solar_time->prevSolar->rightAscension, this_solar_time->nextSolar->rightAscension,
                              this_solar_time->solar->declination,
                              this_solar_time->prevSolar->declination, this_solar_time->nextSolar->declination);

}

double afternoon(solar_time_t *this_solar_time, shadow_length shadowLength) {
    /* TODO (from Swift version) source shadow angle calculation */
    const double tangent = fabs(this_solar_time->observer->latitude - this_solar_time->solar->declination);
    const double inverse = shadowLength + tan(to_radius(tangent));
    const double angle = to_degrees(atan(1.0 / inverse));

    return hourAngle(this_solar_time, angle, true);
}