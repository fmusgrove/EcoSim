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
    static vector<Point> edibleFloraFaunaNearby(const EcosystemElement &element);

    static vector<Point> freeLocations(const EcosystemElement &element);

    static void moveElement(EcosystemElement &elementToMove, const Point &newLocation);

    static void eatElement(EcosystemElement &elementEating, const Point &locationToEat);

    static void killElement(EcosystemElement &element);

    static FloraFaunaList floraFauna;
    static WaterObstacleList terrain;
    static int mapRows;
    static int mapColumns;
};


#endif //ECOSIM_MAP_MANAGER_HPP
