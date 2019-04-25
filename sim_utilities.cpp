#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "ncurses.h"

#include "ecosystem_element.hpp"
#include "map_manager.hpp"

using namespace std;
using WaterObstacleList = map<Point, char>;
using FloraFaunaList = multimap<Point, unique_ptr<EcosystemElement>>;

struct SpeciesTraits {
public:
    string speciesType;
    int regrowthCoeff;
    int energy;
    vector<char> foodChain;
};

void drawMap(WINDOW *window, const int mapOffsetY, const int mapOffsetX) {
    // Cursor location entered in the form row, column (y, x)
    // Draw terrain
    for (auto &pointCharPair: *MapManager::terrain) {
        switch (pointCharPair.second) {
            case '~' :
                // Water
                wattron(window, COLOR_PAIR(2));
                mvwaddch(window, pointCharPair.first.second + mapOffsetY, pointCharPair.first.first + mapOffsetX, '~');
                wattroff(window, COLOR_PAIR(2));
                break;
            case '#':
                // Obstacle
                wattron(window, COLOR_PAIR(3));
                mvwaddch(window, pointCharPair.first.second + mapOffsetY, pointCharPair.first.first + mapOffsetX, '#');
                wattroff(window, COLOR_PAIR(3));
                break;
            default:
                break;
        }
    }

    // Draw plants and animals
    for (auto &element: *MapManager::floraFauna) {
        wattron(window, COLOR_PAIR(element.second->getColorPair()));
        mvwaddch(window, element.first.second + mapOffsetY, element.first.first + mapOffsetX,
                 element.second->getCharID());
        wattroff(window, COLOR_PAIR(element.second->getColorPair()));
    }

    wrefresh(window);
}

WINDOW *createWindow(int height, int width, int startY, int startX, bool addBorders) {
    auto local_win = newwin(height, width, startY, startX);
    // Add border around the window
    if (addBorders) {
        box(local_win, 0, 0);
    }
    // Show the border
    wrefresh(local_win);
    return local_win;
}

void destroyWindow(WINDOW *local_win) {
    // Erase the window borders
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_win);
    // Remove the window
    delwin(local_win);
}