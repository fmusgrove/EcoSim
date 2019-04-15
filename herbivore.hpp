#ifndef ECOSIM_HERBIVORE_HPP
#define ECOSIM_HERBIVORE_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"
#include <vector>

class Herbivore : EcosystemElement {
public:
    explicit Herbivore(const char &characterID, const std::vector<char> &&foodChainList, const int energyPoints)
            : charID(characterID),
              energy(energyPoints) {
        std::move(foodChainList.begin(), foodChainList.end(), foodChain.begin());
    }

    NCURSES_COLOR_T getColorPair() override { return Herbivore::colorPair; }

    char getCharID() const override { return this->charID; }

    int getEnergy() const override { return this->energy; }

    std::vector<char> getFoodChain() const override { return this->foodChain; }

private:
    const static NCURSES_COLOR_T colorPair = 4;
    char charID;
    std::vector<char> foodChain;
    int energy;
};

#endif //ECOSIM_HERBIVORE_HPP
