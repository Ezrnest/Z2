//
// Created by liyicheng on 2019/8/10.
//

#include "MapBase.h"
#include "World.h"
namespace z2 {

MapBase::MapBase(int w, int h) : width(w), height(h){
    data.reserve(w);
    for(int i=0;i<w;i++){
        data.emplace_back(h);
    }
}

int MapBase::getWidth() const {
    return width;
}

int MapBase::getHeight() const {
    return height;
}

const vector<vector<MapTile>> &MapBase::getData() const {
    return data;
}

void MapBase::setTile(int x, int y, Terrain t, Resource r) {
    data[x][y].terrain = t;
    data[x][y].resource = r;
}

void MapBase::setDataTo(World &w) {
    for(int i=0;i<width;i++){
        for (int j = 0; j < height; j++) {
            Tile &t = w.getTile(i, j);
            data[i][j].setTo(t);
        }
    }
}

MapTile &MapBase::tile(int x, int y) {
    return data[x][y];
}

void MapTile::setTo(Tile &t) {
    t.setTerrain(terrain);
    t.setResource(resource);
}

MapTile::MapTile(Terrain terrain, Resource resource) : terrain(terrain), resource(resource) {}

MapTile::MapTile() = default;

}