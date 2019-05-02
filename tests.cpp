#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <map>
#include <unordered_map>

#include "sim_utilities.hpp"
#include "map_manager.hpp"
#include "species_type.hpp"
#include "ecosystem_element.hpp"
#include "plant.hpp"
#include "herbivore.hpp"
#include "omnivore.hpp"

TEST_CASE("EcoSim Test Suite") {
    SECTION("Map and species file loading") {
        unordered_map<char, SimUtilities::SpeciesTraits> speciesList;
        // Load species list
        speciesList = SimUtilities::loadSpeciesList("test_input/species.txt");

        // Load map into memory
        SimUtilities::loadMap("test_input/map.txt", speciesList);

        REQUIRE(MapManager::mapRows == 10);
        REQUIRE(MapManager::mapColumns == 45);

        int numPlants = 0;
        for (auto &element: MapManager::floraFauna) {
            if (element.second->getSpeciesType() == SpeciesType::PLANT) {
                numPlants++;
            }
        }

        int numHerbivores = 0;
        for (auto &element: MapManager::floraFauna) {
            if (element.second->getSpeciesType() == SpeciesType::HERBIVORE) {
                numHerbivores++;
            }
        }

        int numOmnivores = 0;
        for (auto &element: MapManager::floraFauna) {
            if (element.second->getSpeciesType() == SpeciesType::OMNIVORE) {
                numOmnivores++;
            }
        }

        REQUIRE(numPlants == 35);
        REQUIRE(numHerbivores == 8);
        REQUIRE(numOmnivores == 10);
    }

    SECTION("Edible flora fauna around animals") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(40, 1));
        auto foodNearby = MapManager::edibleFloraFaunaNearby(*floraFaunaIter->second);
        REQUIRE(foodNearby.empty());

        floraFaunaIter = MapManager::floraFauna.find(Point(44, 5));
        foodNearby = MapManager::edibleFloraFaunaNearby(*floraFaunaIter->second);
        REQUIRE(foodNearby.size() == 1);
    }

    SECTION("Free locations around animals") {
        // Test location on top of map
        auto floraFaunaIter = MapManager::floraFauna.find(Point(13, 0));
        auto availableLocations = MapManager::freeLocations(*floraFaunaIter->second);
        REQUIRE(availableLocations.size() == 3);

        // Test location surrounded on all sides
        floraFaunaIter = MapManager::floraFauna.find(Point(5, 1));
        availableLocations = MapManager::freeLocations(*floraFaunaIter->second);
        REQUIRE(availableLocations.empty());

        // Test location on side of map with other element near
        floraFaunaIter = MapManager::floraFauna.find(Point(44, 5));
        availableLocations = MapManager::freeLocations(*floraFaunaIter->second);
        REQUIRE(availableLocations.size() == 2);
    }

    SECTION("Available mates around animals") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(10, 9));
        auto matesNearby = MapManager::nearbyMates(*floraFaunaIter->second);
        REQUIRE(matesNearby.size() == 1);

        floraFaunaIter = MapManager::floraFauna.find(Point(5, 1));
        matesNearby = MapManager::nearbyMates(*floraFaunaIter->second);
        REQUIRE(matesNearby.empty());
    }

    SECTION("General movement") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(21, 8));
        MapManager::moveElement(*floraFaunaIter->second, Point(21, 7));

        floraFaunaIter = MapManager::floraFauna.find(Point(21, 7));

        REQUIRE(floraFaunaIter != MapManager::floraFauna.end());

        // Check that the cached location was updated properly
        REQUIRE(floraFaunaIter->second->getCachedLocation() == Point(21, 7));
    }

    SECTION("Movement over plants") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(17, 6));
        MapManager::moveElement(*floraFaunaIter->second, Point(17, 5));

        auto foundElements = MapManager::floraFauna.equal_range(Point(17, 5));
        int numFoundElements = distance(foundElements.first, foundElements.second);

        REQUIRE(numFoundElements == 2);
    }

    SECTION("Herbivores eating") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(18, 6));
        MapManager::eatElement(*floraFaunaIter->second, Point(18, 5));

        floraFaunaIter = MapManager::floraFauna.find(Point(18, 5));

        REQUIRE(floraFaunaIter->second->getIsGrown() == false);
    }

    SECTION("Omnivores eating") {
        auto floraFaunaIter = MapManager::floraFauna.find(Point(10, 8));
        MapManager::eatElement(*floraFaunaIter->second, Point(10, 9));

        auto foundElements = MapManager::floraFauna.equal_range(Point(10, 9));

        for (auto elementIter = foundElements.first; elementIter != foundElements.second; ++elementIter) {
            if (elementIter->second->getCharID() == 'C') {
                REQUIRE(elementIter->second->getCurrentEnergy() == 0);
            } else if (elementIter->second->getCharID() == 'D') {
                REQUIRE(elementIter->second->getCurrentEnergy() == elementIter->second->getMaxEnergy());
            }
        }
    }
}
