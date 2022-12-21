cmake_minimum_required(VERSION 3.0.0)
project(GLAD VERSION 0.1.0)

# compile options for release
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++11 -Wall -pedantic")

# declaring paths for source, include
set(SOURCE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(glad STATIC ${SOURCE_PATH}/glad.c)

# include header files
target_include_directories(glad PRIVATE ${INCLUDE_PATH})