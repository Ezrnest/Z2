/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Farmer.h"
#include <string>
using namespace std;
using namespace z2;
const string &z2::Farmer::className() {
    static string name = "Farmer";
    return name;
}

z2::Entity *z2::Farmer::create(int objectId, const Properties &initializer) {
    auto* entity = new Farmer(objectId);
    entity->initialize(initializer);
    return entity;
}

z2::Farmer::Farmer(unsigned int objectId) : GameUnit(objectId) {}

const string &Farmer::getClassName() const {
    return Farmer::className();
}

bool Farmer::canPerformAbility(const Point &pos, World &world) {
    Tile& tile = world.getTile(pos);
    switch (tile.getResource()) {
        case Resource::MINE:
        case Resource::GEM:
            return true;
        default:
            return false;
    }
}



void Farmer::doFarm(const Point &pos, World &world) {
    Player& player = world.getPlayer(ownerId_);
    Tile& tile = world.getTile(pos);
    int amount;
    switch (tile.getResource()){
        case Resource::NONE:{
            return;
        }
        case Resource::MINE:{
            amount = goldPerTurn;
            break;
        }
        case Resource::GEM:{
            amount = GEM_MULTIPLIER * goldPerTurn;
            break;
        }
        default:{
            amount = 0;
            break;
        }
    }
    player.addGold(amount);
//    player.setGold(player.getGold() + goldPerTurn);

    //TODO dispatch event
}

void Farmer::performAbility(const Point &pos, World &world) {
    doFarm(pos, world);
}

void Farmer::initialize(const Properties &prop) {
    GameUnit::initialize(prop);
    goldPerTurn = prop.getInt("goldPerTurn",goldPerTurn);

}




void Farmer::serializeDataPart(ostream &output) {
    GameUnit::serializeDataPart(output);
    output << goldPerTurn << ' ';
}


void Farmer::deserializeDataPart(istream &input, Farmer *en) {
    GameUnit::deserializeDataPart(input, en);
    input >> en->goldPerTurn;
}

void Farmer::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
}


Farmer *Farmer::loadFrom(istream &input) {
    auto* f = new Farmer(0);
    Farmer::deserializeDataPart(input, f);
    return f;
}

