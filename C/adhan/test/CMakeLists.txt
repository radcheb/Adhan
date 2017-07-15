CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

project(Adhan-Test)

set(REPO /home/stormcaster/workspace/Adhan/C/adhan/.repo)


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

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(test_SRCS Test.cpp)
add_executable(runUnitTests ${test_SRCS})
target_link_libraries(runUnitTests gtest gmock ${CMAKE_PROJECT_NAME} m)