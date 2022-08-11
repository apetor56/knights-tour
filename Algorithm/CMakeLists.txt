cmake_minimum_required(VERSION 3.0.0)
project(Algorithm VERSION 0.1.0)

add_compile_options(-std=c++17 -Wall -pedantic)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SOURCE_PATH     "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/include")


# Knight problem
add_executable(KnightMain
    ${SOURCE_PATH}/KnightMain.cpp    
    ${SOURCE_PATH}/KnightsTour.cpp    
    # ${SRC}/GraphException.cpp
)

add_executable(convert_output
    ${SOURCE_PATH}/convert_output.cpp
)

target_include_directories(KnightMain PRIVATE ${INCLUDE_PATH})