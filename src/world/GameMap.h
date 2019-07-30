//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_GAMEMAP_H
#define Z2_GAMEMAP_H


#include "World.h"
#include <memory>
#include <string>
#include "GameInitSetting.h"
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

/**
 * The map of the game.
 */
class GameMap {
private:
    string name;
    string description;
    int maxPlayerCount;
    vector<Point> bornPoints;
    MapBase mapBase;


    void setupInitialEntities(const PlayerSetting &ps, World &world);

public:
    GameMap(string name, string description, int maxPlayerCount,  const vector<Point>&  bornPoints,
            MapBase mapBase);



    shared_ptr<World> buildWorld(const vector<PlayerSetting>& players);

    const string& getDescription();

    int getMaxPlayerCount() const;

    const string &getName() const;

    static shared_ptr<GameMap> loadFrom(istream& in);

    static MapTile getTileFromChar(char c);


};

}


#endif //Z2_GAMEMAP_H
