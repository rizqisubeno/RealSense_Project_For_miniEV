# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(check_depth)

add_executable(check_depth check_depth.cpp)
find_package(realsense2 REQUIRED)
set_property(TARGET check_depth PROPERTY CXX_STANDARD 11)
target_link_libraries(check_depth ${DEPENDENCIES} ${realsense2_LIBRARY})
set_target_properties (check_depth PROPERTIES FOLDER "REALSENSE-PROJECT")

install(TARGETS check_depth RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
