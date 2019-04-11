//
// EcoSim - Ecosystem Simulator CLI
// Author - Forrest Musgrove
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "ncurses.h"

#include "species_type.hpp"
#include "ecosystem_element.hpp"

using namespace std;
using WaterObstacleList = vector<pair<Point, char>>;
using FloraFaunaList = multimap<Point, EcosystemElement>;

void drawMap(const unique_ptr<WaterObstacleList> &waterObstacle, const unique_ptr<FloraFaunaList> &floraFauna);

// TODO: Make coordinates relative to the map starting point so curses can draw it wherever, such as the
//  center of the screen

int main(int argc, char **argv) {
    string script_filepath(argv[0]);
    string mapFilePath, speciesFilePath, fileLine;
    auto floraFauna = make_unique<FloraFaunaList>();
    auto terrain = make_unique<WaterObstacleList>();
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
    int xPos = 0;
    int yPos = 0;
    ifstream mapFile(mapFilePath);
    if (mapFile.is_open()) {
        while (getline(mapFile, fileLine)) {
            xPos = 0;
            for (char mapChar : fileLine) {
                if (mapChar == '~' || mapChar == '#') {
                    // Terrain element
                    Point location(xPos, yPos);
                    terrain->push_back(pair(location, mapChar));
                } else {
                    // Flora or fauna element

                }
                xPos++;
            }
            yPos++;
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
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    }

    drawMap(terrain, floraFauna);

    // Wait for user input
    getch();

    // End curses mode and exit
    endwin();
    return 0;
}

void drawMap(const unique_ptr<WaterObstacleList> &waterObstacle, const unique_ptr<FloraFaunaList> &floraFauna) {
    // Draw terrain
    for (auto &pointCharPair: *waterObstacle) {
        switch (pointCharPair.second) {
            case '~' :
                // Water
                attron(COLOR_PAIR(2));
                // Cursor location entered in the form  row, column (y, x)
                mvaddch(pointCharPair.first.second, pointCharPair.first.first, '~');
                attroff(COLOR_PAIR(2));
                break;
            case '#':
                // Obstacle
                attron(COLOR_PAIR(3));
                mvaddch(pointCharPair.first.second, pointCharPair.first.first, '#');
                attroff(COLOR_PAIR(3));
                break;
            default:
                break;
        }
    }

    // Draw plants and animals
    for (auto &element: *floraFauna) {
        attron(COLOR_PAIR(element.second.getColorPair()));
        // Cursor location entered in the form  row, column (y, x)
        mvaddch(element.first.second, element.first.first, element.second.getCharID());
        attroff(COLOR_PAIR(element.second.getColorPair()));
    }

    refresh();
}