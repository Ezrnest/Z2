//
// Created by liyicheng on 2019/8/10.
//

#include <entity/Building.h>
#include <entity/MeleeUnit.h>
#include <entity/RangeUnit.h>
#include <entity/Farmer.h>
#include "MapPreview.h"
#include "MapBase.h"
#include "Tile.h"
#include "entity/Entity.h"

namespace z2 {


MapPreview::MapPreview(int w, int h) : width(w), height(h) {
    data.reserve(w);
    for (int i = 0; i < w; i++) {
        data.emplace_back(h);
    }
}

int MapPreview::getWidth() const {
    return width;
}

int MapPreview::getHeight() const {
    return height;
}

void MapPreview::setTile(int x, int y, TilePreview tp) {
    data[x][y] = tp;
}

TilePreview MapPreview::getTile(int x, int y) {
    return data[x][y];
}

TilePreview MapPreview::fromMapTile(MapTile &mp) {
    return fromTerrainAndResource(mp.terrain,mp.resource);
}

TilePreview fromEntity(const shared_ptr<Entity> &en) {
    if (en->getEntityName() == "ConstructionBase") {
        return TilePreview::BornPoint;
    }
    if (dynamic_pointer_cast<Building>(en)) {
        return TilePreview::Building;
    }
    if (dynamic_pointer_cast<BattleUnit>(en)) {
        return TilePreview::BattleUnit;
    }
    if (dynamic_pointer_cast<Farmer>(en)) {
        return TilePreview::EcoUnit;
    }
    if (dynamic_pointer_cast<GameUnit>(en)) {
        return TilePreview::GameUnit;
    }
    return TilePreview::Plain;
//    if(dynamic_pointer_cast<MeleeUnit>(en) || dynamic_pointer_cast<RangeUnit>(en))
}

TilePreview MapPreview::fromTile(Tile &tile) {
    //TODO
    if (tile.hasEntity()) {
        return fromEntity(tile.getEntity());
    }
    return fromTerrainAndResource(tile.getTerrain(), tile.getResource());
}

void MapPreview::addBornPoint(const Point &p) {
    bornPoints.push_back(p);
}

const vector<Point> &MapPreview::getBornPoints() const {
    return bornPoints;
}

TilePreview MapPreview::fromTerrainAndResource(Terrain t, Resource r) {
    switch (r) {
        case Resource::MINE:
            return TilePreview::Mine;
        case Resource::GEM:
            return TilePreview::Gem;
        default:
            break;
    }
    switch (t) {
        case Terrain::MOUNTAIN:
            return TilePreview::Mountain;
        case Terrain ::WATER:
            return TilePreview ::Water;
        default:
            break;
    }
    return TilePreview::Plain;
}
}