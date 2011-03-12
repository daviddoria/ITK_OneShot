cmake_minimum_required(VERSION 2.6)

PROJECT(MultiplyScalar)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(MultiplyScalar MultiplyScalar.cxx)
TARGET_LINK_LIBRARIES(MultiplyScalar ${ITK_LIBRARIES})
