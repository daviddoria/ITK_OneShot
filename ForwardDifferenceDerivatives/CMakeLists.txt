cmake_minimum_required(VERSION 2.6)

PROJECT(ForwardDifferenceDerivatives)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ForwardDifferenceDerivatives ForwardDifferenceDerivatives.cxx)
TARGET_LINK_LIBRARIES(ForwardDifferenceDerivatives ${ITK_LIBRARIES})
