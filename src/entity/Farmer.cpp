/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Farmer.h"
#include <string>
using namespace std;
using namespace z2;
const string &z2::Farmer::getIdentifier() {
    static string name = "Farmer";
    return name;
}

z2::Entity *z2::Farmer::create(int objectId, const Properties &initializer) {
    auto* entity = new Farmer(objectId);
    entity->initialize(initializer);
    return entity;
}

z2::Farmer::Farmer(int objectId) : GameUnit(objectId) {}

const string &Farmer::identifier() const {
    return Farmer::getIdentifier();
}

void Farmer::doFarm(const Point &pos, World &world) {
    Player& player = world.getPlayer(ownerId_);
    player.setGold(player.getGold() + goldPerTurn);
    //TODO dispatch event
}

void Farmer::performAbility(const Point &pos, World &world) {
    doFarm(pos, world);
}


