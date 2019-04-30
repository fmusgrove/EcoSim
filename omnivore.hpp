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

    NCURSES_COLOR_T getColorPair() const override { return Omnivore::colorPair; }

    SpeciesType getSpeciesType() const override { return Omnivore::speciesType; }

    Point getCachedLocation() const override { return this->cachedLocation; }

    void setCachedLocation(const Point &location) override { this->cachedLocation = location; }

    char getCharID() const override { return this->charID; }

    int getCurrentEnergy() const override { return this->currentEnergy; }

    void setCurrentEnergy(const int energyToSet) override { this->currentEnergy = energyToSet; }

    int getMaxEnergy() const override { return this->maxEnergy; }

    std::vector<char> getFoodChain() const override { return this->foodChain; }

private:
    const static NCURSES_COLOR_T colorPair = 5;
    const static SpeciesType speciesType = SpeciesType::OMNIVORE;
    Point cachedLocation;
    const char charID;
    std::vector<char> foodChain;
    int currentEnergy;
    int maxEnergy;
};

#endif //ECOSIM_OMNIVORE_HPP
