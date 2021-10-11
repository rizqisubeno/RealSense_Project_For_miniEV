# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(color_camera)

add_executable(color_camera color_camera.cpp)
find_package(OpenCV REQUIRED )
find_package(realsense2 REQUIRED)
set_property(TARGET color_camera PROPERTY CXX_STANDARD 11)
target_link_libraries(color_camera ${DEPENDENCIES} ${realsense2_LIBRARY} ${OpenCV_LIBS})
set_target_properties (color_camera PROPERTIES FOLDER "REALSENSE-PROJECT")

install(TARGETS color_camera RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
