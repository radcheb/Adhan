#include <stdio.h>
#include "../include/coordinates.h"
#include "../include/data_components.h"
#include "../include/calculation_parameters.h"
#include "../include/prayer_times.h"

int main (int argc, char *argv[]){
  coordinates_t coordinates;
  new_coordinates(&coordinates, 35.78056, -78.6389);
//    const coordinates_t * coordinates = new_coordinates(35.78056, -78.6389);
  printf("Using coordinates: (%f,%f)\n", coordinates.latitude, coordinates.longitude);

    time_t now = time(0);
    date_components_t *dateComponents = (date_components_t *)malloc(sizeof(date_components_t));
    *dateComponents= from_tm( gmtime(&now));
    printf("Using date: %d/%d/%d\n", dateComponents->day, dateComponents->month, dateComponents->year);

    calculation_method method = MUSLIM_WORLD_LEAGUE;
    const calculation_parameters_t * calculation_parameters = getParameters(&method);
    printf("Using calculation method: %s\n", get_calculation_method_name(*(calculation_parameters->method)));
    printf("Using calculation angles: fajr:%d ishaa:%d - %f\n", calculation_parameters->fajrAngle, calculation_parameters->ishaInterval,
         calculation_parameters->ishaAngle);
    printf("Using calculation asr: %s\n", get_madhab_name(*(calculation_parameters->madhab)));
    printf("Using calculation high lat: %s\n", get_high_latitude_rule_name(*(calculation_parameters->highLatitudeRule)));

    char buffer[80];
    printf("Calculating prayer times...");
    prayer_times_t * prayer_times = new_prayer_times(&coordinates, dateComponents, calculation_parameters);

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
