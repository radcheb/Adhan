/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_SOLAR_TIME_H
#define C_SOLAR_TIME_H

#include <stdbool.h>
#include "coordinates.h"
#include "solar_coordinates.h"
#include "shadow.h"
#include "data_components.h"
#include "calendrical_helper.h"
#include "astronomical.h"

typedef struct {
    const double transit;
    const double sunrise;
    const double sunset;
    const coordinates_t *observer;
    solar_coordinates_t solar;
    solar_coordinates_t prevSolar;
    solar_coordinates_t nextSolar;
    double approximateTransit;
} solar_time_t;

solar_time_t new_solar_time(const time_t today, coordinates_t *coordinates);

double hourAngle(solar_time_t *this_solar_time, double angle, bool afterTransit);

double afternoon(solar_time_t *this_solar_time, shadow_length shadowLength);

#endif //C_SOLAR_TIME_H
