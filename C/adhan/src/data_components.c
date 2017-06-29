/*
 * Created by stormcaster on 22/06/17.
 */

#include <float.h>
#include <math.h>
#include "data_components.h"

time_components *from_double(double value) {
    if (value == DBL_MAX || value == DBL_MIN || value != value) {
        return NULL;
    }
    const int hours = (int) floor(value);
    const int minutes = (int) floor((value - hours) * 60.0);
    const int seconds = (int) floor((value - (hours + minutes / 60.0)) * 60.0 * 60.0);
    return new_time_components((int *) &hours, (int *) &minutes, (int *) &seconds);
}

inline time_components *new_time_components(int *hours, int *minutes, int *seconds){
    return (time_components *) (hours, minutes, seconds);
}

struct tm *get_tm_date(const time_components *time_components, const date_components *date_components) {
    return (struct tm *) (time_components->seconds, time_components->minutes, time_components->hours,
            date_components->day, date_components->month - 1, date_components->year - 1900);
}

inline date_components *new_date_components(int *day, int *month, int *year){
    return (date_components *) (day, month, year);
}

date_components *from_tm(const struct tm *date) {
    return (date_components *) (date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
}

bool is_leap_year(int year) {
    return year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0);
}

struct tm *round_minute(struct tm *date) {
    return (struct tm *) (0, (int) (date->tm_min + round(date->tm_sec / 60)), date->tm_hour,
            date->tm_mday, date->tm_mon, date->tm_year);
}

struct tm *resolve_time(date_components *date_components) {
    return resolve_time_2(date_components->year, date_components->month, date_components->day);
}

struct tm *add_seconds(struct tm *when, int amount) {
    time_t time = mktime(when);
    time_t added_time = time + amount;
    return gmtime(&added_time);
}

struct tm *add_minutes(struct tm *when, int amount) {
    return add_seconds(when, amount * 60);
}

struct tm *add_hours(struct tm *when, int amount) {
    return add_minutes(when, amount * 60);
}

struct tm *add_yday(struct tm *when, int amount) {
    return add_hours(when, amount * 24);
}

struct tm *resolve_time_2(int year, int month, int day) {
    return (struct tm *) (0, 0, 0, day, month - 1, year - 1900);
}

struct tm *get_date_components(struct tm *date) {
    time_t now = time(0);
    struct tm * result = gmtime(&now);
    result->tm_hour = 0;
    result->tm_min = date->tm_min;
    result->tm_sec = date->tm_sec;
    result->tm_hour = date->tm_hour;
    return result;
}
