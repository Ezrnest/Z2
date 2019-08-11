//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_GAMEMAPFROMSAVE_H
#define Z2_GAMEMAPFROMSAVE_H


#include "GameMap.h"
#include <iostream>
namespace z2 {

class GameMapFromSave : public GameMap {
private:
    shared_ptr<World> world;

public:
    GameMapFromSave(const string &name, const string &description, shared_ptr<World> world);

    bool saveGameSaving(const string &path);

    void saveTo(ostream &output);

    shared_ptr<MapPreview> getPreview() override;

    shared_ptr<World> buildWorld(const vector<PlayerSetting> &players) override;

    static shared_ptr<GameMapFromSave> loadFrom(istream &input);

    static shared_ptr<GameMapFromSave> loadGameSaving(const string& path);

};

}


#endif //Z2_GAMEMAPFROMSAVE_H
