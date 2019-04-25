#ifndef ECOSIM_MAP_MANAGER_HPP
#define ECOSIM_MAP_MANAGER_HPP

#import <vector>
#import <map>

#import "ecosystem_element.hpp"

using namespace std;

using WaterObstacleList = map<Point, char>;
using FloraFaunaList = multimap<Point, unique_ptr<EcosystemElement>>;


class MapManager {
public:
    static vector<Point> edibleFloraFaunaNearby(const EcosystemElement &element);

    static vector<Point> freeLocations(const EcosystemElement &element);

    static void moveElement(EcosystemElement &elementToMove, const Point &newLocation);

//    static void eatElement(const EcosystemElement &elementEating, const Point &locationToEat);

    static FloraFaunaList floraFauna;
    static WaterObstacleList terrain;
};


#endif //ECOSIM_MAP_MANAGER_HPP
