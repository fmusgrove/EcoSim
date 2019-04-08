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

void drawMap(unique_ptr<vector<vector<char>>> &map);

int main(int argc, char **argv) {
    string script_filepath(argv[0]);
    string mapFilePath, speciesFilePath, fileLine;
    auto map = make_unique<vector<vector<char>>>();
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

    // Load map into memory
    ifstream mapFile(mapFilePath);
    if (mapFile.is_open()) {
        while (getline(mapFile, fileLine)) {
            std::vector<char> line_data(fileLine.begin(), fileLine.end());
            map->push_back(line_data);
        }
        mapFile.close();
    } else {
        cerr << "Unable to open file '" << mapFilePath << "'" << endl;
        return -1;
    }

    // Initialize curses and setup main screen
    initscr();
    if (has_colors() == FALSE) {
        printf("Your terminal does not support color\n");
    } else {
        start_color();

        // Define color pairs for varying map elements
        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
    }

    drawMap(map);

    // Wait for user input
    getch();

    // End curses mode and exit
    endwin();
    return 0;
}

void drawMap(unique_ptr<vector<vector<char>>> &map) {
    for (auto &mapLineVector: *map) {
        for (char &lineChar: mapLineVector) {
            switch (lineChar) {
                case '~' :
                    // Water
                    attron(COLOR_PAIR(2));
                    addch(lineChar);
                    attroff(COLOR_PAIR(2));
                    break;
                case '#':
                    // Obstacle
                    attron(COLOR_PAIR(3));
                    addch(lineChar);
                    attroff(COLOR_PAIR(3));
                    break;
                default:
                    // Default to green on black text
                    attron(COLOR_PAIR(1));
                    addch(lineChar);
                    attroff(COLOR_PAIR(1));
                    break;
            }

        }
        addch('\n');
    }

    refresh();
}