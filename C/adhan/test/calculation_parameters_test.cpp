#include "gtest/gtest.h"

extern "C"
{
    #include "../src/include/calculation_parameters.h"
}

TEST(CalculationMethodTest, testNightPortion) {
    calculation_parameters_t parameters = new_calculation_parameters(18, 18);
    parameters.highLatitudeRule = MIDDLE_OF_THE_NIGHT;
    night_portions_t night_portions = get_night_portions(&parameters);
    ASSERT_NEAR(night_portions.fajr, 0.5, 0.001);
    ASSERT_NEAR(night_portions.isha, 0.5, 0.001);

    parameters.highLatitudeRule = SEVENTH_OF_THE_NIGHT;
    night_portions_t night_portions2 = get_night_portions(&parameters);
    ASSERT_NEAR(night_portions2.fajr, 1.0/7.0, 0.001);
    ASSERT_NEAR(night_portions2.isha, 1.0/7.0, 0.001);

    parameters = new_calculation_parameters(10.0, 15.0);
    parameters.highLatitudeRule = TWILIGHT_ANGLE;
    night_portions_t night_portions3 = get_night_portions(&parameters);
    ASSERT_NEAR(night_portions3.fajr, 10.0/60.0, 0.001);
    ASSERT_NEAR(night_portions3.isha, 15.0/60.0, 0.001);
}

