cmake_minimum_required(VERSION 3.16)
project(img_sound_proc)

set(CMAKE_CXX_STANDARD 17)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)

# modify if using different versions or paths
include_directories(./eigen-3.4.0)
include_directories(./AudioFile)
include_directories(./opencv/build/include/opencv2)
set("OpenCV_DIR" "./opencv/build/")

find_package(OpenCV REQUIRED core imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(img_sound_proc main.cpp utils.cpp transforms.cpp parsing.cpp)
target_link_libraries(img_sound_proc ${OpenCV_LIBS})

# todo: separate into a different cmake file?
#enable_testing()
#add_executable(my_test test.cpp utils.cpp transforms.cpp parsing.cpp)
#find_package(GTest REQUIRED)
#target_link_libraries(my_test GTest::gtest_main)
#target_link_libraries(my_test ${OpenCV_LIBS})
#include(GoogleTest)
#gtest_discover_tests(my_test)
