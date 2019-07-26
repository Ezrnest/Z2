#include <utility>
#include <plog/Log.h>

//
// Created by liyicheng on 2019/7/22.
//

#include "SyncWorld.h"
#include "../../world/World.h"
using namespace z2;
z2::SyncWorld::SyncWorld() : ControlMessage(ControlMessageType::SyncWorld) {}

z2::SyncWorld::SyncWorld(shared_ptr<z2::World> world): ControlMessage(ControlMessageType::SyncWorld), world(std::move(world)) {}

const shared_ptr<World> &SyncWorld::getWorld() const {
    return world;
}

const string &SyncWorld::getClassName() const {
    static string name = "SyncWorld";
    return name;
}

void SyncWorld::deserializeData(istream &input) {
//    ControlMessage::deserializeData(input);
    auto* de = SerializableRegistry::instance().deserialize(input);
    auto* w = dynamic_cast<World*>(de);
    world = shared_ptr<World>(w);
    if(!world){
        PLOG_WARNING << "Failed to deserialize the world!";
    }
}

void SyncWorld::serializeData(ostream &output) {
//    ControlMessage::serializeData(output);
    world->serializeTo(output);
}

