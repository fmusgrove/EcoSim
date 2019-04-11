#ifndef ECOSIM_ECOSYSTEM_ELEMENT_HPP
#define ECOSIM_ECOSYSTEM_ELEMENT_HPP

#include <utility>
#include "ncurses.h"

using Point = std::pair<int, int>;

/**
 * Top-level ecosystem class that all other types inherit from. Used for
 * polymorphic storage of the ecosystem map
 */
class EcosystemElement {
public:
    virtual NCURSES_COLOR_T getColorPair() { return EcosystemElement::colorPair; }

    virtual char getCharID() { return this->charID; }

private:
    const static NCURSES_COLOR_T colorPair = 0;
    const char charID = '?';
};


#endif //ECOSIM_ECOSYSTEM_ELEMENT_HPP
