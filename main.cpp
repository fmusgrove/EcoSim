//
// EcoSim - Ecosystem Simulator CLI
// Author - Forrest Musgrove
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>
#include <unordered_map>
#include "ncurses.h"

#include "sim_utilities.hpp"
#include "map_manager.hpp"
#include "species_type.hpp"
#include "ecosystem_element.hpp"
#include "plant.hpp"
#include "herbivore.hpp"
#include "omnivore.hpp"

// Macro to disable ncurses for debugging purposes
//#define DEBUG_MODE

using namespace std;

int main(int argc, char **argv) {
    string script_filepath(argv[0]);
    string mapFilePath, speciesFilePath, fileLine;

    // Get arguments and set default map and species files if none are specified
    if (argc >= 3) {
        mapFilePath = argv[1];
        speciesFilePath = argv[2];
    } else if (script_filepath.find("cmake") != string::npos) {
        mapFilePath = "../default_input/map.txt";
        speciesFilePath = "../default_input/species.txt";
    } else {
        mapFilePath = "default_input/map.txt";
        speciesFilePath = "default_input/species.txt";
    }

    //region Map and file loading
    // Load species list
    ifstream speciesFile(speciesFilePath);
    istringstream stringTraitStream;
    string traitString;
    unordered_map<char, SimUtilities::SpeciesTraits> speciesList;
    if (speciesFile.is_open()) {
        while (getline(speciesFile, fileLine)) {
            stringTraitStream.clear();
            stringTraitStream.str(fileLine);
            string speciesType;
            char speciesID;
            vector<char> foodChain;
            int regrowthCoeff = -1;
            int energy = -1;

            // Read the species type
            stringTraitStream >> traitString;
            speciesType = traitString;
            transform(speciesType.begin(), speciesType.end(), speciesType.begin(), ::tolower);
            // Read the character ID
            stringTraitStream >> traitString;
            speciesID = traitString[0];

            // Read the rest of the traits
            do {
                stringTraitStream >> traitString;

                // Assemble food chain
                if (traitString.find('[') != string::npos || traitString.find(']') != string::npos ||
                    traitString.find(',') != string::npos) {
                    traitString.erase(std::remove(traitString.begin(), traitString.end(), '['), traitString.end());
                    traitString.erase(std::remove(traitString.begin(), traitString.end(), ']'), traitString.end());
                    traitString.erase(std::remove(traitString.begin(), traitString.end(), ','), traitString.end());
                    foodChain.push_back(traitString[0]);
                    continue;
                }

                // Set regrowth coefficient for plants and maximum energy levels for both plants and animals
                if (speciesType == "plant") {
                    if (regrowthCoeff == -1) {
                        regrowthCoeff = stoi(traitString);
                    } else if (energy == -1) {
                        energy = stoi(traitString);
                    }
                } else {
                    energy = stoi(traitString);
                }

            } while (stringTraitStream);

            // Add species definition to the reference map
            speciesList.insert(pair<char, SimUtilities::SpeciesTraits>(speciesID, {speciesType, regrowthCoeff, energy,
                                                                                   foodChain}));
        }
        speciesFile.close();
    } else {
        cerr << "Unable to open file '" << speciesFilePath << "'" << endl;
        return -1;
    }

    // Load map into memory
    int xPos = 0;
    int yPos = 0;
    int mapColumns = 0;
    ifstream mapFile(mapFilePath);
    if (mapFile.is_open()) {
        while (getline(mapFile, fileLine)) {
            xPos = 0;
            for (char mapChar : fileLine) {
                Point location(xPos, yPos);
                if (mapChar == '~' || mapChar == '#') {
                    // Terrain element
                    MapManager::terrain.insert(pair(location, mapChar));
                } else if (mapChar != ' ') {
                    // Flora or fauna element
                    auto foundSpeciesType = speciesList.find(mapChar)->second;

                    if (foundSpeciesType.speciesType == "plant") {
                        MapManager::floraFauna.insert(
                                pair(location, make_unique<Plant>(mapChar, location, foundSpeciesType.regrowthCoeff,
                                                                  foundSpeciesType.energy)));
                    } else if (foundSpeciesType.speciesType == "herbivore") {
                        MapManager::floraFauna.insert(
                                pair(location, make_unique<Herbivore>(mapChar, location, foundSpeciesType.foodChain,
                                                                      foundSpeciesType.energy)));
                    } else if (foundSpeciesType.speciesType == "omnivore") {
                        MapManager::floraFauna.insert(
                                pair(location, make_unique<Omnivore>(mapChar, location, foundSpeciesType.foodChain,
                                                                     foundSpeciesType.energy)));
                    }

                }
                xPos++;
            }
            mapColumns = max(mapColumns, xPos);
            yPos++;
        }

        MapManager::mapRows = yPos;
        // Exclude newline character
        mapColumns--;
        MapManager::mapColumns = mapColumns;

        cout << "Map with " << MapManager::mapRows << " rows and " << MapManager::mapColumns << " columns loaded"
             << endl;

        mapFile.close();
    } else {
        cerr << "Unable to open file '" << mapFilePath << "'" << endl;
        return -1;
    }
    //endregion

    //region Curses setup
#ifndef DEBUG_MODE
    const int BANNER_HEIGHT = 20;
    // Initialize curses and setup main screen
    initscr();
    // Set the simulation window to take up 70% of the remaining space below the banner
    const int SIM_WINDOW_HEIGHT = (int) round((LINES - BANNER_HEIGHT) * 0.7f);
    const int COMMAND_WINDOW_HEIGHT = LINES - BANNER_HEIGHT - SIM_WINDOW_HEIGHT;
    // Offset to add for centering the map in the console
    const int MAP_OFFSET_Y = (SIM_WINDOW_HEIGHT - MapManager::mapRows) / 2;
    const int MAP_OFFSET_X = (COLS - MapManager::mapColumns) / 2;

    auto topBanner = SimUtilities::createWindow(BANNER_HEIGHT, COLS, 0, 0, false);
    auto simulationWindow = SimUtilities::createWindow(SIM_WINDOW_HEIGHT, COLS, BANNER_HEIGHT, 0, true);
    auto commandWindow = SimUtilities::createWindow(COMMAND_WINDOW_HEIGHT, COLS, BANNER_HEIGHT + SIM_WINDOW_HEIGHT, 0,
                                                    true);

    if (has_colors() == FALSE) {
        wprintw(topBanner, "Your terminal does not support color\n");
        wprintw(topBanner, "Press any key to run the simulation with the default color profile...\n");
        wrefresh(topBanner);
        getch();
        wclear(topBanner);
    } else {
        start_color();

        // Define color pairs for varying map elements
        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);

        // Set default background color of all windows
        wbkgdset(topBanner, COLOR_PAIR(1));
        wbkgdset(simulationWindow, COLOR_PAIR(1));
        wbkgdset(commandWindow, COLOR_PAIR(1));
    }

    // Banner
    wattron(topBanner, COLOR_PAIR(1));
    vector<string> bannerLines = {
            "EEEEEEEEEEEEEEEEEEEEEE                                        SSSSSSSSSSSSSSS   iiii                          \n",
            "E::::::::::::::::::::E                                      SS:::::::::::::::S i::::i                         \n",
            "E::::::::::::::::::::E                                     S:::::SSSSSS::::::S  iiii                          \n",
            "EE::::::EEEEEEEEE::::E                                     S:::::S     SSSSSSS                                \n",
            "  E:::::E       EEEEEE    cccccccccccccccc   ooooooooooo   S:::::S            iiiiiii    mmmmmmm    mmmmmmm   \n",
            "  E:::::E               cc:::::::::::::::c oo:::::::::::oo S:::::S            i:::::i  mm:::::::m  m:::::::mm \n",
            "  E::::::EEEEEEEEEE    c:::::::::::::::::co:::::::::::::::o S::::SSSS          i::::i m::::::::::mm::::::::::m\n",
            "  E:::::::::::::::E   c:::::::cccccc:::::co:::::ooooo:::::o  SS::::::SSSSS     i::::i m::::::::::::::::::::::m\n",
            "  E:::::::::::::::E   c::::::c     ccccccco::::o     o::::o    SSS::::::::SS   i::::i m:::::mmm::::::mmm:::::m\n",
            "  E::::::EEEEEEEEEE   c:::::c             o::::o     o::::o       SSSSSS::::S  i::::i m::::m   m::::m   m::::m\n",
            "  E:::::E             c:::::c             o::::o     o::::o            S:::::S i::::i m::::m   m::::m   m::::m\n",
            "  E:::::E       EEEEEEc::::::c     ccccccco::::o     o::::o            S:::::S i::::i m::::m   m::::m   m::::m\n",
            "EE::::::EEEEEEEE:::::Ec:::::::cccccc:::::co:::::ooooo:::::oSSSSSSS     S:::::Si::::::im::::m   m::::m   m::::m\n",
            "E::::::::::::::::::::E c:::::::::::::::::co:::::::::::::::oS::::::SSSSSS:::::Si::::::im::::m   m::::m   m::::m\n",
            "E::::::::::::::::::::E  cc:::::::::::::::c oo:::::::::::oo S:::::::::::::::SS i::::::im::::m   m::::m   m::::m\n",
            "EEEEEEEEEEEEEEEEEEEEEE    cccccccccccccccc   ooooooooooo    SSSSSSSSSSSSSSS   iiiiiiiimmmmmm   mmmmmm   mmmmmm\n"
    };

    // Draw the banner
    int bannerRow = 2;
    int bannerWidth = bannerLines[0].length() - 1; // Exclude newline character
    for (string &line: bannerLines) {
        mvwprintw(topBanner, bannerRow, (COLS - bannerWidth) / 2, line.c_str());
        bannerRow++;
    }
    wrefresh(topBanner);
    //endregion

    SimUtilities::drawMap(simulationWindow, MAP_OFFSET_Y, MAP_OFFSET_X, true);
#endif

    //region Main simulation tick loop
    bool shouldStop = false;
    int tickCount;
    while (!shouldStop) {
        // Prompt user for number of simulation loops to run
        tickCount = SimUtilities::windowPromptInt(commandWindow, "Enter the number of simulation loops to run: ", 10);
        SimUtilities::printStringToWindow(commandWindow, "Running Simulation", true);


        // Run the simulation for the defined number of steps
        for (int tickNum = 0; tickNum < tickCount; tickNum++) {
            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::PLANT) {
                    element.second->tick();
                }
            }

            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::HERBIVORE) {
                    element.second->tick();
                    // Check if energy levels are depleted
                    if (element.second->getCurrentEnergy() <= 0) {
                        MapManager::killElement(*element.second);
                    }
                }
            }

            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::OMNIVORE) {
                    element.second->tick();
                }
            }

#ifndef DEBUG_MODE
            SimUtilities::drawMap(simulationWindow, MAP_OFFSET_Y, MAP_OFFSET_X, true);
#endif
            // Sleep to allow the user to see the result of each simulation cycle
            this_thread::sleep_for(chrono::milliseconds(500));
        }


        vector<string> allowedValues = {"y", "n"};
        string continueRunning = SimUtilities::windowPromptStr(commandWindow, "Continue running simulation?(y/n): ",
                                                               allowedValues, true);
        if (continueRunning == "y") {
            shouldStop = false;
        } else {
            shouldStop = true;
        }
    }

    //endregion

    // Wait for user input
#ifndef DEBUG_MODE
    SimUtilities::destroyWindow(topBanner);
    SimUtilities::destroyWindow(simulationWindow);
    SimUtilities::destroyWindow(commandWindow);
    // End curses mode and exit
    endwin();
#endif

    cout << "Simulation complete" << endl;
    return 0;
}
