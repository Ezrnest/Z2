//
// Created by liyicheng on 2019/7/3.
//

#include "Tile.h"

using namespace z2;

Terrain Tile::getTerrain() {
    return terrain;
}

std::shared_ptr<Entity> Tile::getEntity() {
    return entity;
}

std::shared_ptr<Entity> Tile::removeEntity() {
    auto copy = entity;
    entity.reset();
    return copy;
}

Resource Tile::getResource() {
    return resource;
}

void Tile::setResource(const Resource &resource_) {
    Tile::resource = resource_;
}

bool Tile::hasEntity() {
    return entity != nullptr;
}

bool Tile::isOccupied() {
    return false;
}

void Tile::setTerrain(Terrain terrain_) {
    Tile::terrain = terrain_;
}

void Tile::setEntity(const shared_ptr<Entity>& entity_) {
    entity = entity_;
}

Visibility Tile::getVisibility(int playerId) {
    return visibility[playerId];
}

void Tile::setVisibility(int playerId, Visibility v) {
    visibility[playerId] = v;
}

void Tile::resetVisibility(int playerId) {
    Visibility v = visibility[playerId];
    Visibility newVisibility = (v == Visibility::DARK) ? Visibility::DARK : Visibility::GREY;
    visibility[playerId] = newVisibility;
}

void Tile::setPlayerCount(int count) {
    visibility.reserve(count);
    for(int i=0;i<count;i++){
        visibility[i] = Visibility::DARK;
    }
}

Tile::Tile() = default;


bool isBlocking(const Terrain t) {
    switch(t){
        case Terrain::PLAIN:
            return false;
        case Terrain::MOUNTAIN:
            return true;
    }
}
