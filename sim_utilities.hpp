#ifndef ECOSIM_SIM_UTILITIES_HPP
#define ECOSIM_SIM_UTILITIES_HPP

#include <string>
#include <random>
#include <iterator>
#include "ncurses.h"
#include "ecosystem_element.hpp"
#include "map_manager.hpp"

namespace SimUtilities {

    struct SpeciesTraits {
    public:
        string speciesType;
        int regrowthCoeff;
        int energy;
        vector<char> foodChain;
    };

    void drawMap(WINDOW *window, const int mapOffsetY, const int mapOffsetX, bool has_border);

    WINDOW *createWindow(int height, int width, int startY, int startX, bool addBorders);

    void destroyWindow(WINDOW *local_win);

    void windowPrintString(WINDOW *window, const char *printString, bool has_border);

    int windowPromptInt(WINDOW *window, const char *promptString, int bufferSize);

    string windowPromptStr(WINDOW *window, const char *promptString, vector<string> &allowedValues, int bufferSize);

    std::vector<Point> randomSelect(const std::vector<Point> &locations, size_t count);

    double getValUniformRandDist();
}

#endif //ECOSIM_SIM_UTILITIES_HPP