#include "plant.hpp"

void Plant::tick() {
    if (!isGrown) {
        regrowthStep++;
        if (regrowthStep == regrowthCoeff) {
            isGrown = true;
            colorPair = 1;
        }
    }

}

void Plant::doEat() {
    isGrown = false;
    regrowthStep = 0;
    colorPair = 6;
}