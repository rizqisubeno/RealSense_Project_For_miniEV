# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(myCamera)

add_executable(myCamera myCamera.cpp)
find_package(OpenCV REQUIRED )
find_package(realsense2 REQUIRED)
set_property(TARGET myCamera PROPERTY CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
target_link_libraries(myCamera ${DEPENDENCIES} ${realsense2_LIBRARY} ${OpenCV_LIBS})
set_target_properties (myCamera PROPERTIES FOLDER "REALSENSE-PROJECT")

install(TARGETS myCamera RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
