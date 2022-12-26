cmake_minimum_required(VERSION 3.4.0)
project(OpenGL VERSION 0.1.0)

# compile options for release
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++11 -Wall -pedantic")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# compile options for debugging (including glDebugMessageControl)
# warning: there will be error relative to line width
# (without debugging we can change line width over 1)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++11 -g -Wall -pedantic -D glDebug")

# set more convenient .exe output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# declaring paths for source, include and libraries 
set(SOURCE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include"
                 "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui")
set(LIB_PATH     "${CMAKE_CURRENT_SOURCE_DIR}/lib/bin")

# list of libs linked later against main program
list(APPEND LIBS
    ${LIB_PATH}/libglad.a
    ${LIB_PATH}/libglfw3.a
)

#create main program
add_executable(main
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/Window.cpp
    ${SOURCE_PATH}/Exception.cpp
    ${SOURCE_PATH}/Shader.cpp
    ${SOURCE_PATH}/Renderer.cpp
    ${SOURCE_PATH}/stb_image.cpp
    ${SOURCE_PATH}/Texture2D.cpp
    ${SOURCE_PATH}/VertexArray.cpp
    ${SOURCE_PATH}/KnightsTour.cpp

    ${SOURCE_PATH}/imgui/imgui.cpp
    ${SOURCE_PATH}/imgui/imgui_demo.cpp
    ${SOURCE_PATH}/imgui/imgui_draw.cpp
    ${SOURCE_PATH}/imgui/imgui_impl_opengl3.cpp
    ${SOURCE_PATH}/imgui/imgui_impl_glfw.cpp
    ${SOURCE_PATH}/imgui/imgui_tables.cpp
    ${SOURCE_PATH}/imgui/imgui_widgets.cpp
)

# include header files and link libraries
target_include_directories(main PRIVATE ${INCLUDE_PATH})
target_link_libraries(main PRIVATE ${LIBS})