//
// Created by liyicheng on 2019/7/3.
//
#ifndef Z2_TILE_H
#define Z2_TILE_H

namespace z2 {
class Point;
}


#include <memory>
#include "Resource.h"
#include "../entity/Entity.h"
#include <vector>

namespace z2 {
class Entity;
class GameUnit;

/**
* Describes the terrain of a single tile.
*/
enum class Terrain {
    PLAIN = 0, MOUNTAIN = 1
};

/**
 * Determines whether the Terrain is blocking.
 */
bool isBlocking(Terrain t);

using namespace std;
using namespace ancono;

enum class Visibility {
    DARK = 0, GREY = 1, CLEAR = 2
};

class Tile {
private:
    Terrain terrain = Terrain::PLAIN;
    Resource resource = Resource::NONE;
    shared_ptr<Entity> entity;
    /**
     * The visibility of the players
     */
    vector<Visibility> visibility;
public:
    Tile();

//    Tile(const Tile& tile) = delete;
//
//    Tile& operator=(const Tile& tile) = delete;

    Terrain getTerrain();

    void setTerrain(Terrain terrain_);

    Resource getResource();

    void setResource(const Resource &resource_);

    Visibility getVisibility(int playerId);

    void setVisibility(int playerId, Visibility v);

    bool hasEntity();

    bool isOccupied();

    /**
     * Determines whether an unit of the player can pass through this tile.
     */
    bool canPassThrough(const shared_ptr<GameUnit>& unit);

    std::shared_ptr<Entity> getEntity();

    void setEntity(const shared_ptr<Entity> &entity_);

    std::shared_ptr<Entity> removeEntity();

    /**
     * If v != `DARK`, sets it to `GRAY`.
     */
    void resetVisibility(int playerId);

    void setPlayerCount(int count);

    friend class World;
};


}

#endif //Z2_TILE_H
