#ifndef ECOSIM_ECOSYSTEM_ELEMENT_HPP
#define ECOSIM_ECOSYSTEM_ELEMENT_HPP

#include <utility>
#include <vector>
#include "ncurses.h"

#include "species_type.hpp"

using Point = std::pair<int, int>;

/**
 * Top-level ecosystem class that all other types inherit from. Used for
 * polymorphic storage of the ecosystem map
 */
class EcosystemElement {
public:
    virtual void tick() {}

    virtual NCURSES_COLOR_T getColorPair() { return EcosystemElement::colorPair; }

    virtual SpeciesType getSpeciesType() { return EcosystemElement::speciesType; }

    virtual char getCharID() const { return this->charID; }

    virtual int getRegrowthCoeff() const { return this->regrowthCoeff; }

    virtual int getEnergy() const { return this->energy; }

    virtual std::vector<char> getFoodChain() const { return this->foodChain; }

    Point cachedLocation;
private:
    const static NCURSES_COLOR_T colorPair = 0;
    const static SpeciesType speciesType = SpeciesType::GENERAL_ELEMENT;
    const char charID = '?';
    int regrowthCoeff = -1;
    int energy = -1;
    std::vector<char> foodChain;
};


#endif //ECOSIM_ECOSYSTEM_ELEMENT_HPP
