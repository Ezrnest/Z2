//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_MAPBASE_H
#define Z2_MAPBASE_H

#include <c++/vector>
#include "Tile.h"

using namespace std;

namespace z2 {


class MapTile{
public:
    Terrain terrain = Terrain::PLAIN;
    Resource resource = Resource::NONE;

    MapTile(Terrain terrain, Resource resource);

    MapTile();

    void setTo(Tile& t);


};



class MapBase{
private:
    int width;
    int height;
    vector<vector<MapTile>> data;
public:
    MapBase(int w,int h);

    void setTile(int x,int y, Terrain t,Resource r);

    MapTile& tile(int x, int y);

    int getWidth() const;

    int getHeight() const;

    void setDataTo(World& w);

    const vector<vector<MapTile>> &getData() const;
};

}


#endif //Z2_MAPBASE_H
