#ifndef ECOSIM_HERBIVORE_HPP
#define ECOSIM_HERBIVORE_HPP

#include <vector>
#include "ncurses.h"

#include "ecosystem_element.hpp"
#include "species_type.hpp"

class Herbivore : public EcosystemElement {
public:
    explicit Herbivore(const char &characterID, const Point initialLocation, const std::vector<char> &foodChainList,
                       const int maxEnergyPoints)
            : charID(characterID),
              cachedLocation(initialLocation),
              maxEnergy(maxEnergyPoints),
              currentEnergy(maxEnergyPoints),
              foodChain(foodChainList.begin(), foodChainList.end()) {}

    void tick() override;

    NCURSES_COLOR_T getColorPair() override { return Herbivore::colorPair; }

    SpeciesType getSpeciesType() override { return Herbivore::speciesType; }

    char getCharID() const override { return this->charID; }

    int getEnergy() const override { return this->maxEnergy; }

    std::vector<char> getFoodChain() const override { return this->foodChain; }

    Point cachedLocation;
private:
    const static NCURSES_COLOR_T colorPair = 4;
    const static SpeciesType speciesType = SpeciesType::HERBIVORE;
    char charID;
    std::vector<char> foodChain;
    const int maxEnergy;
    int currentEnergy;
};

#endif //ECOSIM_HERBIVORE_HPP
