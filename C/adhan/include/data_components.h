/*
 * Created by stormcaster on 22/06/17.
 */

#include <stdbool.h>
#include <time.h>

#ifndef C_DATA_COMPONENTS_H
#define C_DATA_COMPONENTS_H

typedef struct {
    int hours;
    int minutes;
    int seconds;
} time_components;

typedef struct {
    int day;
    int month;
    int year;
} date_components;

// TimeComponents
time_components *from_double(double value);

inline time_components *new_time_components(int *hours, int *minutes, int *seconds);

struct tm *get_tm_date(const time_components *time_components, const date_components *date_components);

// DateComponents
inline date_components *new_date_components(int *day, int *month, int *year);

date_components *from_tm(const struct tm *date);

// CalendarUtil
bool is_leap_year(int year);

struct tm *round_minute(struct tm *date);

struct tm *resolve_time(date_components *date_components);

struct tm *add_seconds(struct tm *when, int amount);

struct tm *add_minutes(struct tm *when, int amount);

struct tm *add_hours(struct tm *when, int amount);

struct tm *add_yday(struct tm *when, int amount);

struct tm *resolve_time_2(int year, int month, int day);

#endif //C_DATA_COMPONENTS_H

struct tm *get_date_components(struct tm *date);