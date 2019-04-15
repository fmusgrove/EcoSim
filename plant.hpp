#ifndef ECOSIM_PLANT_HPP
#define ECOSIM_PLANT_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"

class Plant : public EcosystemElement {
public:
    explicit Plant(const char &characterID, const int regrowthCoefficient, const int energyPoints)
            : charID(characterID),
              regrowthCoeff(regrowthCoefficient),
              energy(energyPoints) {}

    NCURSES_COLOR_T getColorPair() override { return Plant::colorPair; }

    char getCharID() const override { return this->charID; }

    int getRegrowthCoeff() const override { return this->regrowthCoeff; }

    int getEnergy() const override { return this->energy; }

private:
    const static NCURSES_COLOR_T colorPair = 1;
    char charID;
    int regrowthCoeff;
    int energy;
};

#endif //ECOSIM_PLANT_HPP
