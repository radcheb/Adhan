#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <math.h>

extern "C"
{
    #include "../src/include/data_components.h"
    #include "../src/include/double_utils.h"
    #include "../src/include/astronomical.h"
    #include "../src/include/solar_time.h"
}

MATCHER_P2(equalsTime, expectedHours, expectedMinutes,
           std::string(negation ? "isn't" : "equals") + " same time as " + ::testing::PrintToString(expectedHours) + ":" +
           ::testing::PrintToString(expectedMinutes)) {
    if(expectedMinutes != expectedMinutes || expectedHours != expectedHours){
        return ( arg != arg);
    } else {
        int actual_hours = (int) floor(arg);
        int exact_minutes = (int) floor((arg - actual_hours) * 60.0);
        int actual_seconds = (int) floor((arg - (actual_hours + exact_minutes / 60.0)) * 60.0 * 60.0);
        int rounded_minutes = (int) (exact_minutes + round(actual_seconds / 60.0));
        return expectedHours == actual_hours && expectedMinutes == rounded_minutes;
    }
}

TEST(AstronomicalTest, testAngleConversion) {
    EXPECT_NEAR(to_degrees(M_PI), 180.0, 0.00001);
    EXPECT_NEAR(to_degrees(M_PI / 2), 90.0, 0.00001);

    EXPECT_NEAR(to_radius(180.0), M_PI, 0.00001);
    EXPECT_NEAR(to_radius(90.0), M_PI / 2, 0.00001);
}

TEST(AstronomicalTest, testSolarCoordinates) {
    // values from Astronomical Algorithms page 165
    double jd = julianDay(/* year */ 1992, /* month */ 10, /* day */ 13);
    solar_coordinates_t *solar = new_solar_coordinates(/* julianDay */ jd);

    double T = julianCentury(/* julianDay */ jd);
    double L0 = meanSolarLongitude(/* julianCentury */ T);
    double epsilon0 = meanObliquityOfTheEcliptic(/* julianCentury */ T);
    double epsilonApp = apparentObliquityOfTheEcliptic(
            /* julianCentury */ T, /* meanObliquityOfTheEcliptic */ epsilon0);
    double M = meanSolarAnomaly(/* julianCentury */ T);
    double C = solarEquationOfTheCenter(
            /* julianCentury */ T, /* meanAnomaly */ M);
    double lambda = apparentSolarLongitude(
            /* julianCentury */ T, /* meanLongitude */ L0);
    double sigma = solar->declination;
    double alpha = unwind_angle(solar->rightAscension);

    EXPECT_NEAR(T, -0.072183436, 0.00000000001);

    EXPECT_NEAR(L0, 201.80720, 0.00001);

    EXPECT_NEAR(epsilon0, 23.44023, 0.00001);

    EXPECT_NEAR(epsilonApp, 23.43999, 0.00001);

    EXPECT_NEAR(M, 278.99397, 0.00001);

    EXPECT_NEAR(C, -1.89732, 0.00001);

    // lower accuracy than desired
    EXPECT_NEAR(lambda, 199.90895, 0.00002);

    EXPECT_NEAR(sigma, -7.78507, 0.00001);

    EXPECT_NEAR(alpha, 198.38083, 0.00001);

    // values from Astronomical Algorithms page 88

    jd = julianDay(/* year */ 1987, /* month */ 4, /* day */ 10);
    solar = new_solar_coordinates(/* julianDay */ jd);
    T = julianCentury(/* julianDay */ jd);

    double theta0 = meanSiderealTime(/* julianCentury */ T);
    double thetaApp = solar->apparentSiderealTime;
    double Omega = ascendingLunarNodeLongitude(/* julianCentury */ T);
    epsilon0 = meanObliquityOfTheEcliptic(/* julianCentury */ T);
    L0 = meanSolarLongitude(/* julianCentury */ T);
    double Lp = meanLunarLongitude(/* julianCentury */ T);
    double deltaPsi = nutationInLongitude(/* julianCentury */ T,
            /* solarLongitude */ L0, /* lunarLongitude */ Lp, /* ascendingNode */ Omega);
    double deltaEpsilon = nutationInObliquity(/* julianCentury */ T,
            /* solarLongitude */ L0, /* lunarLongitude */ Lp, /* ascendingNode */ Omega);
    double epsilon = epsilon0 + deltaEpsilon;

    EXPECT_NEAR(theta0, 197.693195, 0.000001);

    EXPECT_NEAR(thetaApp, 197.6922295833, 0.0001);

    // values from Astronomical Algorithms page 148

    EXPECT_NEAR(Omega, 11.2531, 0.0001);

    EXPECT_NEAR(deltaPsi, -0.0010522, 0.0001);

    EXPECT_NEAR(deltaEpsilon, 0.0026230556, 0.00001);

    EXPECT_NEAR(epsilon0, 23.4409463889, 0.000001);

    EXPECT_NEAR(epsilon, 23.4435694444, 0.00001);
}

TEST(AstronomicalTest, testRightAscensionEdgeCase) {
    solar_time_t *previousTime = NULL;
    coordinates_t coordinates = {35 + 47.0 / 60.0, -78 - 39.0 / 60.0};
    for (int i = 0; i < 365; i++) {
        time_t date = add_yday(resolve_time_2(2016, 1, 1), i);
        solar_time_t *time = new_solar_time(date, &coordinates);
        if (i > 0) {
            // transit from one day to another should not differ more than one minute
            EXPECT_LT(abs(time->transit - previousTime->transit), (1.0 / 60.0));

            // sunrise and sunset from one day to another should not differ more than two minutes
            EXPECT_LT(abs(time->sunrise - previousTime->sunrise), (2.0 / 60.0));
            EXPECT_LT(abs(time->sunset - previousTime->sunset), (2.0 / 60.0));
        }
        previousTime = time;
    }

}

TEST(AstronomicalTest, testAltitudeOfCelestialBody) {
    double Phi = 38 + (55 / 60.0) + (17.0 / 3600);
    double sigma = -6 - (43 / 60.0) - (11.61 / 3600);
    double H = 64.352133;
    double h = altitudeOfCelestialBody(
            /* observerLatitude */ Phi, /* declination */ sigma, /* localHourAngle */ H);
    ASSERT_NEAR(h, 15.1249, 0.0001);
}

TEST(AstronomicalTest, testTransitAndHourAngle) {
    // values from Astronomical Algorithms page 103
    double longitude = -71.0833;
    double theta = 177.74208;
    double alpha1 = 40.68021;
    double alpha2 = 41.73129;
    double alpha3 = 42.78204;
    double m0 = getApproximateTransit(longitude,
            /* siderealTime */ theta, /* rightAscension */ alpha2);

    EXPECT_NEAR(m0, 0.81965, 0.00001);

    double transit = correctedTransit(
            /* approximateTransit */ m0, longitude, /* siderealTime */ theta,
            /* rightAscension */ alpha2, /* previousRightAscension */ alpha1,
            /* nextRightAscension */ alpha3) / 24;

    EXPECT_NEAR(transit, 0.81980, 0.00001);

    double sigma1 = 18.04761;
    double sigma2 = 18.44092;
    double sigma3 = 18.82742;

    coordinates_t coordinates =  {/* latitude */ 42.3333, longitude};
    double rise = correctedHourAngle(/* approximateTransit */ m0,
            /* angle */ -0.5667, &coordinates,
            /* afterTransit */ false, /* siderealTime */ theta,
            /* rightAscension */ alpha2, /* previousRightAscension */ alpha1,
            /* nextRightAscension */ alpha3, /* declination */ sigma2,
            /* previousDeclination */ sigma1, /* nextDeclination */ sigma3) / 24;
    EXPECT_NEAR(rise, 0.51766, 0.00001);
}

TEST(AstronomicalTest, testSolarTime) {
    /*
     * Comparison values generated from
     * http://aa.usno.navy.mil/rstt/onedaytable?form=1&ID=AA&year=2015&month=7&day=12&state=NC&place=raleigh
     */

    coordinates_t coordinates = {35 + 47.0/60.0, -78 - 39.0/60.0};
    solar_time_t *solar = new_solar_time(resolve_time_2(2015, 7, 12), &coordinates);

    double transit = solar->transit;
    double sunrise = solar->sunrise;
    double sunset = solar->sunset;
    double twilightStart = hourAngle(solar ,-6, /* afterTransit */ false);
    double twilightEnd = hourAngle(solar ,-6, /* afterTransit */ true);
    double invalid = hourAngle(solar ,-36, /* afterTransit */ true);
    EXPECT_THAT(twilightStart, equalsTime(9, 38));
    EXPECT_THAT(sunrise, equalsTime(10, 8));
    EXPECT_THAT(transit, equalsTime(17, 20));
    EXPECT_THAT(sunset, equalsTime(24, 32));
    EXPECT_THAT(twilightEnd, equalsTime(25, 2));
    EXPECT_TRUE(std::isnan(invalid));
}

TEST(AstronomicalTest, testCalendricalDate) {
    // generated from http://aa.usno.navy.mil/data/docs/RS_OneYear.php for KUKUIHAELE, HAWAII
    coordinates_t coordinates = {/* latitude */ 20 + 7.0/60.0, /* longitude */ -155.0 - 34.0/60.0};
    solar_time_t* day1solar = new_solar_time(resolve_time_2(2015, 4, /* day */ 2), &coordinates);
    solar_time_t* day2solar = new_solar_time(resolve_time_2(2015, 4, 3), &coordinates);

    double day1 = day1solar->sunrise;
    double day2 = day2solar->sunrise;

    EXPECT_THAT(day1, equalsTime(16, 15));
    EXPECT_THAT(day2, equalsTime(16, 14));
}

TEST(AstronomicalTest, testInterpolation) {
    // values from Astronomical Algorithms page 25
    double interpolatedValue = interpolate(/* value */ 0.877366,
            /* previousValue */ 0.884226, /* nextValue */ 0.870531, /* factor */ 4.35/24);
    ASSERT_NEAR(interpolatedValue, 0.876125, 0.000001);

    double i1 = interpolate(
            /* value */ 1, /* previousValue */ -1, /* nextValue */ 3, /* factor */ 0.6);
    ASSERT_NEAR(i1, 2.2, 0.000001);
}

TEST(AstronomicalTest, testAngleInterpolation) {
    double i1 = interpolateAngles(/* value */ 1, /* previousValue */ -1,
            /* nextValue */ 3, /* factor */ 0.6);
    ASSERT_NEAR(i1, 2.2, 0.000001);

    double i2 = interpolateAngles(/* value */ 1, /* previousValue */ 359,
            /* nextValue */ 3, /* factor */ 0.6);
    ASSERT_NEAR(i2, 2.2, 0.000001);
}

TEST(AstronomicalTest, testJulianDay) {
    /*
     * Comparison values generated from http://aa.usno.navy.mil/data/docs/JulianDate.php
     */

    ASSERT_NEAR(julianDay(/* year */ 2010, /* month */ 1, /* day */ 2), 2455198.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2011, /* month */ 2, /* day */ 4), 2455596.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2012, /* month */ 3, /* day */ 6), 2455992.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2013, /* month */ 4, /* day */ 8), 2456390.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2014, /* month */ 5, /* day */ 10), 2456787.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2015, /* month */ 6, /* day */ 12), 2457185.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2016, /* month */ 7, /* day */ 14), 2457583.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2017, /* month */ 8, /* day */ 16), 2457981.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2018, /* month */ 9, /* day */ 18), 2458379.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2019, /* month */ 10, /* day */ 20), 2458776.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2020, /* month */ 11, /* day */ 22), 2459175.500000, 0.00001);
    ASSERT_NEAR(julianDay(/* year */ 2021, /* month */ 12, /* day */ 24), 2459572.500000, 0.00001);

    double jdVal = 2457215.67708333;
    ASSERT_NEAR(_julianDay(/* year */ 2015, /* month */ 7, /* day */ 12, /* hours */ 4.25),jdVal, 0.000001);

    date_components_t dateComponent = new_date_components(/* day */ 12, /* month */ 7, /* year */ 2015);
    time_components_t time_component = new_time_components(/* hour */ 4, /* minute */ 15, 0);
    time_t when = get_tm_date(&time_component, &dateComponent);
    printf("when : %lu\n", when);
    ASSERT_NEAR(julianDay2(gmtime(&when)),jdVal, 0.000001);

    ASSERT_NEAR(_julianDay(/* year */ 2015, /* month */ 7, /* day */ 12, /* hours */ 8.0),2457215.833333, 0.000001);
    ASSERT_NEAR(_julianDay(/* year */ 1992, /* month */ 10, /* day */ 13, /* hours */ 0.0), 2448908.5, 0.000001);
}

TEST(AstronomicalTest, testJulianHours) {
    double j1 = julianDay(/* year */ 2010, /* month */ 1, /* day */ 3);
    double j2 = _julianDay(/* year */ 2010,
            /* month */ 1, /* day */ 1, /* hours */ 48);
    ASSERT_NEAR(j1, j2, 0.0000001);
}

TEST(AstronomicalTest, testLeapYear) {
    ASSERT_EQ(is_leap_year(2015), false);
    ASSERT_EQ(is_leap_year(2016), true);
    ASSERT_EQ(is_leap_year(1600), true);
    ASSERT_EQ(is_leap_year(2000), true);
    ASSERT_EQ(is_leap_year(2400), true);
    ASSERT_EQ(is_leap_year(1700), false);
    ASSERT_EQ(is_leap_year(1800), false);
    ASSERT_EQ(is_leap_year(1900), false);
    ASSERT_EQ(is_leap_year(2100), false);
    ASSERT_EQ(is_leap_year(2200), false);
    ASSERT_EQ(is_leap_year(2300), false);
    ASSERT_EQ(is_leap_year(2500), false);
    ASSERT_EQ(is_leap_year(2600), false);
}