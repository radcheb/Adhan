#include "gtest/gtest.h"
#include "../src/include/astronomical.h"
#include "../src/include/prayer_times.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ComplexNumberTest, positives){
    int a = 3;
    int b = 3;
    EXPECT_EQ(b, a);
}