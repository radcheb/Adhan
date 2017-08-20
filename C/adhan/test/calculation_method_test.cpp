#include "gtest/gtest.h"
#include <math.h>

extern "C"
{
    #include "../src/include/calculation_method.h"
    #include "../src/include/calculation_parameters.h"
}

TEST(CalculationMethodTest, testCalculationMethods) {
    calculation_method method = MUSLIM_WORLD_LEAGUE;
    calculation_parameters_t * parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 17, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), MUSLIM_WORLD_LEAGUE);
    free(parameters);

    method = EGYPTIAN;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 20, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 18, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), EGYPTIAN);
    free(parameters);

    method = KARACHI;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 18, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), KARACHI);
    free(parameters);

    method = UMM_AL_QURA;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18.5, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 0, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 90);
    EXPECT_EQ(*(parameters->method), UMM_AL_QURA);
    free(parameters);

    method = GULF;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 19.5, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 0, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 90);
    EXPECT_EQ(*(parameters->method), GULF);
    free(parameters);

    method = MOON_SIGHTING_COMMITTEE;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 18, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), MOON_SIGHTING_COMMITTEE);
    free(parameters);

    method = NORTH_AMERICA;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 15, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 15, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), NORTH_AMERICA);
    free(parameters);

    method = KUWAIT;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 17.5, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), KUWAIT);
    free(parameters);

    method = QATAR;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 18, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 0, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 90);
    EXPECT_EQ(*(parameters->method), QATAR);
    free(parameters);

    method = OTHER;
    parameters = getParameters(&method);
    EXPECT_NEAR(parameters->fajrAngle, 0, 0.000001);
    EXPECT_NEAR(parameters->ishaAngle, 0, 0.000001);
    EXPECT_EQ(parameters->ishaInterval, 0);
    EXPECT_EQ(*(parameters->method), OTHER);
    free(parameters);
}
