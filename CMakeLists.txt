cmake_minimum_required(VERSION 3.16)
project(img_sound_proc)

set(CMAKE_CXX_STANDARD 17)

include_directories(../eigen-3.4.0)
include_directories(../opencv/build/include/opencv2)

set("OpenCV_DIR" "../opencv/build/")
find_package(OpenCV REQUIRED core imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(img_sound_proc main.cpp)
target_link_libraries(img_sound_proc ${OpenCV_LIBS})