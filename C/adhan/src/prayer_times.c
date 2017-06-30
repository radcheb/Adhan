/*
 * Created by stormcaster on 22/06/17.
 */


#include "math.h"
#include "include/prayer_times.h"
#include "include/solar_time.h"

inline prayer_times_t *
new_prayer_times(coordinates_t *coordinates, date_components *date, calculation_parameters_t *params) {
    return new_prayer_times2(coordinates, resolve_time(date), params);
}

prayer_times_t *new_prayer_times2(coordinates_t *coordinates, struct tm *date, calculation_parameters_t *parameters) {
    struct tm *tempFajr = NULL;
    struct tm *tempSunrise = NULL;
    struct tm *tempDhuhr = NULL;
    struct tm *tempAsr = NULL;
    struct tm *tempMaghrib = NULL;
    struct tm *tempIsha = NULL;

    const int year = date->tm_year;
    const int dayOfYear = date->tm_yday;

    solar_time_t *solar_time = new_solar_time(date, coordinates);

    time_components *time_components1 = from_double(solar_time->transit);
    struct tm *transit = time_components1 ? get_date_components(date) : NULL;

    time_components1 = from_double(solar_time->sunrise);
    struct tm *sunriseComponents = time_components1 ? get_date_components(date) : NULL;

    time_components1 = from_double(solar_time->sunset);
    struct tm *sunsetComponents = time_components1 ? get_date_components(date) : NULL;

    bool error = !(transit && sunriseComponents && sunsetComponents);

    if (!error) {
        tempDhuhr = transit;
        tempSunrise = sunriseComponents;
        tempMaghrib = sunsetComponents;

        time_components1 = from_double(afternoon(solar_time, getShadowLength(*(parameters->madhab))));
        if (time_components1) {
            tempAsr = get_date_components(date);
        }

        // get night length
        struct tm *tomorrowSunrise = add_yday(sunriseComponents, 1);
        long night = (long) mktime(tomorrowSunrise) - mktime(sunsetComponents);

        time_components1 = from_double(hourAngle(solar_time, -parameters->fajrAngle, false));
        if (time_components1) {
            tempFajr = get_date_components(date);
        }

        if (*(parameters->method) == MOON_SIGHTING_COMMITTEE && coordinates->latitude >= 55) {
            tempFajr = add_seconds(sunriseComponents, -1 * (int) (night / 7000));
        }

        const night_portions_t *nightPortions = get_night_portions(parameters);

        struct tm *safeFajr;
        if (*(parameters->method) == MOON_SIGHTING_COMMITTEE) {
            safeFajr = seasonAdjustedEveningTwilight(coordinates->latitude, dayOfYear, year, sunriseComponents);
        } else {
            double portion = nightPortions->fajr;
            long nightFraction = (long) (portion * night / 1000);
            safeFajr = add_seconds(sunriseComponents, -1 * (int) nightFraction);
        }

        if (!tempFajr || difftime(mktime(safeFajr), mktime(tempFajr)) > 0) {
            tempFajr = safeFajr;
        }

        // Isha calculation with check against safe value
        if (parameters->ishaInterval > 0) {
            tempIsha = add_seconds(tempMaghrib, parameters->ishaInterval * 60);
        } else {
            time_components1 = from_double(hourAngle(solar_time, -parameters->ishaAngle, true));
            if (time_components1) {
                tempIsha = get_date_components(date);
            }

            if (*(parameters->method) == MOON_SIGHTING_COMMITTEE && coordinates->latitude >= 55) {
                long nightFraction = night / 7000;
                tempIsha = add_seconds(sunsetComponents, (int) nightFraction);
            }

            struct tm *safeIsha;
            if (*(parameters->method) == MOON_SIGHTING_COMMITTEE) {
                safeIsha = seasonAdjustedEveningTwilight(coordinates->latitude, dayOfYear, year, sunsetComponents);
            } else {
                double portion = nightPortions->isha;
                long nightFraction = (long) (portion * night / 1000);
                safeIsha = add_seconds(sunsetComponents, (int) nightFraction);
            }

            if (!tempIsha || difftime(mktime(safeIsha), mktime(tempIsha)) < 0) {
                tempIsha = safeIsha;
            }
        }
    }

    // method based offsets
    int dhuhrOffsetInMinutes;
    if (*(parameters->method) == MOON_SIGHTING_COMMITTEE) {
        // Moonsighting Committee requires 5 minutes for the sun to pass
        // the zenith and dhuhr to enter
        dhuhrOffsetInMinutes = 5;
    } else if (*(parameters->method) == UMM_AL_QURA ||
               *(parameters->method) == GULF ||
               *(parameters->method) == QATAR) {
        // UmmAlQura and derivatives don't add
        // anything to zenith for dhuhr
        dhuhrOffsetInMinutes = 0;
    } else {
        dhuhrOffsetInMinutes = 1;
    }

    int maghribOffsetInMinutes;
    if (*(parameters->method) == MOON_SIGHTING_COMMITTEE) {
        // Moonsighting Committee adds 3 minutes to sunset time to account for light refraction
        maghribOffsetInMinutes = 3;
    } else {
        maghribOffsetInMinutes = 0;

    }

    if (error || !tempAsr) {
        // if we don't have all prayer times then initialization failed
        return (prayer_times_t *) (NULL, NULL, NULL, NULL, NULL, NULL);
    } else {
        prayer_times_t *prayer_times = (prayer_times_t *) malloc(sizeof(prayer_times_t));
        *(struct tm **) &prayer_times->fajr = round_minute(add_minutes(tempFajr, parameters->adjustments->fajr));
        *(struct tm **) &prayer_times->sunrise = round_minute(
                add_minutes(tempSunrise, parameters->adjustments->sunrise));
        *(struct tm **) &prayer_times->dhuhr = round_minute(
                add_minutes(tempDhuhr, parameters->adjustments->dhuhr + dhuhrOffsetInMinutes));
        *(struct tm **) &prayer_times->asr = round_minute(add_minutes(tempAsr, parameters->adjustments->asr));
        *(struct tm **) &prayer_times->maghrib = round_minute(
                add_minutes(tempMaghrib, parameters->adjustments->maghrib + maghribOffsetInMinutes));
        *(struct tm **) &prayer_times->isha = round_minute(add_minutes(tempIsha, parameters->adjustments->isha));

        return prayer_times;
    }
}

prayer_t *currentPrayer(prayer_times_t *prayer_times) {
    time_t now = time(0);
    struct tm *today = gmtime(&now);
    return currentPrayer2(prayer_times, today);
}

prayer_t *currentPrayer2(prayer_times_t *prayer_times, struct tm *time) {
    long when = mktime(time);
    if (mktime((struct tm*)prayer_times->isha) - when <= 0) {
        return init_prayer(ISHA);
    } else if (mktime((struct tm*)prayer_times->maghrib) - when <= 0) {
        return init_prayer(MAGHRIB);
    } else if (mktime((struct tm*)prayer_times->asr) - when <= 0) {
        return init_prayer(ASR);
    } else if (mktime((struct tm*)prayer_times->dhuhr) - when <= 0) {
        return init_prayer(DHUHR);
    } else if (mktime((struct tm*)prayer_times->sunrise) - when <= 0) {
        return init_prayer(SUNRISE);
    } else if (mktime((struct tm*)prayer_times->fajr) - when <= 0) {
        return init_prayer(FAJR);
    } else {
        return init_prayer(NONE);
    }
}

prayer_t *next_prayer(prayer_times_t *prayer_times) {
    time_t now = time(0);
    struct tm *today = gmtime(&now);
    return next_prayer2(prayer_times, today);
}

prayer_t *next_prayer2(prayer_times_t *prayer_times, struct tm *time) {
    long when = mktime(time);
    if (mktime((struct tm*)prayer_times->isha) - when <= 0) {
        return init_prayer(NONE);
    } else if (mktime((struct tm*)prayer_times->maghrib) - when <= 0) {
        return init_prayer(ISHA);
    } else if (mktime((struct tm*)prayer_times->asr) - when <= 0) {
        return init_prayer(MAGHRIB);
    } else if (mktime((struct tm*)prayer_times->dhuhr) - when <= 0) {
        return init_prayer(ASR);
    } else if (mktime((struct tm*)prayer_times->sunrise) - when <= 0) {
        return init_prayer(DHUHR);
    } else if (mktime((struct tm*)prayer_times->fajr) - when <= 0) {
        return init_prayer(SUNRISE);
    } else {
        return init_prayer(FAJR);
    }
}

struct tm *timeForPrayer(prayer_times_t *prayer_times, prayer_t *prayer) {
    switch (*prayer) {
        case FAJR:
            return (struct tm*)(prayer_times->fajr);
        case SUNRISE:
            return (struct tm*)(prayer_times->sunrise);
        case DHUHR:
            return (struct tm*)(prayer_times->dhuhr);
        case ASR:
            return (struct tm*)(prayer_times->asr);
        case MAGHRIB:
            return (struct tm*)(prayer_times->maghrib);
        case ISHA:
            return (struct tm*)(prayer_times->isha);
        case NONE:
            return NULL;
        default:
            return NULL;
    }
}

struct tm *seasonAdjustedMorningTwilight(double latitude, int day, int year, struct tm *sunrise) {
    const double a = 75 + ((28.65 / 55.0) * fabs(latitude));
    const double b = 75 + ((19.44 / 55.0) * fabs(latitude));
    const double c = 75 + ((32.74 / 55.0) * fabs(latitude));
    const double d = 75 + ((48.10 / 55.0) * fabs(latitude));

    double adjustment;
    const int dyy = daysSinceSolstice(day, year, latitude);
    if (dyy < 91) {
        adjustment = a + (b - a) / 91.0 * dyy;
    } else if (dyy < 137) {
        adjustment = b + (c - b) / 46.0 * (dyy - 91);
    } else if (dyy < 183) {
        adjustment = c + (d - c) / 46.0 * (dyy - 137);
    } else if (dyy < 229) {
        adjustment = d + (c - d) / 46.0 * (dyy - 183);
    } else if (dyy < 275) {
        adjustment = c + (b - c) / 46.0 * (dyy - 229);
    } else {
        adjustment = b + (a - b) / 91.0 * (dyy - 275);
    }

    return add_seconds(sunrise, -(int) round(adjustment * 60.0));
}

struct tm *seasonAdjustedEveningTwilight(double latitude, int day, int year, struct tm *sunset) {
    const double a = 75 + ((25.60 / 55.0) * fabs(latitude));
    const double b = 75 + ((2.050 / 55.0) * fabs(latitude));
    const double c = 75 - ((9.210 / 55.0) * fabs(latitude));
    const double d = 75 + ((6.140 / 55.0) * fabs(latitude));

    double adjustment;
    int dyy = daysSinceSolstice(day, year, latitude);
    if (dyy < 91) {
        adjustment = a + (b - a) / 91.0 * dyy;
    } else if (dyy < 137) {
        adjustment = b + (c - b) / 46.0 * (dyy - 91);
    } else if (dyy < 183) {
        adjustment = c + (d - c) / 46.0 * (dyy - 137);
    } else if (dyy < 229) {
        adjustment = d + (c - d) / 46.0 * (dyy - 183);
    } else if (dyy < 275) {
        adjustment = c + (b - c) / 46.0 * (dyy - 229);
    } else {
        adjustment = b + (a - b) / 91.0 * (dyy - 275);
    }

    return add_seconds(sunset, (int) round(adjustment * 60.0));

}

int daysSinceSolstice(int dayOfYear, int year, double latitude) {
    int daysSinceSolistice;
    const int northernOffset = 10;
    bool isLeapYear = is_leap_year(year);
    const int southernOffset = isLeapYear ? 173 : 172;
    const int daysInYear = isLeapYear ? 366 : 365;

    if (latitude >= 0) {
        daysSinceSolistice = dayOfYear + northernOffset;
        if (daysSinceSolistice >= daysInYear) {
            daysSinceSolistice = daysSinceSolistice - daysInYear;
        }
    } else {
        daysSinceSolistice = dayOfYear - southernOffset;
        if (daysSinceSolistice < 0) {
            daysSinceSolistice = daysSinceSolistice + daysInYear;
        }
    }
    return daysSinceSolistice;

}
