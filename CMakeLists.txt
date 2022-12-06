cmake_minimum_required(VERSION 3.23)
project(img_sound_proc)

set(CMAKE_CXX_STANDARD 17)

include_directories(../eigen-3.4.0)
include_directories(../CImg-3.2.0_pre110222)

#include_directories(../opencv)
#include_directories(../opencv/build/include/opencv2)

#set(OpenCV_DIR "../opencv/build/x64/vc15/lib")
#find_package(OpenCV REQUIRED core imgcodecs)
#include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(img_sound_proc main.cpp)

#set(OpenCV_LIBS opencv_core opencv_imgcodecs)
#target_link_libraries(img_sound_proc ${OpenCV_LIBS})