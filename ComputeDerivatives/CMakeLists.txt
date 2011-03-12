cmake_minimum_required(VERSION 2.6)

PROJECT(ComputeDerivatives)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ComputeDerivatives ComputeDerivatives.cxx)
TARGET_LINK_LIBRARIES(ComputeDerivatives ${ITK_LIBRARIES})
