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
    explicit MapManager();

    vector<Point> edibleFloraFaunaNearby(const EcosystemElement &element);

    vector<Point> freeLocations(const EcosystemElement &element);

//    void moveElement(const EcosystemElement &ecosystemElement, const Point &newLocation);

    unique_ptr<FloraFaunaList> floraFauna;
    unique_ptr<WaterObstacleList> terrain;
};


#endif //ECOSIM_MAP_MANAGER_HPP
