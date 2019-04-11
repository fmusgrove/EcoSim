#ifndef ECOSIM_PLANT_HPP
#define ECOSIM_PLANT_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"

class Plant : EcosystemElement {
public:
    explicit Plant(const char &characterID) : charID(characterID) {}

    NCURSES_COLOR_T getColorPair() override { return Plant::colorPair; }

    char getCharID() override { return this->charID; }

private:
    const static NCURSES_COLOR_T colorPair = 1;
    char charID;
};

#endif //ECOSIM_PLANT_HPP
