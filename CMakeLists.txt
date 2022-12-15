cmake_minimum_required(VERSION 3.10)
project(img_sound_proc)

set(CMAKE_CXX_STANDARD 17)


# (Experimental) Change opencv paths in text build files
#[[
PATH_OLD = "fedor/Desktop/img_sound_proc"
PATH_NEW = "new_user/new_path/img_sound_proc"
file(READ opencv/build/OpenCVConfig.cmake FILE_CONTENTS)
string(REPLACE "${OLD_PATH}" "${NEW_PATH}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE opencv/build/OpenCVConfig.cmake "${FILE_CONTENTS}")

file(READ opencv/build/OpenCVModules.cmake FILE_CONTENTS)
string(REPLACE "${OLD_PATH}" "${NEW_PATH}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE opencv/build/OpenCVModules.cmake "${FILE_CONTENTS}")

file(READ opencv/build/CMakeVars.txt FILE_CONTENTS)
string(REPLACE "${OLD_PATH}" "${NEW_PATH}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE opencv/build/CMakeVars.txt "${FILE_CONTENTS}")

file(READ opencv/build/CPackConfig.cmake FILE_CONTENTS)
string(REPLACE "${OLD_PATH}" "${NEW_PATH}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE opencv/build/CPackConfig.cmake "${FILE_CONTENTS}")

file(READ opencv/build/CPackSourceConfig.cmake FILE_CONTENTS)
string(REPLACE "${OLD_PATH}" "${NEW_PATH}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE opencv/build/CPackSourceConfig.cmake "${FILE_CONTENTS}")
#]]


# get Googlet Test
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)


# Include directories (modify if using different versions or paths)
include_directories(eigen-3.4.0/)
include_directories(AudioFile/)
include_directories(opencv/build/opencv2/)
include_directories(opencv/build/lib/)
set("OpenCV_DIR" "opencv/build/")
find_package(OpenCV REQUIRED core imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})


# Link libraries
add_executable(img_sound_proc main.cpp utils.cpp transforms.cpp parsing.cpp)
target_link_libraries(img_sound_proc stdc++fs ${OpenCV_LIBS})


# Link libraries for tests (todo: separate into a different cmake file?)
#[[
enable_testing()
add_executable(my_test test.cpp utils.cpp transforms.cpp parsing.cpp)
find_package(GTest REQUIRED)
target_link_libraries(my_test stdc++fs ${OpenCV_LIBS} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(my_test)
#]]