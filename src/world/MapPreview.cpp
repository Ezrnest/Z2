//
// Created by liyicheng on 2019/8/10.
//

#include "MapPreview.h"
#include "MapBase.h"
#include "Tile.h"
namespace z2 {


MapPreview::MapPreview(int w, int h) : width(w), height(h){
    data.reserve(w);
    for(int i=0;i<w;i++){
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
    switch (mp.resource) {
        case Resource::MINE:
            return TilePreview ::Mine;
        case Resource::GEM:
            return TilePreview::Gem;
        default:
            break;
    }
    switch (mp.terrain) {
        case Terrain::MOUNTAIN:
            return TilePreview::Mountain;
        default:
            break;
    }
    return TilePreview::Plain;
}

TilePreview MapPreview::fromTile(Tile &tile) {
    //TODO



    return TilePreview::Plain;
}
}