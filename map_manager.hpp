#ifndef ECOSIM_MAP_MANAGER_HPP
#define ECOSIM_MAP_MANAGER_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

#include "ecosystem_element.hpp"

using namespace std;

using WaterObstacleList = map<Point, char>;
using FloraFaunaList = multimap<Point, unique_ptr<EcosystemElement>>;


class MapManager {
public:
    explicit MapManager();

    static vector<Point> edibleFloraFaunaNearby(const shared_ptr<EcosystemElement> &element);

    static vector<Point> freeLocations(const shared_ptr<EcosystemElement> &element);

    static void moveElement(shared_ptr<EcosystemElement> &elementToMove, const Point &newLocation);

//    static void eatElement(const EcosystemElement &elementEating, const Point &locationToEat);

    static unique_ptr<FloraFaunaList> floraFauna;
    static unique_ptr<WaterObstacleList> terrain;
};


#endif //ECOSIM_MAP_MANAGER_HPP
