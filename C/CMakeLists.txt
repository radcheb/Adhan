cmake_minimum_required(VERSION 3.5)
project(C)
message("Installation target directory is: " ${CMAKE_INSTALL_PREFIX})
message("To override this value, use 'make -DCMAKE_INSTALL_PREFIX=$DIR'")

set(COMPILE_FLAGS "-ansi -pedantic -Wall -O2")
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        CMakeLists.txt src/data_components.h src/data_components.c src/internal/shadow.h src/internal/double_utils.h src/internal/solar_coordinates.h src/internal/solar_coordinates.c src/internal/solar_time.h src/internal/coordinates.h src/internal/solar_time.c src/internal/calendrical_helper.h src/internal/astronomical.h src/internal/astronomical.c src/internal/calendrical_helper.c src/calculation_method.h src/calculation_parameters.h src/madhab.h src/high_latitude_rule.h src/prayer.h src/prayer_adjustments.h src/calculation_parameters.c src/prayer_times.h src/prayer_times.c)

add_executable(C ${SOURCE_FILES})