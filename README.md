# EcoSim
A command line simulated terrestrial ecosystem created as a capstone project for CS:3210

Requires the ncurses library to be installed on the system at compilation (macOS and presumably other Unix-based
systems come with it already installed). Use the **-lcurses** flag to properly link ncurses with the binary.

---
### Run EcoSim

`clang++ -std=c++17 -lcurses main.cpp map_manager.cpp sim_utilities.cpp ecosystem_element.cpp plant.cpp herbivore.cpp omnivore.cpp -o main && ./main $MAP_FILEPATH $SPECIES_FILEPATH`

If no map and species filepath are specified, the simulation defaults will be used

---
### Run Catch test cases

`clang++ -std=c++17 -DCURSES_DISABLED tests.cpp map_manager.cpp sim_utilities.cpp ecosystem_element.cpp plant.cpp herbivore.cpp omnivore.cpp -o EcoSimTests && ./EcoSimTests`