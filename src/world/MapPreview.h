//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_MAPPREVIEW_H
#define Z2_MAPPREVIEW_H

#include <vector>
using namespace std;
namespace z2 {
class MapTile;
class Tile;
enum class TilePreview{
    Plain,
    Mountain,
    Water,
    Mine,
    Gem,
    BornPoint,
    Building,
    BattleUnit,
    EcoUnit,
    GameUnit
};

class MapPreview {
private:
    int width;
    int height;
    vector<vector<TilePreview>> data;
    vector<Point> bornPoints;
public:
    MapPreview(int w,int h);

    void setTile(int x,int y, TilePreview tp);

    void addBornPoint(const Point& p);

    TilePreview getTile(int x, int y);

    int getWidth() const;

    int getHeight() const;

    const vector<Point> &getBornPoints() const;

    static TilePreview fromTerrainAndResource(Terrain t, Resource r);

    static TilePreview fromMapTile(MapTile& mp);

    static TilePreview fromTile(Tile &tile);

};

}


#endif //Z2_MAPPREVIEW_H
