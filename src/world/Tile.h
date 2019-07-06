//
// Created by liyicheng on 2019/7/3.
//
#ifndef Z2_TILE_H
#define Z2_TILE_H

namespace z2{
class Point;
}


#include <memory>
#include "Resource.h"
#include "../entity/Entity.h"
#include <vector>

namespace z2 {
    class Entity;

/**
* Describes the terrain of a single tile.
*/
enum class Terrain {
    PLAIN, MOUNTAIN
};

/**
 * Determines whether the Terrain is blocking.
 */
bool isBlocking(Terrain t);

using namespace std;
using namespace ancono;

enum class Visibility {
    DARK, GREY, CLEAR
};

class Tile {
private:
    Terrain terrain = Terrain::PLAIN;
    shared_ptr<Entity> entity;
    Resource resource = Resource::NONE;
    /**
     * The visibility of the players
     */
    vector<int> visibility;
public:

    Terrain getTerrain();

    void setTerrain(Terrain terrain_);

    Resource getResource();

    void setResource(const Resource &resource_);

    bool hasEntity();

    bool isOccupied();

    std::shared_ptr<Entity> getEntity();

    void setEntity(const shared_ptr<Entity>& entity_);

    std::shared_ptr<Entity> removeEntity();
};


}

#endif //Z2_TILE_H
