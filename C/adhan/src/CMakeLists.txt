
INCLUDE_DIRECTORIES(include)
SET(SOURCE_FILES
        astronomical.c
        solar_coordinates.c
        solar_time.c
        solar_coordinates.c
        calculation_parameters.c
        prayer_times.c
        data_components.c
        calendrical_helper.c
        )

ADD_LIBRARY(${CMAKE_PROJECT_NAME} STATIC ${SOURCE_FILES})
add_executable(example example.c)
target_link_libraries(example ${CMAKE_PROJECT_NAME} m)