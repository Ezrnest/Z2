//
// Created by liyicheng on 2019/7/27.
//

#include "MapRepository.h"
#include "plog/Log.h"
namespace z2 {


void MapRepository::initFromFolder(const ancono::File &folder) {
    if(!folder.exists()){
        LOG_WARNING << "Unable to load map from: " << folder.getPath();
        return;
    }
    auto& repo = instance();
    auto entityFiles = folder.listSubfiles();
    for(File& f : entityFiles){
        ifstream in;
        f.inStream(in);
        shared_ptr<GameMap> m = GameMap::loadFrom(in);
        in.close();
        repo.addMap(m);
    }
}

MapRepository &MapRepository::instance() {
    static MapRepository repo;
    return repo;
}

void MapRepository::addMap(const shared_ptr<GameMap> &map) {
    maps.push_back(map);
    PLOG_INFO << "[MapRepository] Added map: " << map->getName();
}

vector<shared_ptr<GameMap>> &MapRepository::getMaps() {
    return maps;
}
}