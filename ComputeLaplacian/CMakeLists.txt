cmake_minimum_required(VERSION 2.6)

PROJECT(ComputeLaplacian)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ComputeLaplacian ComputeLaplacian.cxx)
TARGET_LINK_LIBRARIES(ComputeLaplacian ${ITK_LIBRARIES})
