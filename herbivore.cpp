#include "herbivore.hpp"

#include "map_manager.hpp"
#include "sim_utilities.hpp"

using namespace std;

void Herbivore::tick() {
    Point actionableLocation;
    auto availableLocations = MapManager::freeLocations(*this);
    auto foodNearby = MapManager::edibleFloraFaunaNearby(*this);
    auto matesNearby = MapManager::nearbyMates(*this);

    // TODO: Add logic to allow animal to run away from predators
    if (!foodNearby.empty() && currentEnergy < (0.3 * maxEnergy)) {
        // Prioritize eating if energy levels are getting low
        actionableLocation = SimUtilities::randomSelect(foodNearby, 1)[0];
        MapManager::eatElement(*this, actionableLocation);
    } else if (!matesNearby.empty() && currentEnergy > (0.5 * maxEnergy) && !availableLocations.empty() &&
               SimUtilities::getValUniformRandDist() > 0.85) {
        // Produce offspring if energy levels are at a high enough level and the probability threshold
        // is reached (to avoid overpopulation)
        actionableLocation = SimUtilities::randomSelect(availableLocations, 1)[0];
        MapManager::floraFauna.insert(
                pair(actionableLocation, make_unique<Herbivore>(charID, actionableLocation, foodChain, maxEnergy)));
    } else if (!availableLocations.empty()) {
        // Randomly pick a location to move to
        actionableLocation = SimUtilities::randomSelect(availableLocations, 1)[0];
        MapManager::moveElement(*this, actionableLocation);
    }
}