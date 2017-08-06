/*
 * Created by stormcaster on 22/06/17.
 */

#include "include/calculation_parameters.h"

calculation_parameters_t *new_calculation_parameters(double fajrAngle, double ishaAngle) {
    calculation_parameters_t *calculation_parameters = (calculation_parameters_t *) malloc(
            sizeof(calculation_parameters_t));
    calculation_method  method = OTHER;
    madhab_t madhab = SHAFI;
    high_latitude_rule_t highLatitudeRule = TWILIGHT_ANGLE;
    *calculation_parameters = (calculation_parameters_t) {&method, fajrAngle, ishaAngle, 0, madhab, highLatitudeRule,
                                                          INIT_PRAYER_ADJUSTMENTS()};
    return calculation_parameters;
}

inline calculation_parameters_t *new_calculation_parameters2(double fajrAngle, int ishaInterval) {
    calculation_parameters_t *calculation_parameters = (calculation_parameters_t *) malloc(
            sizeof(calculation_parameters_t));
    calculation_method  method = OTHER;
    madhab_t madhab = SHAFI;
    high_latitude_rule_t highLatitudeRule = TWILIGHT_ANGLE;
    *calculation_parameters = (calculation_parameters_t) {&method, fajrAngle, 0, ishaInterval, madhab, highLatitudeRule,
                                                          INIT_PRAYER_ADJUSTMENTS()};
    return calculation_parameters;
}

inline calculation_parameters_t *
new_calculation_parameters3(double fajrAngle, double ishaAngle, calculation_method *method) {
    calculation_parameters_t *calculation_parameters = (calculation_parameters_t *) malloc(
            sizeof(calculation_parameters_t));
    madhab_t madhab = SHAFI;
    high_latitude_rule_t highLatitudeRule = TWILIGHT_ANGLE;
    *calculation_parameters = (calculation_parameters_t) {method, fajrAngle, ishaAngle, 0, madhab, highLatitudeRule,
                                                          INIT_PRAYER_ADJUSTMENTS()};
    return calculation_parameters;
}

inline calculation_parameters_t *
new_calculation_parameters4(double fajrAngle, int ishaInterval, calculation_method *method) {
    calculation_parameters_t *calculation_parameters = (calculation_parameters_t *) malloc(
            sizeof(calculation_parameters_t));
    madhab_t madhab = SHAFI;
    high_latitude_rule_t highLatitudeRule = TWILIGHT_ANGLE;
    *calculation_parameters = (calculation_parameters_t) {method, fajrAngle, 0, ishaInterval, madhab, highLatitudeRule,
                                                          INIT_PRAYER_ADJUSTMENTS()};
    return calculation_parameters;
}

night_portions_t* new_night_portions(double fajr, double isha){
    night_portions_t* night_portions = (night_portions_t*) malloc(sizeof(night_portions));
    *(double *)&night_portions->fajr= fajr;
    *(double *)&night_portions->isha= isha;
    return night_portions;
}

night_portions_t *get_night_portions(calculation_parameters_t *calculation_parameters) {
    switch(calculation_parameters->highLatitudeRule){
        case MIDDLE_OF_THE_NIGHT:
            return new_night_portions(1.0 / 2.0, 1.0 / 2.0);
        case SEVENTH_OF_THE_NIGHT:
            return new_night_portions(1.0 / 7.0, 1.0 / 7.0);
        case TWILIGHT_ANGLE:
            return new_night_portions(calculation_parameters->fajrAngle / 60.0, calculation_parameters->ishaAngle / 60.0);
        default:
            return new_night_portions(1.0 / 2.0, 1.0 / 2.0);
    }
}

calculation_parameters_t* getParameters(calculation_method*calculation_method1){
    calculation_method default_method = OTHER;
    switch(*calculation_method1){
        case MUSLIM_WORLD_LEAGUE:
            return new_calculation_parameters3(18.0, 17.0, calculation_method1);
        case EGYPTIAN:
            return new_calculation_parameters3(20.0, 18.0, calculation_method1);
        case KARACHI:
            return new_calculation_parameters3(18.0, 18.0, calculation_method1);
        case UMM_AL_QURA:
            return new_calculation_parameters4(18.5, 90, calculation_method1);
        case GULF:
            return new_calculation_parameters4(19.5, 90, calculation_method1);
        case MOON_SIGHTING_COMMITTEE:
            return new_calculation_parameters3(18.0, 18.0, calculation_method1);
        case NORTH_AMERICA:
            return new_calculation_parameters3(15.0, 15.0, calculation_method1);
        case KUWAIT:
            return new_calculation_parameters3(18.0, 17.5, calculation_method1);
        case QATAR:
            return new_calculation_parameters4(18.0, 90, calculation_method1);
        case OTHER:
            return new_calculation_parameters3(0, 0, calculation_method1);
        default:
            return new_calculation_parameters3(0, 0, &default_method);
    }
}