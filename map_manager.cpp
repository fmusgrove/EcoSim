#include "map_manager.hpp"
#include <iostream>


FloraFaunaList MapManager::floraFauna = {};
WaterObstacleList MapManager::terrain = {};

/**
 * Returns a vector of locations that contain edible food around the element
 * @param element ecosystem element to check surroundings on
 * @return vector of points that contain edible food
 */
vector<Point> MapManager::edibleFloraFaunaNearby(const EcosystemElement &element) {
    vector<Point> edibleLocations;
    Point location(element.getCachedLocation());

    Point north(location.first, location.second - 1);
    Point south(location.first, location.second + 1);
    Point east(location.first + 1, location.second);
    Point west(location.first - 1, location.second);

    auto northIter = MapManager::floraFauna.find(north);
    if (northIter != MapManager::floraFauna.end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), northIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(north);
        }
    }

    auto southIter = MapManager::floraFauna.find(south);
    if (southIter != MapManager::floraFauna.end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), southIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(south);
        }
    }

    auto eastIter = MapManager::floraFauna.find(east);
    if (eastIter != MapManager::floraFauna.end()) {
        if (find(element.getFoodChain().begin(), element.getFoodChain().end(), eastIter->second->getCharID()) !=
            element.getFoodChain().end()) {
            edibleLocations.push_back(east);
        }
    }

    auto westIter = MapManager::floraFauna.find(west);
    if (westIter != MapManager::floraFauna.end()) {
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
    Point location(element.getCachedLocation());

    Point north(location.first, location.second - 1);
    Point south(location.first, location.second + 1);
    Point east(location.first + 1, location.second);
    Point west(location.first - 1, location.second);

    auto northIterFloraFauna = MapManager::floraFauna.find(north);
    auto northIterTerrain = MapManager::terrain.find(north);
    if (northIterFloraFauna == MapManager::floraFauna.end() && northIterTerrain == MapManager::terrain.end()) {
        availableLocations.push_back(north);
    }

    auto southIterFloraFauna = MapManager::floraFauna.find(south);
    auto southIterTerrain = MapManager::terrain.find(south);
    if (southIterFloraFauna == MapManager::floraFauna.end() && southIterTerrain == MapManager::terrain.end()) {
        availableLocations.push_back(south);
    }

    auto eastIterFloraFauna = MapManager::floraFauna.find(east);
    auto eastIterTerrain = MapManager::terrain.find(east);
    if (eastIterFloraFauna == MapManager::floraFauna.end() && eastIterTerrain == MapManager::terrain.end()) {
        availableLocations.push_back(east);
    }

    auto westIterFloraFauna = MapManager::floraFauna.find(west);
    auto westIterTerrain = MapManager::terrain.find(west);
    if (westIterFloraFauna == MapManager::floraFauna.end() && westIterTerrain == MapManager::terrain.end()) {
        availableLocations.push_back(west);
    }

    return availableLocations;
}

/**
 * Moves an element to the specified location
 * @param ecosystemElement element to move
 * @param newLocation new location for the element to occupy
 */
void MapManager::moveElement(EcosystemElement &elementToMove, const Point &newLocation) {
    // Extract the node containing the old location
    auto nodeHandler = MapManager::floraFauna.extract(elementToMove.getCachedLocation());

    // Update the key
    nodeHandler.key() = newLocation;

    // Reinsert the modified node into the map
    MapManager::floraFauna.insert(move(nodeHandler));

    // Update the cached location of the element
    elementToMove.setCachedLocation(newLocation);
}