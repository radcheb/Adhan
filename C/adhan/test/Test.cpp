#include "gtest/gtest.h"
#include "../src/include/astronomical.h"
#include "../src/include/prayer_times.h"

int main(int argc, char **argv) {
    putenv((char *) "TZ=UTC");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
