//
// Created by liyicheng on 2019/8/10.
//

#include "GameMapFromSave.h"

#include <memory>
#include <utility>
#include <util/File.h>

namespace z2 {


GameMapFromSave::GameMapFromSave(const string &name, const string &description,
                                 shared_ptr<World> world) : GameMap(name, description, world->getPlayerCount()),
                                                                   world(std::move(world)) {}

shared_ptr<GameMapFromSave> GameMapFromSave::loadFrom(istream &input) {
    Properties prop;
    prop.loadFrom(input);
    string name = prop.get("name", "Unknown");
    string description = prop.get("description", "");
    auto ser = SerializableRegistry::instance().deserialize(input);
    shared_ptr<World> world(dynamic_cast<World*>(ser));
    if(!world){
        return shared_ptr<GameMapFromSave>();
    }
    return std::make_shared<GameMapFromSave>(name, description, world);
}

void GameMapFromSave::saveTo(ostream &output) {
    Properties prop;
    prop.set("name", getName());
    prop.set("description", getDescription());
    prop.saveTo(output);
    output << "$$" << endl;
    world->serializeTo(output);
}

shared_ptr<World> GameMapFromSave::buildWorld(const vector<PlayerSetting> &players) {
    return std::make_shared<World>(*world);
}

shared_ptr<GameMapFromSave> GameMapFromSave::loadGameSaving(const string &path) {
    ifstream in(path.c_str());
    if(!in.good()){
        return shared_ptr<GameMapFromSave>();
    }
    auto t = loadFrom(in);
    in.close();
    return t;
}

bool GameMapFromSave::saveGameSaving(const string &path) {
    ofstream out(path.c_str());
    if(!out.good()){
        return false;
    }
    saveTo(out);
    out.close();
    return true;
}

shared_ptr<MapPreview> GameMapFromSave::getPreview() {
    //TODO
    return shared_ptr<MapPreview>();
}

}