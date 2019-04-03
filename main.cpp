//
// EcoSim - Ecosystem Simulator CLI
// Author - Forrest Musgrove
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ncurses.h"

#include "species_type.hpp"

using namespace std;

int main(int argc, char **argv) {
    string mapFilePath, speciesFilePath, fileLine;
    vector<vector<char>> map;
    // Get arguments and set default map and species files if none are specified
    if (argc >= 3) {
        mapFilePath = argv[1];
        speciesFilePath = argv[2];
    } else {
        mapFilePath = "default_input/map.txt";
        speciesFilePath = "default_input/species.txt";
    }

    // Load map into memory
    ifstream mapFile(mapFilePath);
    if (mapFile.is_open()) {
        while (getline(mapFile, fileLine)) {
            cout << fileLine << '\n';
        }
        mapFile.close();
    } else {
        cerr << "Unable to open file '" << mapFilePath << "'" << endl;
        return -1;
    }

    // Initialize curses and setup main screen
    initscr();


    refresh();

    // Wait for user input
    getch();

    // End curses mode and exit
    endwin();
    return 0;
}