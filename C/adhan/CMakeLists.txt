PROJECT(adhan)
SET(CMAKE_PROJECT_NAME adhan)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# Mode debug par defaut
IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "DEBUG")
    ADD_DEFINITIONS(-DNOSYSLOG=1)
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")

IF("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    SET(FORMAT_CODE style)
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")

SET(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
SET(CMAKE_C_FLAGS_DEBUG " -DDEBUG ")
SET(CMAKE_CXX_FLAGS_DEBUG " -DDEBUG")
SET(CPACK_SOURCE_IGNORE_FILES "/build.*/;/deb/;/CVS/;/\\\\.svn/;/\\\\.git/;\\\\.swp$;/*~;tags;\\\\.pcs$;")

SET(MAJOR 1)
SET(MINOR 0)
SET(PATCH 1)
SET(VERSION ${MAJOR}.${MINOR}.${PATCH})

ADD_DEFINITIONS("-fgnu89-inline")

ADD_DEFINITIONS("-Wall -ggdb3  -Wfatal-errors")
ADD_DEFINITIONS("-fno-builtin-malloc -fno-builtin-realloc -fno-builtin-calloc -fno-builtin-free -fno-strict-aliasing -fno-omit-frame-pointer")
ADD_DEFINITIONS("-fstrict-aliasing")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(include)


SET(ADAHN_SRC
        src/astronomical.c
        src/solar_coordinates.c
        src/solar_time.c
        src/solar_coordinates.c
        src/calculation_parameters.c
        src/prayer_times.c
        src/data_components.c
        src/calendrical_helper.c
  )


# Executable for bidviser
#ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${ADAHN_SRC})
ADD_LIBRARY(${CMAKE_PROJECT_NAME} STATIC ${ADAHN_SRC})
  #TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}  m  )


set(TEST_ADHAN test/example.c)

add_executable( test-adhan ${TEST_ADHAN})
target_link_libraries(test-adhan
                    ${CMAKE_PROJECT_NAME}
                    m)
