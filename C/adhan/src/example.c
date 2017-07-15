#include <stdio.h>
#include "include/coordinates.h"
#include "include/calculation_parameters.h"
#include "include/prayer_times.h"

void test_dates(){

    time_t now;
    struct tm *lcltime = (struct tm *)malloc(sizeof(struct tm));
    now = time(NULL);

    struct tm *tmp= gmtime(&now);
    *lcltime = *tmp;

    time_t yesterday_time = (now - 86400);

    struct tm *yesterday_date = (struct tm *)malloc(sizeof(struct tm));

    tmp = gmtime(&yesterday_time);
    *yesterday_date = *tmp;

    printf("The time is %d:%d\n", lcltime->tm_hour, lcltime->tm_min);

    printf("\ntoday: %s\n", asctime(lcltime));
    printf("\nyesterday: %s\n", asctime(yesterday_date));

}

int main (int argc, char *argv[]){

//  test_dates();
  coordinates_t coordinates;
  new_coordinates(&coordinates, 48.866667, 2.333333 );
  printf("Using coordinates: (lat:%f, long:%f)\n", coordinates.latitude, coordinates.longitude);

    time_t now = time(0);
    date_components_t *dateComponents = (date_components_t *)malloc(sizeof(date_components_t));
    *dateComponents= from_time_t(now);
    printf("Using date: %d/%d/%d\n", dateComponents->day, dateComponents->month, dateComponents->year);

    calculation_method method = MOON_SIGHTING_COMMITTEE;
    const calculation_parameters_t * calculation_parameters = getParameters(&method);
    printf("Using calculation method: %s\n", get_calculation_method_name(*(calculation_parameters->method)));
    printf("Using calculation angles: fajr:%d ishaa:%d - %f\n", (int) calculation_parameters->fajrAngle, calculation_parameters->ishaInterval,
           calculation_parameters->ishaAngle);
    printf("Using calculation asr: %s\n", get_madhab_name(*(calculation_parameters->madhab)));
    *(calculation_parameters->highLatitudeRule) = TWILIGHT_ANGLE;
    printf("Using calculation high lat: %s\n", get_high_latitude_rule_name(*(calculation_parameters->highLatitudeRule)));

    char buffer[80];
    printf("Calculating prayer times...\n");

//    printf("_julianDay(%d, %d, %d, %d) = %f\n", dateComponents->year, dateComponents->month, dateComponents->day,
//           0, _julianDay(dateComponents->year, dateComponents->month, dateComponents->day, 0.0));
//
//    struct tm *today = resolve_time(dateComponents);
//    struct tm *yesterday = add_yday(today, -2);
//    double result = julianDay2(yesterday);
//    double expected = 2457942.5;
//    printf("Julien day of %d/%d/%d is %d but expected %d\n", dateComponents->day, dateComponents->month,
//           dateComponents->year, result, expected);


    prayer_times_t * prayer_times = new_prayer_times(&coordinates, dateComponents,
                                                     (calculation_parameters_t *) calculation_parameters);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->fajr);
    printf("Fajr: %s\n", buffer);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->sunrise);
    printf("Sunrise: %s\n", buffer);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->dhuhr);
    printf("Dhuhr: %s\n", buffer);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->asr);
    printf("Asr: %s\n", buffer);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->maghrib);
    printf("Maghrib: %s\n", buffer);

    strftime(buffer,80,"%x - %I:%M%p", prayer_times->isha);
    printf("Isha: %s\n", buffer);
}
