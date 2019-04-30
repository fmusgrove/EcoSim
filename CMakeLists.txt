cmake_minimum_required(VERSION 3.13)
project(EcoSim)
set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(EcoSim main.cpp species_type.hpp ecosystem_element.cpp ecosystem_element.hpp plant.cpp plant.hpp herbivore.cpp herbivore.hpp omnivore.cpp omnivore.hpp map_manager.cpp map_manager.hpp sim_utilities.hpp sim_utilities.cpp)
target_link_libraries(EcoSim ${CURSES_LIBRARIES})