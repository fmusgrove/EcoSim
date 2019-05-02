#include "map_manager.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>


FloraFaunaList MapManager::floraFauna = {};
WaterObstacleList MapManager::terrain = {};
int MapManager::mapRows = 0;
int MapManager::mapColumns = 0;

vector<Point> MapManager::edibleFloraFaunaNearby(const EcosystemElement &element) {
    vector<Point> edibleLocations;
    Point location(element.getCachedLocation());
    vector<char> foodChain = element.getFoodChain();

    vector<Point> cardinalPoints = {Point(location.first, location.second - 1),
                                    Point(location.first, location.second + 1),
                                    Point(location.first + 1, location.second),
                                    Point(location.first - 1, location.second)};

    for (Point &pointToCheck: cardinalPoints) {
        auto foundElements = MapManager::floraFauna.equal_range(pointToCheck);
        int numFoundElements = distance(foundElements.first, foundElements.second);

        if (numFoundElements == 1) {
            if (find(foodChain.begin(), foodChain.end(), foundElements.first->second->getCharID()) != foodChain.end()) {
                if (foundElements.first->second->getSpeciesType() == SpeciesType::PLANT) {
                    // Only add fully grown plants to the edible locations
                    if (foundElements.first->second->getIsGrown() == true) {
                        edibleLocations.push_back(pointToCheck);
                    }
                } else {
                    edibleLocations.push_back(pointToCheck);
                }
            }
        }
    }

    return edibleLocations;
}

vector<Point> MapManager::freeLocations(const EcosystemElement &element) {
    vector<Point> availableLocations;
    Point location(element.getCachedLocation());

    Point north(location.first, location.second - 1);
    Point south(location.first, location.second + 1);
    Point east(location.first + 1, location.second);
    Point west(location.first - 1, location.second);

    if (north.second >= 0) {
        auto northIterFloraFauna = MapManager::floraFauna.find(north);
        auto northIterTerrain = MapManager::terrain.find(north);
        if ((northIterFloraFauna == MapManager::floraFauna.end() ||
             northIterFloraFauna->second->getSpeciesType() == SpeciesType::PLANT) &&
            northIterTerrain == MapManager::terrain.end()) {
            availableLocations.push_back(north);
        }
    }

    if (south.second < MapManager::mapRows) {
        auto southIterFloraFauna = MapManager::floraFauna.find(south);
        auto southIterTerrain = MapManager::terrain.find(south);
        if ((southIterFloraFauna == MapManager::floraFauna.end() ||
             southIterFloraFauna->second->getSpeciesType() == SpeciesType::PLANT) &&
            southIterTerrain == MapManager::terrain.end()) {
            availableLocations.push_back(south);
        }
    }

    if (east.first < MapManager::mapColumns) {
        auto eastIterFloraFauna = MapManager::floraFauna.find(east);
        auto eastIterTerrain = MapManager::terrain.find(east);
        if ((eastIterFloraFauna == MapManager::floraFauna.end() ||
             eastIterFloraFauna->second->getSpeciesType() == SpeciesType::PLANT) &&
            eastIterTerrain == MapManager::terrain.end()) {
            availableLocations.push_back(east);
        }
    }

    if (west.first >= 0) {
        auto westIterFloraFauna = MapManager::floraFauna.find(west);
        auto westIterTerrain = MapManager::terrain.find(west);
        if ((westIterFloraFauna == MapManager::floraFauna.end() ||
             westIterFloraFauna->second->getSpeciesType() == SpeciesType::PLANT) &&
            westIterTerrain == MapManager::terrain.end()) {
            availableLocations.push_back(west);
        }
    }

    return availableLocations;
}

vector<Point> MapManager::nearbyMates(const EcosystemElement &element) {
    vector<Point> matesNearby;
    Point location(element.getCachedLocation());

    vector<Point> cardinalPoints = {Point(location.first, location.second - 1),
                                    Point(location.first, location.second + 1),
                                    Point(location.first + 1, location.second),
                                    Point(location.first - 1, location.second)};

    for (Point &pointToCheck: cardinalPoints) {
        auto foundElementIter = MapManager::floraFauna.find(pointToCheck);
        if (foundElementIter != MapManager::floraFauna.end()) {
            if (foundElementIter->second->getCharID() == element.getCharID() &&
                foundElementIter->second->getCurrentEnergy() > (0.5 * foundElementIter->second->getMaxEnergy())) {
                matesNearby.push_back(pointToCheck);
            }
        }
    }

    return matesNearby;
}

void MapManager::moveElement(EcosystemElement &elementToMove, const Point &newLocation) {
    // Extract the element(s) at the old location
    auto foundElements = MapManager::floraFauna.equal_range(elementToMove.getCachedLocation());

    // Scan through the elements
    for (auto elementsIter = foundElements.first; elementsIter != foundElements.second; ++elementsIter) {
        // If the element is not a plant (ie. if an herbivore is in the same location after eating the plant) then
        // move the element to the new location
        if (elementsIter->second->getSpeciesType() != SpeciesType::PLANT) {
            auto nodeHandler = MapManager::floraFauna.extract(elementsIter);
            nodeHandler.key() = newLocation;
            MapManager::floraFauna.insert(move(nodeHandler));
            break;
        }
    }

    // Update the cached location of the element
    elementToMove.setCachedLocation(newLocation);

    // Decrease the energy level by 1
    elementToMove.setCurrentEnergy(elementToMove.getCurrentEnergy() - 1);
}

void MapManager::eatElement(EcosystemElement &elementEating, const Point &locationToEat) {
    // Find element at the location
    auto elementToEatIter = MapManager::floraFauna.find(locationToEat);

    // If the element exists, eat it
    if (elementToEatIter != MapManager::floraFauna.end()) {
        int energyToAdd = elementToEatIter->second->getCurrentEnergy();
        elementToEatIter->second->makeEaten();

        // Move to the new location
        MapManager::moveElement(elementEating, locationToEat);

        // Update energy level of element doing the eating
        elementEating.setCurrentEnergy(
                min(elementEating.getCurrentEnergy() + energyToAdd, elementEating.getMaxEnergy()));
    }
}

void MapManager::killElement(EcosystemElement &element) {
    // Extract the element(s) at the location
    auto foundElements = MapManager::floraFauna.equal_range(element.getCachedLocation());

    // Scan through the elements
    for (auto elementsIter = foundElements.first; elementsIter != foundElements.second; ++elementsIter) {
        // If the element is not a plant (ie. if an herbivore is in the same location after eating the plant) then
        // go ahead and kill the element
        if (elementsIter->second->getSpeciesType() != SpeciesType::PLANT) {
            MapManager::floraFauna.erase(elementsIter);
            break;
        }
    }
}

bool MapManager::saveMapToFile(const string &filePath) {
    vector<string> mapLines;
    Point currentLocation;
    ofstream mapFileStream(filePath);
    bool isFirstLineWritten = false;

    if (!mapFileStream.is_open()) {
        return false;
    }

    for (int currentRow = 0; currentRow < mapRows; currentRow++) {
        string lineToAdd;
        for (int currentCol = 0; currentCol < mapColumns; currentCol++) {
            currentLocation = {currentCol, currentRow};
            auto floraFaunaIter = MapManager::floraFauna.find(currentLocation);
            auto terrainIter = MapManager::terrain.find(currentLocation);
            if (floraFaunaIter != MapManager::floraFauna.end()) {
                // Animal element
                lineToAdd += floraFaunaIter->second->getCharID();
            } else if (terrainIter != MapManager::terrain.end()) {
                // Terrain element
                lineToAdd += terrainIter->second;
            } else {
                // Empty space
                lineToAdd += " ";
            }
        }
        // Write the line to the file
        if (isFirstLineWritten) {
            mapFileStream << endl;
            mapFileStream << lineToAdd;
        } else {
            mapFileStream << lineToAdd;
            isFirstLineWritten = true;
        }
    }

    mapFileStream.close();
    return true;
}
