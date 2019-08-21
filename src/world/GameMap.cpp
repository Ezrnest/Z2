#include <memory>

#include <utility>

#include <utility>

//
// Created by liyicheng on 2019/7/27.
//

#include "GameMap.h"
#include "plog/Log.h"
#include "GameMapFromSave.h"

namespace z2 {


const string &GameMap::getDescription() {
    return description;
}

int GameMap::getMaxPlayerCount() const {
    return maxPlayerCount;
}



GameMap::GameMap(string name, string description, int maxPlayerCount)
        : description(std::move(description)), maxPlayerCount(maxPlayerCount),
          name(std::move(name)) {}


const string &GameMap::getName() const {
    return name;
}

bool GameMap::saveWorldTo(const string &path, const string& description, const shared_ptr<World> &world) {
    GameMapFromSave save(world->getMapName(),description,world);
    return save.saveGameSaving(path);
}

shared_ptr<GameMap> GameMap::loadWorldFrom(const string &path) {
    return GameMapFromSave::loadGameSaving(path);
}

bool GameMap::isValidPlayerCount(int count) const {
    return count > 0 && count <= maxPlayerCount;
}


}