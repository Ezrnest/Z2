//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_MAPREPOSITORY_H
#define Z2_MAPREPOSITORY_H

#include <world/GameMap.h>
#include "util/File.h"
namespace z2 {

class MapRepository {
private:
    vector<shared_ptr<GameMap>> maps;

public:
    vector<shared_ptr<GameMap>>& getMaps();


    void addMap(const shared_ptr<GameMap> &map);

    shared_ptr<GameMap> getMap(const string& name);

    static MapRepository& instance();

    static void initFromFolder(const ancono::File &folder);
};

}


#endif //Z2_MAPREPOSITORY_H
