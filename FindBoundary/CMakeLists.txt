cmake_minimum_required(VERSION 2.6)

PROJECT(FindBoundary)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(FindBoundary FindBoundary.cxx)
TARGET_LINK_LIBRARIES(FindBoundary
ITKBasicFilters ITKCommon ITKIO)