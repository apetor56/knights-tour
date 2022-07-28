cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_basics VERSION 0.1.0)

# OS specific instructions
# setting correct preffix and suffix for static libraries
if(WIN32)
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
elseif(UNIX OR APPLE)
    set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
endif()

add_compile_options(-std=c++17 -g -Wall -pedantic)

# set more convenient .exe output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# declaring paths for source, include and libraries 
set(SOURCE_PATH  "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")
set(LIB_PATH     "${CMAKE_SOURCE_DIR}/lib")

# list of libs linked later against main program
list(APPEND LIBS
    ${LIB_PATH}/glad.lib
    ${LIB_PATH}/glfw3.lib
)

#create main program
add_executable(main
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/Window.cpp
    ${SOURCE_PATH}/Exception.cpp
    ${SOURCE_PATH}/Shader.cpp
)

# include header files and libraries
target_include_directories(main PRIVATE ${INCLUDE_PATH})
target_link_libraries(main PRIVATE ${LIBS})