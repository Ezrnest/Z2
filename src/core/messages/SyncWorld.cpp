#include <utility>

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
    world = shared_ptr<World>(World::loadFrom(input));
}

void SyncWorld::serializeData(ostream &output) {
//    ControlMessage::serializeData(output);
    world->serializeTo(output);
}

