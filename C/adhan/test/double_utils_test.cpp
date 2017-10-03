#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include <math.h>
extern "C"
{
    #include "../src/include/double_utils.h"
    #include "../src/include/astronomical.h"
}

TEST(DoubleUtilsTest, testNormalizing) {
    EXPECT_NEAR(normalize_with_bound(2.0, -5), -3, 0.00001);

    EXPECT_NEAR(normalize_with_bound(-4.0, -5.0), -4, 0.00001);
    EXPECT_NEAR(normalize_with_bound(-6.0, -5.0), -1, 0.00001);

    EXPECT_NEAR(normalize_with_bound(-1.0, 24), 23, 0.00001);
    EXPECT_NEAR(normalize_with_bound(1.0, 24.0), 1, 0.00001);
    EXPECT_NEAR(normalize_with_bound(49.0, 24), 1, 0.00001);

    EXPECT_NEAR(normalize_with_bound(361.0, 360), 1, 0.00001);
    EXPECT_NEAR(normalize_with_bound(360.0, 360), 0, 0.00001);
    EXPECT_NEAR(normalize_with_bound(259.0, 360), 259, 0.00001);
    EXPECT_NEAR(normalize_with_bound(2592.0, 360), 72, 0.00001);

    EXPECT_NEAR(unwind_angle(-45.0), 315, 0.00001);
    EXPECT_NEAR(unwind_angle(361.0), 1, 0.00001);
    EXPECT_NEAR(unwind_angle(360.0), 0, 0.00001);
    EXPECT_NEAR(unwind_angle(259.0), 259, 0.00001);
    EXPECT_NEAR(unwind_angle(2592.0), 72, 0.00001);

    EXPECT_NEAR(normalize_with_bound(360.1, 360), 0.1, 0.01);
}

TEST(DoubleUtilsTest, testClosestAngle) {
    EXPECT_NEAR(closest_angle(360.0), 0, 0.000001);
    EXPECT_NEAR(closest_angle(361.0), 1, 0.000001);
    EXPECT_NEAR(closest_angle(1.0), 1, 0.000001);
    EXPECT_NEAR(closest_angle(-1.0), -1, 0.000001);
    EXPECT_NEAR(closest_angle(-181.0), 179, 0.000001);
    EXPECT_NEAR(closest_angle(180.0), 180, 0.000001);
    EXPECT_NEAR(closest_angle(359.0), -1, 0.000001);
    EXPECT_NEAR(closest_angle(-359.0), 1, 0.000001);
    EXPECT_NEAR(closest_angle(1261.0), -179, 0.000001);
    EXPECT_NEAR(closest_angle(-360.1), -0.1, 0.01);
}