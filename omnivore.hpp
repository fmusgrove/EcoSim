#ifndef ECOSIM_OMNIVORE_HPP
#define ECOSIM_OMNIVORE_HPP

#include "ecosystem_element.hpp"
#include "ncurses.h"
#include <vector>

class Omnivore : public EcosystemElement {
public:
    explicit Omnivore(const char &characterID, const std::vector<char> &foodChainList, const int energyPoints)
            : charID(characterID),
              energy(energyPoints),
              foodChain(foodChainList.begin(), foodChainList.end()) {}

    NCURSES_COLOR_T getColorPair() override { return Omnivore::colorPair; }

    char getCharID() const override { return this->charID; }

    int getEnergy() const override { return this->energy; }

    std::vector<char> getFoodChain() const override { return this->foodChain; }

private:
    const static NCURSES_COLOR_T colorPair = 5;
    char charID;
    std::vector<char> foodChain;
    int energy;
};

#endif //ECOSIM_OMNIVORE_HPP
