/*
 * Created by stormcaster on 22/06/17.
 */


#include "data_components.h"
#include "coordinates.h"
#include "data_components.h"
#include "calculation_parameters.h"
#include "prayer.h"

#ifndef C_PRAYER_TIMES_H
#define C_PRAYER_TIMES_H

typedef struct {
    struct tm *fajr;
    struct tm *sunrise;
    struct tm *dhuhr;
    struct tm *asr;
    struct tm *maghrib;
    struct tm *isha;
} prayer_times_t;

prayer_times_t* new_prayer_times(coordinates_t* coordinates, date_components_t* date, calculation_parameters_t* params);

prayer_times_t* new_prayer_times2(coordinates_t* coordinates, time_t date, calculation_parameters_t* parameters);

prayer_t currentPrayer(prayer_times_t* prayer_times);

prayer_t currentPrayer2(prayer_times_t* prayer_times, time_t when);

prayer_t next_prayer(prayer_times_t* prayer_times);

prayer_t next_prayer2(prayer_times_t* prayer_times, time_t when);

time_t timeForPrayer(prayer_times_t* prayer_times, prayer_t prayer);

time_t seasonAdjustedMorningTwilight(double latitude, int day, int year, time_t sunrise);

time_t seasonAdjustedEveningTwilight(double latitude, int day, int year, time_t sunset);

int daysSinceSolstice(int dayOfYear, int year, double latitude);
#endif //C_PRAYER_TIMES_H
