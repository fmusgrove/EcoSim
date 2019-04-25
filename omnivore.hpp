#ifndef ECOSIM_OMNIVORE_HPP
#define ECOSIM_OMNIVORE_HPP

#include <vector>
#include "ncurses.h"

#include "ecosystem_element.hpp"
#include "species_type.hpp"

class Omnivore : public EcosystemElement {
public:
    explicit Omnivore(const char &characterID, const Point initialLocation, const std::vector<char> &foodChainList,
                      const int maxEnergyPoints)
            : charID(characterID),
              cachedLocation(initialLocation),
              maxEnergy(maxEnergyPoints),
              currentEnergy(maxEnergyPoints),
              foodChain(foodChainList.begin(), foodChainList.end()) {}

    void tick() override;

    NCURSES_COLOR_T getColorPair() override { return Omnivore::colorPair; }

    SpeciesType getSpeciesType() override { return Omnivore::speciesType; }

    char getCharID() const override { return this->charID; }

    int getEnergy() const override { return this->maxEnergy; }

    std::vector<char> getFoodChain() const override { return this->foodChain; }

    Point cachedLocation;
private:
    const static NCURSES_COLOR_T colorPair = 5;
    const static SpeciesType speciesType = SpeciesType::OMNIVORE;
    char charID;
    std::vector<char> foodChain;
    const int maxEnergy;
    int currentEnergy;
};

#endif //ECOSIM_OMNIVORE_HPP
