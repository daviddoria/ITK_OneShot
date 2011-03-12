cmake_minimum_required(VERSION 2.6)

PROJECT(AppendImageComponents)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(AppendImageComponents AppendImageComponents.cxx)
TARGET_LINK_LIBRARIES(AppendImageComponents ${ITK_LIBRARIES})
