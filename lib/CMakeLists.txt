# pre-build libraries

cmake_minimum_required(VERSION 3.0.0)
project(libs VERSION 0.1.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(src/glad)
add_subdirectory(src/glfw)