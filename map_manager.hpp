#ifndef ECOSIM_MAP_MANAGER_HPP
#define ECOSIM_MAP_MANAGER_HPP

#include <vector>
#include <map>

#include "ecosystem_element.hpp"

using namespace std;

using FloraFaunaList = multimap<Point, unique_ptr<EcosystemElement>>;
using WaterObstacleList = map<Point, char>;


class MapManager {
public:
    /**
    * Returns a vector of locations that contain edible food around the element
    * @param element ecosystem element to check surroundings on
    * @return vector of points that contain edible food
    */
    static vector<Point> edibleFloraFaunaNearby(const EcosystemElement &element);

    /**
    * Returns a vector of empty locations on the map around the element
    * @param element ecosystem element to check surroundings on
    * @return vector of points that are free locations to move to
    */
    static vector<Point> freeLocations(const EcosystemElement &element);

    /**
     * Find viable mates in the location around an animal
     * @param element element to find mates for
     * @return vector of points that contain viable mates
     */
    static vector<Point> nearbyMates(const EcosystemElement &element);

    /**
    * Moves an element to the specified location
    * @param ecosystemElement element to move
    * @param newLocation new location for the element to occupy
    */
    static void moveElement(EcosystemElement &elementToMove, const Point &newLocation);

    /**
    * Moves the element doing the eating to the position of the animal to be eaten and consumes them
    * @param elementEating element doing the eating
    * @param locationToEat point location for the targeted element to be eaten
    */
    static void eatElement(EcosystemElement &elementEating, const Point &locationToEat);

    /**
    * Removes the element from the simulation
    * @param element element to remove
    */
    static void killElement(EcosystemElement &element);

    /**
     * Save the map out to the specified filepath
     * @param filePath filepath
     * @return boolean value representing if the operation was successful or not
     */
    static bool saveMapToFile(const string &filePath);

    static FloraFaunaList floraFauna;
    static WaterObstacleList terrain;
    static int mapRows;
    static int mapColumns;
};


#endif //ECOSIM_MAP_MANAGER_HPP
