cmake_minimum_required(VERSION 3.5)
project(.)
message("Installation target directory is: " ${CMAKE_INSTALL_PREFIX})
message("To override this value, use 'make -DCMAKE_INSTALL_PREFIX=$DIR'")

set(COMPILE_FLAGS "-ansi -pedantic -Wall -O2")
set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${COMPILE_FLAGS}" "${CMAKE_ENABLE_C99}")

set(HEADER_FILES
        src/data_components.h src/internal/shadow.h src/internal/double_utils.h src/internal/solar_coordinates.h src/internal/solar_time.h src/internal/coordinates.h src/internal/calendrical_helper.h src/internal/astronomical.h src/calculation_method.h src/calculation_parameters.h src/madhab.h src/high_latitude_rule.h src/prayer.h src/prayer_adjustments.h src/prayer_times.h)

set(SOURCE_FILES
        src/data_components.c src/internal/solar_coordinates.c src/internal/solar_time.c src/internal/astronomical.c src/internal/calendrical_helper.c src/calculation_parameters.c src/prayer_times.c src/samples/example.c)

add_library(adhan STATIC ${HEADER_FILES} ${SOURCE_FILES} )

install( FILES ${HEADER_FILES} DESTINATION "include/" )
install( TARGETS adhan DESTINATION "lib/" )

add_executable( example src/samples/example.c)
#target_link_libraries( sample adhan -lm )