# CMake generated Testfile for 
# Source directory: /home/fedor/Desktop/img_sound_proc/opencv/opencv-4.x/modules/highgui
# Build directory: /home/fedor/Desktop/img_sound_proc/opencv/build/modules/highgui
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_highgui "/home/fedor/Desktop/img_sound_proc/opencv/build/bin/opencv_test_highgui" "--gtest_output=xml:opencv_test_highgui.xml")
set_tests_properties(opencv_test_highgui PROPERTIES  LABELS "Main;opencv_highgui;Accuracy" WORKING_DIRECTORY "/home/fedor/Desktop/img_sound_proc/opencv/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/fedor/Desktop/img_sound_proc/opencv/opencv-4.x/cmake/OpenCVUtils.cmake;1752;add_test;/home/fedor/Desktop/img_sound_proc/opencv/opencv-4.x/cmake/OpenCVModule.cmake;1352;ocv_add_test_from_target;/home/fedor/Desktop/img_sound_proc/opencv/opencv-4.x/modules/highgui/CMakeLists.txt;318;ocv_add_accuracy_tests;/home/fedor/Desktop/img_sound_proc/opencv/opencv-4.x/modules/highgui/CMakeLists.txt;0;")
