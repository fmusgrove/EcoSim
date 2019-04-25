#include "map_manager.hpp"

MapManager::MapManager() {
    floraFauna = make_unique<FloraFaunaList>();
    terrain = make_unique<WaterObstacleList>();
}

/**
 * Returns a vector of locations that contain edible food around the element
 * @param element ecosystem element to check surroundings on
 * @return vector of points that contain edible food
 */
vector<Point> MapManager::edibleFloraFaunaNearby(const EcosystemElement &element) {
    vector<Point> edibleLocations;

    Point north(element.cachedLocation.first, element.cachedLocation.second + 1);
    Point south(element.cachedLocation.first, element.cachedLocation.second - 1);
    Point east(element.cachedLocation.first + 1, element.cachedLocation.second);
    Point west(element.cachedLocation.first - 1, element.cachedLocation.second);

    auto northIter = this->floraFauna->find(north);
    if (northIter != this->floraFauna->end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), northIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(north);
        }
    }

    auto southIter = this->floraFauna->find(south);
    if (southIter != this->floraFauna->end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), southIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(south);
        }
    }

    auto eastIter = this->floraFauna->find(east);
    if (eastIter != this->floraFauna->end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), eastIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(east);
        }
    }

    auto westIter = this->floraFauna->find(west);
    if (westIter != this->floraFauna->end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), westIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(west);
        }
    }

    return edibleLocations;
}

/**
 * Returns a vector of empty locations on the map around the element
 * @param element ecosystem element to check surroundings on
 * @return vector of points that are free locations to move to
 */
vector<Point> MapManager::freeLocations(const EcosystemElement &element) {
    vector<Point> availableLocations;

    Point north(element.cachedLocation.first, element.cachedLocation.second + 1);
    Point south(element.cachedLocation.first, element.cachedLocation.second - 1);
    Point east(element.cachedLocation.first + 1, element.cachedLocation.second);
    Point west(element.cachedLocation.first - 1, element.cachedLocation.second);

    auto northIterFloraFauna = this->floraFauna->find(north);
    auto northIterTerrain = this->terrain->find(north);
    if (northIterFloraFauna == this->floraFauna->end() && northIterTerrain == this->terrain->end()) {
        availableLocations.push_back(north);
    }

    auto southIterFloraFauna = this->floraFauna->find(south);
    auto southIterTerrain = this->terrain->find(south);
    if (southIterFloraFauna == this->floraFauna->end() && southIterTerrain == this->terrain->end()) {
        availableLocations.push_back(south);
    }

    auto eastIterFloraFauna = this->floraFauna->find(east);
    auto eastIterTerrain = this->terrain->find(east);
    if (eastIterFloraFauna == this->floraFauna->end() && eastIterTerrain == this->terrain->end()) {
        availableLocations.push_back(east);
    }

    auto westIterFloraFauna = this->floraFauna->find(west);
    auto westIterTerrain = this->terrain->find(west);
    if (westIterFloraFauna == this->floraFauna->end() && westIterTerrain == this->terrain->end()) {
        availableLocations.push_back(west);
    }

    return availableLocations;
}