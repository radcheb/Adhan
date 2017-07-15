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
} time_components_t;

typedef struct {
    int day;
    int month;
    int year;
} date_components_t;

// TimeComponents
inline time_components_t *from_double(double value);

static inline time_components_t new_time_components(int hours, int minutes, int seconds);

static inline time_t get_time_t(const time_components_t *time_components, const date_components_t *date_components);

// DateComponents
static inline date_components_t new_date_components(int day, int month, int year);

inline date_components_t from_time_t(const time_t time);

// CalendarUtil
bool is_leap_year(int year);

inline time_t round_minute(time_t time);

time_t add_seconds(const time_t when, int amount);

time_t add_minutes(const time_t when, int amount);

time_t add_hours(const time_t when, int amount);

time_t add_yday(const time_t when, int amount);

time_t resolve_time(const date_components_t *date_components);

time_t resolve_time_2(int year, int month, int day);

time_t get_date_components(const time_t time, const time_components_t* time_components);

#endif //C_DATA_COMPONENTS_H
