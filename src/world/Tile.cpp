//
// Created by liyicheng on 2019/7/3.
//

#include "Tile.h"
#include "entity/GameUnit.h"
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

bool z2::isBlocking(const Terrain t) {
    switch(t){
        case Terrain::PLAIN:
            return false;
        case Terrain::MOUNTAIN:
        case Terrain::WATER:
            return true;
        default:
            return false;
    }
}


bool Tile::isOccupied() {
    return ::isBlocking(terrain) || hasEntity() ;
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
    visibility.resize(count);
    for(int i=0;i<count;i++){
        visibility[i] = Visibility::DARK;
    }
}

bool Tile::canPassThrough(const shared_ptr<GameUnit> &unit) {
    return (!isBlocking(terrain)) && (!hasEntity() || entity->getOwnerId() == unit->getOwnerId());
}

int Tile::getTerrainRMP(Terrain t) {
    switch (t) {
        case Terrain::PLAIN:
        case Terrain::DESERT:
            return 1;
        case Terrain::FOREST:
        case Terrain::HILL:
            return 2;
        case Terrain::MOUNTAIN:
        case Terrain::WATER:
            return 1000; // not passable
    }
}

int Tile::getBaseRMP() const {
    return getTerrainRMP(terrain);
}

Tile::Tile() = default;


