#ifndef ECOSIM_HERBIVORE_HPP
#define ECOSIM_HERBIVORE_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"

class Herbivore : EcosystemElement {
public:
    explicit Herbivore(const char &characterID) : charID(characterID) {}

    NCURSES_COLOR_T getColorPair() override { return Herbivore::colorPair; }

    char getCharID() override { return this->charID; }

private:
    const static NCURSES_COLOR_T colorPair = 4;
    char charID;
};

#endif //ECOSIM_HERBIVORE_HPP
