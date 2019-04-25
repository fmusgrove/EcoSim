#ifndef ECOSIM_PLANT_HPP
#define ECOSIM_PLANT_HPP

#include "ncurses.h"
#include "ecosystem_element.hpp"
#include "species_type.hpp"

class Plant : public EcosystemElement {
public:
    explicit Plant(const char &characterID, const Point initialLocation, const int regrowthCoefficient,
                   const int energyPoints)
            : charID(characterID),
              cachedLocation(initialLocation),
              regrowthCoeff(regrowthCoefficient),
              energy(energyPoints) {}

    void tick() override;

    void doEat();

    NCURSES_COLOR_T getColorPair() const override { return Plant::colorPair; }

    SpeciesType getSpeciesType() const override { return Plant::speciesType; }

    Point getCachedLocation() const override { return this->cachedLocation; }

    void setCachedLocation(const Point &location) override { this->cachedLocation = location; }

    char getCharID() const override { return this->charID; }

    int getRegrowthCoeff() const override { return this->regrowthCoeff; }

    int getEnergy() const override { return this->energy; }

private:
    NCURSES_COLOR_T colorPair = 1;
    const static SpeciesType speciesType = SpeciesType::PLANT;
    Point cachedLocation;
    char charID;
    int regrowthCoeff;
    int energy;
    int regrowthStep = 0;
    bool isGrown = true;
};

#endif //ECOSIM_PLANT_HPP
