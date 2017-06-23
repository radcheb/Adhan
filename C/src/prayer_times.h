//
// Created by stormcaster on 22/06/17.
//

#include "data_components.h"
#include "internal/coordinates.h"
#include "data_components.h"
#include "calculation_parameters.h"
#include "prayer.h"

#ifndef C_PRAYER_TIMES_H
#define C_PRAYER_TIMES_H

typedef struct {
    const struct tm *fajr;
    const struct tm *sunrise;
    const struct tm *dhuhr;
    const struct tm *asr;
    const struct tm *maghrib;
    const struct tm *isha;
} prayer_times_t;

inline prayer_times_t* new_prayer_times(coordinates_t* coordinates, date_components* date, calculation_parameters_t* params);

prayer_times_t* new_prayer_times2(coordinates_t* coordinates, struct tm* date, calculation_parameters_t* parameters);

prayer_t* currentPrayer(prayer_times_t* prayer_times);

prayer_t* currentPrayer2(prayer_times_t* prayer_times, struct tm* time);

prayer_t* next_prayer(prayer_times_t* prayer_times);

prayer_t* next_prayer2(prayer_times_t* prayer_times, struct tm* time);

struct tm* timeForPrayer(prayer_times_t* prayer_times, prayer_t* prayer);

struct tm* seasonAdjustedMorningTwilight(double latitude, int day, int year, struct tm* sunrise);

struct tm* seasonAdjustedEveningTwilight(double latitude, int day, int year, struct tm* sunset);

int daysSinceSolstice(int dayOfYear, int year, double latitude);
#endif //C_PRAYER_TIMES_H
