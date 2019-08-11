//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_GAMEMAPPREDEFINED_H
#define Z2_GAMEMAPPREDEFINED_H


#include "GameMap.h"
#include "MapBase.h"

namespace z2 {

class GameMapPredefined : public GameMap {
private:
    vector<Point> bornPoints;
    MapBase mapBase;


    void setupInitialEntities(const PlayerSetting &ps, World &world);
public:
    GameMapPredefined(const string &name, const string &description, int maxPlayerCount,
                      vector<Point> bornPoints, MapBase mapBase);


    shared_ptr<World> buildWorld(const vector<PlayerSetting> &players) override;

    shared_ptr<MapPreview> getPreview() override;

    static shared_ptr<GameMap> loadFrom(istream& in);

    static MapTile getTileFromChar(char c);



};

}


#endif //Z2_GAMEMAPPREDEFINED_H
