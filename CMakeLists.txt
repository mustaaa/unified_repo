cmake_minimum_required(VERSION 3.5)

project(mandelbrot_visualization)
set (CMAKE_CXX_STANDARD 11)

add_executable(mandelbrot_visualization main.cpp
	CellularAutomata.h
)
execute_process(COMMAND git submodule update --init -- 3rdParty/SFML
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SFML_STATIC_LIBRARIES TRUE)
set(BUILD_SHARED_LIBS FALSE)

target_link_libraries(mandelbrot_visualization PUBLIC sfml-graphics)

set_property(TARGET mandelbrot_visualization PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:mandelbrot_visualization>)

add_subdirectory(3rdParty)