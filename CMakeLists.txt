cmake_minimum_required(VERSION 3.13)
project(EcoSim)
set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(EcoSim main.cpp species_type.hpp)
target_link_libraries(EcoSim ${CURSES_LIBRARIES})