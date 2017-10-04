#include <stdio.h>
#include <calculation_parameters.h>
#include <data_components.h>
#include "include/coordinates.h"
#include "include/calculation_parameters.h"
#include "include/prayer_times.h"

#define PARIS_COORDINATES {48.866667, 2.333333};

int main(int argc, char *argv[]) {

//  test_dates();
    coordinates_t coordinates = PARIS_COORDINATES;
    calculation_method method = OTHER;
    high_latitude_rule_t highLatitudeRule = MIDDLE_OF_THE_NIGHT;
    calculation_parameters_t calculation_parameters = new_calculation_parameters(15.0, 13.0);
//    const calculation_parameters_t *calculation_parameters = getParameters(&method);
    calculation_parameters.highLatitudeRule = highLatitudeRule;
    printf("Using calculation high lat: %s\n",
           get_high_latitude_rule_name(calculation_parameters.highLatitudeRule));





    printf("Using coordinates: (lat:%f, long:%f)\n", coordinates.latitude, coordinates.longitude);
    printf("Using calculation method: %s\n", get_calculation_method_name(calculation_parameters.method));
    printf("Using calculation angles: fajr:%d ishaa:%d - %f\n", (int) calculation_parameters.fajrAngle,
           calculation_parameters.ishaInterval,
           calculation_parameters.ishaAngle);
    printf("Using calculation asr: %s\n", get_madhab_name(calculation_parameters.madhab));
    printf("Using calculation high lat: %s\n",
           get_high_latitude_rule_name(calculation_parameters.highLatitudeRule));

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



    time_t now = resolve_time_2(2017, 10, 01);
    time_t start_time = add_yday(now, - 30 - 16);

    date_components_t dateComponents = from_time_t(start_time);
    printf("Starting from date: %d/%d/%d\n", dateComponents.day, dateComponents.month, dateComponents.year);

    printf(" Date \t\t Fajr \t\t Sunrise \t Dhuhr \t\t Asr \t\t Maghrib \t Ishaa\n");

    for(int i = 1; i < 31; i++){

        time_t ref_date = add_yday(start_time, i);
        dateComponents = from_time_t(ref_date);

        prayer_times_t prayer_times = new_prayer_times(&coordinates, &dateComponents, &calculation_parameters);

        strftime(buffer, 80, "%x", localtime(&ref_date));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.fajr));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.sunrise));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.dhuhr));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.asr));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.maghrib));
        printf(" %s\t", buffer);

        strftime(buffer, 80, "%I:%M%p", localtime(&prayer_times.isha));
        printf(" %s\n", buffer);
    }


}
