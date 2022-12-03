cmake_minimum_required(VERSION 3.23)
project(img_sound_proc)

set(CMAKE_CXX_STANDARD 17)

include_directories(../eigen-3.4.0)
add_executable(img_sound_proc main.cpp)
