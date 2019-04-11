#ifndef ECOSIM_OMNIVORE_HPP
#define ECOSIM_OMNIVORE_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"

class Omnivore : EcosystemElement {
public:
    explicit Omnivore(const char &characterID) : charID(characterID) {}

    NCURSES_COLOR_T getColorPair() override { return Omnivore::colorPair; }

    char getCharID() override { return this->charID; }

private:
    const static NCURSES_COLOR_T colorPair = 3;
    char charID;
};

#endif //ECOSIM_OMNIVORE_HPP
