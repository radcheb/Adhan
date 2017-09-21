CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

project(Adhan-Test)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(REPO ${PROJECT_SOURCE_DIR}/../.repo)

enable_language(CXX)
include(CTest)
enable_testing()

#set(gtest_disable_pthreads on) #needed in MinGW
include(${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

download_project(
        PROJ                googlemock
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(test_SRCS
        Test.cpp
        astronomical_test.cpp
        double_utils_test.cpp)

add_executable(runUnitTests ${test_SRCS})
add_dependencies(runUnitTests adhan)
target_link_libraries(runUnitTests
        gtest
        gmock
        adhan
        m)
