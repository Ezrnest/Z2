//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_GAMEMAP_H
#define Z2_GAMEMAP_H


#include "World.h"
#include <memory>
#include <string>
#include "GameInitSetting.h"
#include "MapPreview.h"

using namespace std;
namespace z2 {

/**
 * The map of the game.
 */
class GameMap {
private:
    string name;
    string description;
    int maxPlayerCount;


public:
    GameMap(string name, string description, int maxPlayerCount);

    virtual shared_ptr<MapPreview> getPreview() = 0;

    virtual shared_ptr<World> buildWorld(const vector<PlayerSetting>& players) = 0;

    const string& getDescription();

    int getMaxPlayerCount() const;

    const string &getName() const;

    static bool saveWorldTo(const string &path, const string& description, const shared_ptr<World> &world);

    static shared_ptr<GameMap> loadWorldFrom(const string& path);

};

}


#endif //Z2_GAMEMAP_H
