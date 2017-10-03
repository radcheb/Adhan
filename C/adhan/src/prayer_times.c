/*
 * Created by stormcaster on 22/06/17.
 */


#include <calculation_parameters.h>
#include "math.h"
#include "include/prayer_times.h"
#include "include/solar_time.h"

inline prayer_times_t
new_prayer_times(coordinates_t *coordinates, date_components_t *date, calculation_parameters_t *params) {
    return new_prayer_times2(coordinates, resolve_time(date), params);
}

prayer_times_t new_prayer_times2(coordinates_t *coordinates, time_t date, calculation_parameters_t *parameters) {
    time_t tempFajr = 0;
    time_t tempSunrise = 0;
    time_t tempDhuhr = 0;
    time_t tempAsr = 0;
    time_t tempMaghrib = 0;
    time_t tempIsha = 0;

    struct tm *tm_date = localtime(&date);
    const int year = tm_date->tm_year + 1900;
    const int dayOfYear = tm_date->tm_yday + 1;

    solar_time_t solar_time = new_solar_time(date, coordinates);

    time_components_t time_components;
    time_components = from_double(solar_time.transit);
    time_t transit = is_valid_time(time_components) ? get_date_components(date, &time_components) : 0;

    time_components = from_double(solar_time.sunrise);
    time_t sunriseComponents = is_valid_time(time_components) ? get_date_components(date, &time_components) : 0;

    time_components = from_double(solar_time.sunset);
    time_t sunsetComponents = is_valid_time(time_components) ? get_date_components(date, &time_components) : 0;

    bool error = (transit == 0 || sunriseComponents == 0 || sunsetComponents == 0);

    if (!error) {
        tempDhuhr = transit;
        tempSunrise = sunriseComponents;
        tempMaghrib = sunsetComponents;

        time_components = from_double(afternoon(&solar_time, getShadowLength(parameters->madhab)));
        if (is_valid_time(time_components)) {
            tempAsr = get_date_components(date, &time_components);
        }

        // get night length
        time_t tomorrowSunrise = add_yday(sunriseComponents, 1);
        long night = tomorrowSunrise * 1000 - sunsetComponents * 1000 ;

        time_components = from_double(
                hourAngle(&solar_time, -parameters->fajrAngle, false));
        if (is_valid_time(time_components)) {
            tempFajr = get_date_components(date, &time_components);
        }

        if (parameters->method == MOON_SIGHTING_COMMITTEE && coordinates->latitude >= 55) {
            time_t tmp_time = add_seconds(sunriseComponents, -1 * (int) (night / 7000));
            tempFajr = tmp_time;
        }

        const night_portions_t nightPortions = get_night_portions(parameters);

        time_t safeFajr;
        if (parameters->method == MOON_SIGHTING_COMMITTEE) {
            safeFajr = seasonAdjustedMorningTwilight(coordinates->latitude, dayOfYear, year, sunriseComponents);
        } else {
            double portion = nightPortions.fajr;
            long nightFraction = (long) (portion * night / 1000);
            safeFajr = add_seconds(sunriseComponents, -1 * (int) nightFraction);
        }

        if (!tempFajr || difftime(safeFajr, tempFajr) > 0) {
            tempFajr = safeFajr;
        }

        // Isha calculation with check against safe value
        if (parameters->ishaInterval > 0) {
            time_t tmp_time = add_seconds(tempMaghrib, parameters->ishaInterval * 60);
            tempIsha = tmp_time;
        } else {
            time_components = from_double(hourAngle(&solar_time, -parameters->ishaAngle, true));
            if (is_valid_time(time_components)) {
                tempIsha = get_date_components(date, &time_components);
            }

            if (parameters->method == MOON_SIGHTING_COMMITTEE && coordinates->latitude >= 55) {
                long nightFraction = night / 7000;
                time_t tmp_time = add_seconds(sunsetComponents, (int) nightFraction);
                tempIsha = tmp_time;
            }

            time_t safeIsha;
            if (parameters->method == MOON_SIGHTING_COMMITTEE) {
                safeIsha = seasonAdjustedEveningTwilight(coordinates->latitude, dayOfYear, year, sunsetComponents);
            } else {
                double portion = nightPortions.isha;
                long nightFraction = (long) (portion * night / 1000);
                safeIsha = add_seconds(sunsetComponents, (int) nightFraction);
            }

            if (!tempIsha || difftime(safeIsha, tempIsha) < 0) {
                tempIsha = safeIsha;
            }
        }
    }

    // method based offsets
    int dhuhrOffsetInMinutes;
    if (parameters->method == MOON_SIGHTING_COMMITTEE) {
        // Moonsighting Committee requires 5 minutes for the sun to pass
        // the zenith and dhuhr to enter
        dhuhrOffsetInMinutes = 5;
    } else if (parameters->method == UMM_AL_QURA ||
               parameters->method == GULF ||
               parameters->method == QATAR) {
        // UmmAlQura and derivatives don't add
        // anything to zenith for dhuhr
        dhuhrOffsetInMinutes = 0;
    } else {
        dhuhrOffsetInMinutes = 1;
    }

    int maghribOffsetInMinutes;
    if (parameters->method == MOON_SIGHTING_COMMITTEE) {
        // Moonsighting Committee adds 3 minutes to sunset time to account for light refraction
        maghribOffsetInMinutes = 3;
    } else {
        maghribOffsetInMinutes = 0;

    }

    if (error || !tempAsr) {
        // if we don't have all prayer times then initialization failed
        return (prayer_times_t) NULL_PRAYER_TIMES;
    } else {

        time_t final_fajr_time = round_minute(add_minutes(tempFajr, parameters->adjustments.fajr));
        time_t final_sunrise_time = round_minute(add_minutes(tempSunrise, parameters->adjustments.sunrise));
        time_t final_dhuhr_time = round_minute(
                add_minutes(tempDhuhr, parameters->adjustments.dhuhr + dhuhrOffsetInMinutes));
        time_t final_asr_time = round_minute(add_minutes(tempAsr, parameters->adjustments.asr));
        time_t final_maghrib_time = round_minute(
                add_minutes(tempMaghrib, parameters->adjustments.maghrib + maghribOffsetInMinutes));
        time_t final_isha_time = round_minute(add_minutes(tempIsha, parameters->adjustments.isha));

        prayer_times_t prayer_times = {final_fajr_time, final_sunrise_time, final_dhuhr_time, final_asr_time,
                                           final_maghrib_time, final_isha_time};
        return prayer_times;
    }
}

prayer_t currentPrayer(prayer_times_t *prayer_times) {
    time_t now = time(0);
    return currentPrayer2(prayer_times, now);
}

prayer_t currentPrayer2(prayer_times_t *prayer_times, time_t when) {
    if (prayer_times->isha - when <= 0) {
        return ISHA;
    } else if (prayer_times->maghrib - when <= 0) {
        return MAGHRIB;
    } else if (prayer_times->asr - when <= 0) {
        return ASR;
    } else if (prayer_times->dhuhr - when <= 0) {
        return DHUHR;
    } else if (prayer_times->sunrise - when <= 0) {
        return SUNRISE;
    } else if (prayer_times->fajr - when <= 0) {
        return FAJR;
    } else {
        return NONE;
    }
}

prayer_t next_prayer(prayer_times_t *prayer_times) {
    time_t now = time(0);
    return next_prayer2(prayer_times, now);
}

prayer_t next_prayer2(prayer_times_t *prayer_times, time_t when) {
    if (prayer_times->isha - when <= 0) {
        return NONE;
    } else if (prayer_times->maghrib - when <= 0) {
        return ISHA;
    } else if (prayer_times->asr - when <= 0) {
        return MAGHRIB;
    } else if (prayer_times->dhuhr - when <= 0) {
        return ASR;
    } else if (prayer_times->sunrise - when <= 0) {
        return DHUHR;
    } else if (prayer_times->fajr - when <= 0) {
        return SUNRISE;
    } else {
        return FAJR;
    }
}

time_t timeForPrayer(prayer_times_t *prayer_times, prayer_t prayer) {
    switch (prayer) {
        case FAJR:
            return prayer_times->fajr;
        case SUNRISE:
            return prayer_times->sunrise;
        case DHUHR:
            return prayer_times->dhuhr;
        case ASR:
            return prayer_times->asr;
        case MAGHRIB:
            return prayer_times->maghrib;
        case ISHA:
            return prayer_times->isha;
        case NONE:
            return 0;
        default:
            return 0;
    }
}

time_t seasonAdjustedMorningTwilight(double latitude, int day, int year, time_t sunrise) {
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

time_t seasonAdjustedEveningTwilight(double latitude, int day, int year, time_t sunset) {
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