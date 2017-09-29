/*
 * Created by stormcaster on 22/06/17.
 */

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "include/data_components.h"

inline time_components_t *from_double(double value) {
    if (value == DBL_MAX || value == DBL_MIN || value != value) {
        return NULL;
    }
    const int hours = (int) floor(value);
    const int minutes = (int) floor((value - hours) * 60.0);
    const int seconds = (int) floor((value - (hours + minutes / 60.0)) * 60.0 * 60.0);
    time_components_t * time_components = (time_components_t *)malloc(sizeof(time_components_t));
    time_components->hours = hours;
    time_components->minutes = minutes;
    time_components->seconds = seconds;
    return time_components;
}

extern time_components_t new_time_components(int hours, int minutes, int seconds){
    time_components_t time_components=  {hours, minutes, seconds};
    return time_components;
}

time_t get_tm_date(time_components_t *time_components, date_components_t *date_components) {

    struct tm tmp = {0};
    tmp.tm_mday =date_components->day;
    tmp.tm_mon = date_components->month - 1;
    tmp.tm_year = date_components->year - 1900;
    tmp.tm_hour =time_components->hours;
    tmp.tm_min =time_components->minutes;
    tmp.tm_sec =time_components->seconds;
    time_t time = mktime(&tmp);
    return time;
}

extern date_components_t new_date_components(int day, int month, int year){
    date_components_t date_components =  {day, month, year};
    return date_components;
}

inline date_components_t from_time_t(const time_t time) {
    struct tm * date = localtime(&time);
    date_components_t date_components = {date->tm_mday, date->tm_mon + 1, date->tm_year + 1900};
    return date_components;
}

bool is_leap_year(int year) {
    return year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0);
}

inline time_t round_minute(time_t time) {
    double seconds = time % 60;
    return (time_t) (time - seconds + round(seconds / 60) * 60 );
}

time_t add_seconds(const time_t when, int amount) {
    time_t result = when + amount;
    return result;
}

time_t add_minutes(const time_t when, int amount) {
    return add_seconds(when, amount * 60);
}

time_t add_hours(const time_t when, int amount) {
    return add_minutes(when, amount * 60);
}

time_t add_yday(const time_t when, int amount) {
    return add_hours(when, amount * 24);
}

time_t resolve_time(const date_components_t *date_components) {
    return resolve_time_2(date_components->year, date_components->month, date_components->day);
}

void setToLocalTz(struct tm* date){
    time_t now = time(0);
    struct tm* local_date = localtime(&now);
    date->tm_zone = local_date->tm_zone;
    date->tm_isdst = local_date->tm_isdst;
    date->tm_gmtoff = local_date->tm_gmtoff;
}

time_t resolve_time_2(int year, int month, int day) {
    struct tm tmp = {0};
    tmp.tm_year =year - 1900;
    tmp.tm_mon =month - 1;
    tmp.tm_mday =day;
    time_t tmp_time = mktime(&tmp);
    return tmp_time;
}

time_t get_date_components(const time_t time, const time_components_t* time_components){
    struct tm * tm_date = localtime(&time);
    tm_date->tm_hour = 0;
    tm_date->tm_sec = time_components->seconds;
    tm_date->tm_min = time_components->minutes;
    tm_date->tm_hour = time_components->hours;
    return mktime(tm_date);
}
