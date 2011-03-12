cmake_minimum_required(VERSION 2.6)

PROJECT(ExtractComponent)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ExtractComponent ExtractComponent.cxx)
TARGET_LINK_LIBRARIES(ExtractComponent ${ITK_LIBRARIES})
