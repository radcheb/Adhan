#include "../internal/coordinates.h"
#include "../data_components.h"
#include "../calculation_parameters.h"
#include "../prayer_times.h"

int main (int argc, char *argv[]){
    const coordinates_t * coordinates = new_coordinates(35.78056, -78.6389);
    time_t now = time(0);
    const date_components* dateComponents = from_tm( gmtime(&now));
    calculation_method method = MUSLIM_WORLD_LEAGUE;
    const calculation_parameters_t * calculation_parameters = getParameters(&method);

    char buffer[80];
    prayer_times_t * prayer_times = new_prayer_times(coordinates, dateComponents, calculation_parameters);

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
