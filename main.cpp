//
// EcoSim - Ecosystem Simulator CLI
// Author - Forrest Musgrove
//

#include <iostream>
#include <string>
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
//#define CURSES_DISABLED

using namespace std;

int main(int argc, char **argv) {
    string mapFilePath, speciesFilePath;
    unordered_map<char, SimUtilities::SpeciesTraits> speciesList;

    // Get arguments and set default map and species files if none are specified
    if (argc >= 3) {
        mapFilePath = argv[1];
        speciesFilePath = argv[2];
    } else {
        mapFilePath = "default_input/map.txt";
        speciesFilePath = "default_input/species.txt";
    }

    // Load species list
    speciesList = SimUtilities::loadSpeciesList(speciesFilePath);

    // Load map into memory
    SimUtilities::loadMap(mapFilePath, speciesList);

    //region Curses setup
#ifndef CURSES_DISABLED
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
#ifndef CURSES_DISABLED
        // Prompt user for number of simulation loops to run
        tickCount = SimUtilities::windowPromptInt(commandWindow, "Enter the number of simulation loops to run: ", 10);
        SimUtilities::windowPrintString(commandWindow, "Running Simulation", true);
#else
        tickCount = 20;
#endif

        // Run the simulation for the defined number of steps
        for (int tickNum = 0; tickNum < tickCount; tickNum++) {
            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::PLANT) {
                    element.second->tick();
                }
            }

            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::HERBIVORE) {
                    // Check if energy levels are depleted
                    if (element.second->getCurrentEnergy() <= 0) {
                        MapManager::killElement(*element.second);
                    } else {
                        element.second->tick();
                    }
                }
            }

            for (auto &element: MapManager::floraFauna) {
                if (element.second->getSpeciesType() == SpeciesType::OMNIVORE) {
                    // Check if energy levels are depleted
                    if (element.second->getCurrentEnergy() <= 0) {
                        MapManager::killElement(*element.second);
                    } else {
                        element.second->tick();
                    }
                }
            }

#ifndef CURSES_DISABLED
            SimUtilities::drawMap(simulationWindow, MAP_OFFSET_Y, MAP_OFFSET_X, true);
#endif
            // Sleep to allow the user to see the result of each simulation cycle
            this_thread::sleep_for(chrono::milliseconds(500));
        }

#ifndef CURSES_DISABLED
        vector<string> allowedValues = {"y", "n"};
        string continueRunning = SimUtilities::windowPromptStr(commandWindow,
                                                               "Continue running simulation?(y/n): ",
                                                               allowedValues, true);
        if (continueRunning == "y") {
            shouldStop = false;
        } else {
            string shouldSaveState = SimUtilities::windowPromptStr(commandWindow,
                                                                   "Save ecosystem state to file?(y/n): ",
                                                                   allowedValues, true);
            if (shouldSaveState == "y") {
                allowedValues = {"*"};
                string saveFileName = SimUtilities::windowPromptStr(commandWindow,
                                                                    "Enter a filename to save the map: ",
                                                                    allowedValues, true);
                if (!MapManager::saveMapToFile(saveFileName)) {
                    SimUtilities::windowPrintString(commandWindow, "An error occurred while saving the map. Exiting...",
                                                    true);
                    this_thread::sleep_for(chrono::milliseconds(2000));
                } else {
                    cout << "Map saved to " << saveFileName << endl;
                }
            }
            shouldStop = true;
        }
#else
        shouldStop = true;
#endif
    }
    //endregion

#ifndef CURSES_DISABLED
    SimUtilities::destroyWindow(topBanner);
    SimUtilities::destroyWindow(simulationWindow);
    SimUtilities::destroyWindow(commandWindow);
    // End curses mode and exit
    endwin();
#endif

    cout << "Simulation complete" << endl;
    return 0;
}
