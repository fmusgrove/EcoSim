# EcoSim
A command line simulated terrestrial ecosystem created as a capstone project for CS:3210

Requires the ncurses library to be installed on the system at compilation (macOS and presumably other Unix-based
systems come with it already installed). Use the **-lcurses** flag to properly link ncurses with the binary.

---
### Run EcoSim

`clang++ -std=c++17 -lcurses main.cpp map_manager.cpp sim_utilities.cpp ecosystem_element.cpp plant.cpp herbivore.cpp omnivore.cpp -o EcoSim && ./EcoSim $MAP_FILEPATH $SPECIES_FILEPATH`

If no map and species filepath are specified, the simulation defaults will be used

---
### Run Catch test cases

The **-DCURSES_DISABLED** flag needs to be added when running tests to set the CURSES_DISABLED macro which will stop ncurses from initializing and masking the output from Catch

`clang++ -std=c++17 -DCURSES_DISABLED tests.cpp map_manager.cpp sim_utilities.cpp ecosystem_element.cpp plant.cpp herbivore.cpp omnivore.cpp -o EcoSimTest && ./EcoSimTest`