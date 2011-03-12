cmake_minimum_required(VERSION 2.6)

PROJECT(ReplaceComponent)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ReplaceComponent ReplaceComponent.cxx)
TARGET_LINK_LIBRARIES(ReplaceComponent ${ITK_LIBRARIES})
