cmake_minimum_required(VERSION 3.13)
project(EcoSim)
set(CMAKE_CXX_STANDARD 17)
set(COMMON_SOURCES species_type.hpp ecosystem_element.cpp ecosystem_element.hpp plant.cpp plant.hpp herbivore.cpp herbivore.hpp omnivore.cpp omnivore.hpp map_manager.cpp map_manager.hpp sim_utilities.hpp sim_utilities.cpp)

# Necessary for static default map and species filepath definitions using both CMake and Clang
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(EcoSim main.cpp ${COMMON_SOURCES})
target_link_libraries(EcoSim ${CURSES_LIBRARIES})

add_executable(EcoSimTest tests.cpp ${COMMON_SOURCES})
set_target_properties(EcoSimTest PROPERTIES COMPILE_DEFINITIONS CURSES_DISABLED)