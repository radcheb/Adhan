//
// Created by stormcaster on 22/06/17.
//

#ifndef C_CALCULATION_PARAMETERS_H
#define C_CALCULATION_PARAMETERS_H

#include "calculation_method.h"
#include "madhab.h"
#include "high_latitude_rule.h"
#include "prayer_adjustments.h"

typedef struct {
    calculation_method *method;
    double fajrAngle;
    double ishaAngle;
    int ishaInterval;
    madhab_t *madhab;
    high_latitude_rule_t *highLatitudeRule;
    prayer_adjustments_t *adjustments;
} calculation_parameters_t;

typedef struct {
    const double fajr;
    const double isha;
}night_portions_t;

inline calculation_parameters_t *_INIT_CALCULATION_PARAMETERS(double fajrAngle, double ishaAngle, int ishaInterval) {
    return (calculation_parameters_t *) (OTHER, fajrAngle, ishaAngle, ishaInterval, SHAFI, MIDDLE_OF_THE_NIGHT, INIT_PRAYER_ADJUSTMENTS());
}

inline calculation_parameters_t *INIT_CALCULATION_PARAMETERS() {
    return (calculation_parameters_t *) (OTHER, 0, 0, 0, SHAFI, MIDDLE_OF_THE_NIGHT, INIT_PRAYER_ADJUSTMENTS());
}

inline calculation_parameters_t *new_calculation_parameters(double fajrAngle, double ishaAngle);

inline calculation_parameters_t *new_calculation_parameters2(double fajrAngle, int ishaInterval);

inline calculation_parameters_t *
new_calculation_parameters3(double fajrAngle, double ishaAngle, calculation_method *method);

inline calculation_parameters_t *
new_calculation_parameters4(double fajrAngle, int ishaInterval, calculation_method* method);

night_portions_t* new_night_portions(double fajr, double isha);

night_portions_t* get_night_portions(calculation_parameters_t* calculation_parameters);

calculation_parameters_t* getParameters(calculation_method*calculation_method1);

#endif //C_CALCULATION_PARAMETERS_H