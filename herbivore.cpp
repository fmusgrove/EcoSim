#include "herbivore.hpp"
#include "map_manager.hpp"
#include <iostream>

using namespace std;

void Herbivore::tick() {
    auto availableLocations = MapManager::freeLocations(*this);
}